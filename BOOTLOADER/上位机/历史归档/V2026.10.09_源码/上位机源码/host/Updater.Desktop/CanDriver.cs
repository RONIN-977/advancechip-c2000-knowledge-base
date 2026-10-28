using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;

namespace Updater.Desktop;

internal static class CanDriver
{
    public const string CanProDriver = @"C:\ZLGCANPRO\ZCANPRO\kerneldlls\usbcan.dll";
    public static string DefaultPath
    {
        get
        {
            string bundled = Path.Combine(AppContext.BaseDirectory, "drivers", "zlg", "x86", "usbcan.dll");
            if (File.Exists(bundled)) return bundled;
            string local = Path.Combine(AppContext.BaseDirectory, "ControlCAN.dll");
            return File.Exists(local) ? local : File.Exists(CanProDriver) ? CanProDriver : local;
        }
    }
    public static bool IsX86(string path)
    {
        using var reader = new BinaryReader(File.OpenRead(path));
        if (reader.ReadUInt16() != 0x5a4d) throw new InvalidDataException("CAN 库不是 Windows DLL。");
        reader.BaseStream.Position = 0x3c; int offset = reader.ReadInt32();
        if (offset < 0 || offset > reader.BaseStream.Length - 6) throw new InvalidDataException("CAN DLL 文件头损坏。");
        reader.BaseStream.Position = offset;
        if (reader.ReadUInt32() != 0x4550) throw new InvalidDataException("CAN DLL PE 签名错误。");
        ushort machine = reader.ReadUInt16();
        if (machine is not (0x14c or 0x8664)) throw new InvalidDataException("仅支持 x86 / x64 CAN DLL。");
        return machine == 0x14c;
    }
}

internal sealed class CanBridge : IDisposable
{
    private readonly Process process;
    private readonly BinaryWriter writer;
    private readonly Stream reader;
    private readonly Task<string> errors;
    public CanBridge(string dll)
    {
        string bundledBridge = Path.Combine(AppContext.BaseDirectory, "bridge", "x86", "CanBridge32.exe");
        var start = new ProcessStartInfo(File.Exists(bundledBridge) ? bundledBridge : Path.Combine(AppContext.BaseDirectory, "CanBridge32.exe"))
        { UseShellExecute = false, CreateNoWindow = true, RedirectStandardInput = true, RedirectStandardOutput = true, RedirectStandardError = true };
        start.ArgumentList.Add("--serve"); start.ArgumentList.Add(Path.GetFullPath(dll));
        process = Process.Start(start) ?? throw new IOException("无法启动 32 位 CAN 驱动桥接程序。");
        writer = new BinaryWriter(process.StandardInput.BaseStream); reader = process.StandardOutput.BaseStream;
        errors = process.StandardError.ReadToEndAsync();
        try { if (ReadUInt() != 0x43414e31) throw new IOException("CAN 桥接握手错误。"); }
        catch { Dispose(); throw; }
    }
    private byte[] Read(int count)
    {
        byte[] bytes = new byte[count];
        using var timeout = new CancellationTokenSource(TimeSpan.FromSeconds(5));
        try { reader.ReadExactlyAsync(bytes, timeout.Token).AsTask().GetAwaiter().GetResult(); return bytes; }
        catch (Exception ex) { throw new IOException("CAN 驱动桥接无响应或加载失败。" + (errors.IsCompletedSuccessfully ? errors.Result : ""), ex); }
    }
    private uint ReadUInt() => System.Buffers.Binary.BinaryPrimitives.ReadUInt32LittleEndian(Read(4));
    public uint Open(uint type, uint index, uint channel, byte t0, byte t1)
    { writer.Write((byte)1); writer.Write(type); writer.Write(index); writer.Write(channel); writer.Write(t0); writer.Write(t1); writer.Flush(); return ReadUInt(); }
    public uint Transmit(IntPtr memory)
    { byte[] frame = new byte[24]; Marshal.Copy(memory, frame, 0, 24); writer.Write((byte)2); writer.Write(frame); writer.Flush(); return ReadUInt(); }
    public uint Receive(IntPtr memory)
    { writer.Write((byte)3); writer.Flush(); uint count = ReadUInt(); if (count <= 64) { byte[] frames = Read((int)count * 24); Marshal.Copy(frames, 0, memory, frames.Length); } return count; }
    public void Dispose()
    {
        try { writer.Dispose(); if (!process.WaitForExit(1500)) process.Kill(); }
        catch (InvalidOperationException) { }
        finally { reader.Dispose(); process.Dispose(); }
    }
}
