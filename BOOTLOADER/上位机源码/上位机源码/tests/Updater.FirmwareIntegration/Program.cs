using System.Diagnostics;
using System.Text.Json;
using Updater.Core;

// Real host client + encoded frames + actual compiled C initialization/protocol/MD5.
// Flash is a RAM model; this does not emulate SCI timing or execute target jumps.
if (args.Length != 4) throw new ArgumentException("python firmware-test-script python-deps firmware-build-directory");
await using var transport = new FirmwareTransport(args[0], args[1], args[2]);
await transport.Start();
var client = new Avp335UpdaterClient(transport);
int completed = 0;
void Assert(bool condition) { if (!condition) throw new Exception("Firmware integration assertion failed"); }
async Task Upload(Avp335FirmwareImage image)
{
    int last = 0;
    await client.UpdateAsync(image, new ImmediateProgress(p => last = p.Percent), CancellationToken.None);
    var stats = await transport.Stats();
    completed++;
    Assert(last == 100 && stats.GetProperty("flag").GetInt32() == 0xaa);
    Assert(stats.GetProperty("commit").GetInt32() == completed);
    Assert(stats.GetProperty("erase").GetInt32() == 2 * completed);
}
foreach (string crystal in new[] { "30MHz", "33.3MHz" })
{
    var image = Avp335OutLoader.Load(Path.Combine(args[3], crystal, "APP", $"AVP32F335_APP_{crystal}.out"));
    transport.DropOnce.UnionWith(new ushort[] { 0x70, 0x73, 0x74 });
    int before = (await transport.Stats()).GetProperty("program").GetInt32();
    await Upload(image);
    int after = (await transport.Stats()).GetProperty("program").GetInt32();
    Assert(after - before == (image.Bytes.Length + 127) / 128 + 1 + (completed > 1 ? 1 : 0));
    Console.WriteLine($"PASS real {crystal} OUT: lost begin/data/finish ACK, exact write count, MD5 and valid flag");
}
var original = Avp335OutLoader.Load(Path.Combine(args[3], "30MHz", "APP", "AVP32F335_APP_30MHz.out"));
byte[] full = new byte[checked((int)Avp335Profile.MaxBytes)];
original.Bytes.CopyTo(full, 0);
await Upload(original with { Bytes = full });
Console.WriteLine("PASS 448 KB minus 4, short final packet, full C firmware MD5 commit");
using (var cancel = new CancellationTokenSource())
{
    try
    {
        await client.UpdateAsync(original, new ImmediateProgress(p => { if (p.Percent > 0) cancel.Cancel(); }), cancel.Token);
        throw new Exception("Cancellation ignored");
    }
    catch (OperationCanceledException) when (cancel.IsCancellationRequested) { }
    var stats = await transport.Stats();
    Assert(stats.GetProperty("flag").GetInt32() != 0xaa && stats.GetProperty("commit").GetInt32() == completed);
    // One cancelled BEGIN erased the two groups without committing.
    completed++;
}
await client.UpdateAsync(original, null, CancellationToken.None);
var final = await transport.Stats();
Assert(final.GetProperty("flag").GetInt32() == 0xaa && final.GetProperty("commit").GetInt32() == completed);
await client.ResetAsync(CancellationToken.None);
Assert((await transport.Stats()).GetProperty("action").GetInt32() == 2);
Console.WriteLine("PASS cancellation leaves invalid APP; retry commits; reset command requests APP jump");

sealed class ImmediateProgress(Action<UpdateProgress> action) : IProgress<UpdateProgress>
{ public void Report(UpdateProgress value) => action(value); }

sealed class FirmwareTransport : IAvp335Transport
{
    private readonly Process process;
    public readonly HashSet<ushort> DropOnce = [];
    public FirmwareTransport(string python, string script, string dependencies)
    {
        var start = new ProcessStartInfo(python) { RedirectStandardInput = true, RedirectStandardOutput = true,
            RedirectStandardError = true, UseShellExecute = false, CreateNoWindow = true };
        foreach (var value in new[] { script, "--deps", dependencies, "--serve" }) start.ArgumentList.Add(value);
        process = new Process { StartInfo = start };
    }
    public async Task Start()
    {
        process.Start();
        if (await process.StandardOutput.ReadLineAsync() != "READY")
            throw new Exception(await process.StandardError.ReadToEndAsync());
    }
    public async Task<Avp335Packet> ExchangeAsync(Avp335Packet request, TimeSpan timeout, CancellationToken cancellation)
    {
        cancellation.ThrowIfCancellationRequested();
        await process.StandardInput.WriteLineAsync(Convert.ToHexString(request.Encode()));
        await process.StandardInput.FlushAsync();
        string response = await process.StandardOutput.ReadLineAsync().WaitAsync(timeout) ?? throw new EndOfStreamException();
        var reply = Avp335Packet.Decode(Convert.FromHexString(response));
        if (DropOnce.Remove(request.Command)) throw new TimeoutException("Injected lost ACK after actual C execution");
        return reply;
    }
    public async Task<JsonElement> Stats()
    {
        await process.StandardInput.WriteLineAsync("stats");
        await process.StandardInput.FlushAsync();
        using var doc = JsonDocument.Parse(await process.StandardOutput.ReadLineAsync() ?? throw new EndOfStreamException());
        return doc.RootElement.Clone();
    }
    public async ValueTask DisposeAsync()
    {
        if (process.Id != 0) { process.StandardInput.Close(); await process.WaitForExitAsync(); }
        process.Dispose();
    }
}
