using System.Diagnostics;
using System.IO;
using Updater.Core;

namespace Updater.Desktop;

public partial class MainWindow
{
    private CommunicationTrace? communicationTrace;
    private string diagnosticAction = "";

    private void BeginCommunicationTrace(string action)
    {
        diagnosticAction = action;
        var device = CanModel.SelectedItem as DetectedCanDevice;
        communicationTrace = CommunicationTrace.Create(Path.Combine(AppContext.BaseDirectory, "Logs"), action, new
        {
            version = "V2026.10.25.1", action, chip = ChipName, simulation,
            transport = IsCan ? "CAN" : "SCI", baud = BaudRate.SelectedItem,
            port = IsCan ? null : ComPort.SelectedItem,
            brand = device?.Brand, deviceType = device?.Type, deviceIndex = device?.Index,
            serial = device?.Serial, channel = CanChannel.SelectedItem, node = NodeId.Text,
            driver = IsCan ? device?.DriverPath ?? DllPath.Text : null,
            imagePath = loadedPath, imageBytes = Is335 ? avpImage?.Bytes.Length : image?.Bytes.Length,
            appStart = AppStartAddress.Text, processBits = IntPtr.Size * 8
        });
        if (communicationTrace.FilePath is not null) Write($"后台通讯日志：{communicationTrace.FilePath}");
        else Write($"后台通讯日志不可用，本次操作继续：{communicationTrace.Error}");
    }

    private async Task EndCommunicationTrace()
    {
        var trace = communicationTrace;
        if (trace is null) return;
        trace.Record("operation_result", new { action = diagnosticAction, status = StatusLabel.Text, percent = diagnosticAction == "upgrade" ? (double?)Progress.Value : null, committed = diagnosticAction == "upgrade" ? (bool?)committed : null });
        await trace.DisposeAsync();
        communicationTrace = null;
        if (trace.FilePath is not null)
        {
            try { await Task.Run(() => CommunicationReport.Save(trace.FilePath)); }
            catch (Exception ex) { Write($"后台通讯摘要生成失败：{ex.Message}"); }
        }
        if (trace.Error is not null) Write($"后台通讯日志写入异常：{trace.Error}");
    }

    private ITransport TraceTransport(ITransport transport) =>
        communicationTrace is null ? transport : new DiagnosticTransport(transport, communicationTrace);
}

// One record per protocol exchange, including direct INFO queries and SCI.
// No file access, extra waits or packet changes in the communication thread.
internal sealed class DiagnosticTransport(ITransport inner, CommunicationTrace trace) : ITransport
{
    public async Task<Packet> ExchangeAsync(Packet request, TimeSpan timeout, CancellationToken cancellation)
    {
        var watch = Stopwatch.StartNew();
        trace.Record("transport_exchange_start", new { command = (Command)request.Command, request.Session, request.Sequence, payloadBytes = request.Payload.Length, timeout_ms = timeout.TotalMilliseconds });
        try
        {
            var reply = await inner.ExchangeAsync(request, timeout, cancellation);
            trace.Record("transport_exchange_reply", new {
                command = (Command)request.Command, request.Session, request.Sequence,
                elapsed_ms = watch.Elapsed.TotalMilliseconds, replyCommand = reply.Command,
                replySession = reply.Session, replySequence = reply.Sequence,
                payloadBytes = reply.Payload.Length, status = reply.Payload.Length > 0 ? (int?)reply.Payload[0] : null,
                state = reply.Payload.Length > 1 ? (int?)reply.Payload[1] : null,
                confirmedAddress = reply.Payload.Length == 6 ? (uint?)System.Buffers.Binary.BinaryPrimitives.ReadUInt32LittleEndian(reply.Payload.AsSpan(2)) : null
            });
            return reply;
        }
        catch (Exception ex)
        {
            trace.Record("transport_exchange_error", new { command = (Command)request.Command, request.Session, request.Sequence, elapsed_ms = watch.Elapsed.TotalMilliseconds, errorType = ex.GetType().Name, error = ex.Message });
            throw;
        }
    }

    public ValueTask DisposeAsync() => inner.DisposeAsync();
}
