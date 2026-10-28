using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;
using Microsoft.Win32.SafeHandles;
using Updater.Core;

namespace Updater.Desktop;

internal static class ReplyMatch
{
    public static bool Is(Packet reply, Packet request) => reply.Command == (request.Command | 0x80) && reply.Session == request.Session && reply.Sequence == request.Sequence;
}

internal sealed class SerialConnection : IAsyncDisposable
{
    private readonly SafeFileHandle handle;
    public SerialConnection(string port, int baud)
    {
        if (!Regex.IsMatch(port, "^COM[1-9][0-9]{0,3}$", RegexOptions.IgnoreCase)) throw new ArgumentException("串口格式应为 COM3、COM12 等。");
        if (baud is not (9600 or 19200 or 38400 or 57600 or 115200 or 230400 or 460800)) throw new ArgumentOutOfRangeException(nameof(baud));
        handle = CreateFile("\\\\.\\" + port, 0xC0000000, 0, IntPtr.Zero, 3, 0, IntPtr.Zero);
        if (handle.IsInvalid) { int error = Marshal.GetLastWin32Error(); handle.Dispose(); throw new Win32Exception(error, "无法打开串口。"); }
        try
        {
            var dcb = new Dcb { Length = (uint)Marshal.SizeOf<Dcb>() };
            Check(GetCommState(handle, ref dcb));
            dcb.BaudRate = (uint)baud; dcb.Flags = 1; // binary, no hardware/software flow control, DTR/RTS off
            dcb.ByteSize = 8; dcb.Parity = 0; dcb.StopBits = 0;
            Check(SetCommState(handle, ref dcb));
            var timeouts = new CommTimeouts { ReadInterval = uint.MaxValue, ReadConstant = 50, WriteConstant = 1000 };
            Check(SetCommTimeouts(handle, ref timeouts));
            Check(PurgeComm(handle, 0x000F));
        }
        catch { handle.Dispose(); throw; }
    }
    public void Write(byte[] data)
    {
        Check(WriteFile(handle, data, (uint)data.Length, out uint written, IntPtr.Zero));
        if (written != data.Length) throw new IOException("串口数据未完整发送。");
    }
    public uint Read(byte[] buffer)
    {
        Check(ReadFile(handle, buffer, (uint)buffer.Length, out uint count, IntPtr.Zero));
        return count;
    }
    internal void SetReadWait(uint milliseconds)
    {
        var timeouts = new CommTimeouts { ReadInterval = uint.MaxValue, ReadConstant = milliseconds, WriteConstant = 1000 };
        Check(SetCommTimeouts(handle, ref timeouts));
    }
    public ValueTask DisposeAsync() { handle.Dispose(); return ValueTask.CompletedTask; }
    private static void Check(bool result) { if (!result) throw new Win32Exception(Marshal.GetLastWin32Error()); }
    [StructLayout(LayoutKind.Sequential)] private struct Dcb
    {
        public uint Length, BaudRate, Flags;
        public ushort Reserved, XonLimit, XoffLimit;
        public byte ByteSize, Parity, StopBits;
        public sbyte Xon, Xoff, Error, Eof, Event;
        public ushort Reserved1;
    }
    [StructLayout(LayoutKind.Sequential)] private struct CommTimeouts { public uint ReadInterval, ReadMultiplier, ReadConstant, WriteMultiplier, WriteConstant; }
    [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true, EntryPoint = "CreateFileW")]
    private static extern SafeFileHandle CreateFile(string name, uint access, uint share, IntPtr security, uint creation, uint flags, IntPtr template);
    [DllImport("kernel32.dll", SetLastError = true)] [return: MarshalAs(UnmanagedType.Bool)] private static extern bool GetCommState(SafeFileHandle h, ref Dcb dcb);
    [DllImport("kernel32.dll", SetLastError = true)] [return: MarshalAs(UnmanagedType.Bool)] private static extern bool SetCommState(SafeFileHandle h, ref Dcb dcb);
    [DllImport("kernel32.dll", SetLastError = true)] [return: MarshalAs(UnmanagedType.Bool)] private static extern bool SetCommTimeouts(SafeFileHandle h, ref CommTimeouts timeouts);
    [DllImport("kernel32.dll", SetLastError = true)] [return: MarshalAs(UnmanagedType.Bool)] private static extern bool PurgeComm(SafeFileHandle h, uint flags);
    [DllImport("kernel32.dll", SetLastError = true)] [return: MarshalAs(UnmanagedType.Bool)] private static extern bool ReadFile(SafeFileHandle h, [Out] byte[] buffer, uint length, out uint read, IntPtr overlapped);
    [DllImport("kernel32.dll", SetLastError = true)] [return: MarshalAs(UnmanagedType.Bool)] private static extern bool WriteFile(SafeFileHandle h, byte[] buffer, uint length, out uint written, IntPtr overlapped);
}

public sealed class SerialTransport : ITransport
{
    private readonly SerialConnection connection;
    public SerialTransport(string port, int baud) => connection = new(port, baud);
    public Task<Packet> ExchangeAsync(Packet request, TimeSpan timeout, CancellationToken cancellation) => Task.Run(() =>
    {
        cancellation.ThrowIfCancellationRequested();
        byte[] data = request.Encode();
        connection.Write(data);
        var watch = Stopwatch.StartNew();
        var parser = new SciParser();
        byte[] buffer = new byte[256];
        long receivedBytes = 0;
        int decodedPackets = 0;
        var sample = new List<byte>(96);
        var lastByte = watch.Elapsed;
        while (watch.Elapsed < timeout)
        {
            cancellation.ThrowIfCancellationRequested();
            uint count = connection.Read(buffer);
            receivedBytes += count;
            for (int i = 0; i < count && sample.Count < 96; i++) sample.Add(buffer[i]);
            if (watch.Elapsed - lastByte > TimeSpan.FromMilliseconds(200)) parser.Reset();
            if (count > 0) lastByte = watch.Elapsed;
            for (int i = 0; i < count; i++)
                foreach (var reply in parser.Feed(buffer[i]))
                {
                    ++decodedPackets;
                    if (ReplyMatch.Is(reply, request)) return reply;
                }
        }
        string hint = receivedBytes == 0
            ? "电脑未收到串口数据；请检查 DSP TX 到转换器 RX 的回传链路。"
            : decodedPackets == 0
                ? "收到数据但未解析出有效协议帧；请检查原始数据、波特率和帧 CRC。"
                : "收到有效协议帧，但命令、会话或序号与本次请求不匹配。";
        throw new TimeoutException($"串口应答超时。TX={Convert.ToHexString(data)}；RX字节数={receivedBytes}；有效帧={decodedPackets}；RX前96字节={(sample.Count == 0 ? "无" : Convert.ToHexString(sample.ToArray()))}。{hint}");
    }, cancellation);
    public ValueTask DisposeAsync() => connection.DisposeAsync();
}

public sealed class ControlCanTransport : ITransport
{
    [StructLayout(LayoutKind.Sequential)] private struct InitConfig { public uint AccCode, AccMask, Reserved; public byte Filter, Timing0, Timing1, Mode; }
    [StructLayout(LayoutKind.Sequential)] private struct CanFrame
    {
        public uint Id, Timestamp;
        public byte TimeFlag, SendType, RemoteFlag, ExternFlag, DataLength;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)] public byte[] Data;
        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 3)] public byte[] Reserved;
    }
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] private delegate uint OpenFn(uint type, uint index, uint reserved);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] private delegate uint CloseFn(uint type, uint index);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] private delegate uint InitFn(uint type, uint index, uint channel, ref InitConfig config);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] private delegate uint ChannelFn(uint type, uint index, uint channel);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] private delegate uint TransmitFn(uint type, uint index, uint channel, IntPtr frames, uint count);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] private delegate uint ReceiveFn(uint type, uint index, uint channel, IntPtr frames, uint count, int wait);
    private readonly IntPtr library;
    private readonly CanBridge? bridge;
    private readonly CloseFn close;
    private readonly ChannelFn reset;
    private readonly TransmitFn transmit;
    private readonly ReceiveFn receive;
    private readonly uint type, index, channel, txId, rxId;
    private byte transfer;
    private bool disposed;
    public ControlCanTransport(string dllPath, uint deviceType, uint deviceIndex, uint channelIndex, int baud, uint node)
    {
        if (deviceType is not (3 or 4) || deviceIndex > 15 || channelIndex > (deviceType == 3 ? 0u : 1u) || node is < 1 or > 127)
            throw new ArgumentException("USBCAN-I 只支持通道 0；USBCAN-II 支持通道 0/1；节点地址为 1–127。");
        (int t0, int t1) = baud switch {
            10000 => (0x31, 0x1c), 20000 => (0x18, 0x1c), 50000 => (0x09, 0x1c), 100000 => (0x04, 0x1c),
            125000 => (0x03, 0x1c), 250000 => (0x01, 0x1c), 500000 => (0x00, 0x1c), 800000 => (0x00, 0x16), 1000000 => (0x00, 0x14),
            _ => throw new ArgumentOutOfRangeException(nameof(baud))
        };
        type = deviceType; index = deviceIndex; channel = channelIndex; txId = 0x600 + node; rxId = 0x580 + node;
        if (Marshal.SizeOf<CanFrame>() != 24 || Marshal.SizeOf<InitConfig>() != 16) throw new InvalidOperationException("ControlCAN 结构布局错误。");
        if (IntPtr.Size == 8 && CanDriver.IsX86(dllPath))
        {
            bridge = new CanBridge(dllPath);
            try { Check(bridge.Open(type, index, channel, (byte)t0, (byte)t1), "打开并初始化 CAN 盒"); }
            catch { bridge.Dispose(); throw; }
            close = (_, _) => 1;
            reset = (_, _, _) => 1;
            transmit = (_, _, _, memory, _) => bridge.Transmit(memory);
            receive = (_, _, _, memory, _, _) => bridge.Receive(memory);
            return;
        }
        library = NativeLibrary.Load(Path.GetFullPath(dllPath));
        bool opened = false;
        try
        {
            var open = Bind<OpenFn>("VCI_OpenDevice"); close = Bind<CloseFn>("VCI_CloseDevice");
            var init = Bind<InitFn>("VCI_InitCAN"); var start = Bind<ChannelFn>("VCI_StartCAN");
            reset = Bind<ChannelFn>("VCI_ResetCAN"); transmit = Bind<TransmitFn>("VCI_Transmit"); receive = Bind<ReceiveFn>("VCI_Receive");
            Check(open(type, index, 0), "打开 CAN 盒"); opened = true;
            var config = new InitConfig { AccMask = uint.MaxValue, Filter = 1, Timing0 = (byte)t0, Timing1 = (byte)t1, Mode = 0 };
            Check(init(type, index, channel, ref config), "配置 CAN 通道");
            Check(start(type, index, channel), "启动 CAN 通道");
        }
        catch { if (opened) Bind<CloseFn>("VCI_CloseDevice")(type, index); NativeLibrary.Free(library); throw; }
    }
    private T Bind<T>(string name) where T : Delegate => Marshal.GetDelegateForFunctionPointer<T>(NativeLibrary.GetExport(library, name));
    private static void Check(uint value, string action) { if (value != 1) throw new IOException($"{action}失败（ControlCAN 返回 {value}）。"); }
    public Task<Packet> ExchangeAsync(Packet request, TimeSpan timeout, CancellationToken cancellation) => Task.Run(() =>
    {
        ObjectDisposedException.ThrowIf(disposed, this);
        const int frameSize = 24;
        IntPtr memory = Marshal.AllocHGlobal(frameSize * 64);
        try
        {
            foreach (byte[] fragment in CanFraming.Fragment(request.Encode(), transfer++))
            {
                cancellation.ThrowIfCancellationRequested();
                var frame = new CanFrame { Id = txId, SendType = 1, DataLength = (byte)fragment.Length, Data = new byte[8], Reserved = new byte[3] };
                fragment.CopyTo(frame.Data, 0); Marshal.StructureToPtr(frame, memory, false);
                Check(transmit(type, index, channel, memory, 1), "发送 CAN 帧");
                // A single receive mailbox on the target: pace frames to avoid overwrite.
                if (cancellation.WaitHandle.WaitOne(3)) cancellation.ThrowIfCancellationRequested();
            }
            var decoder = new CanReassembler(); var watch = Stopwatch.StartNew(); var lastFrame = watch.Elapsed;
            while (watch.Elapsed < timeout)
            {
                cancellation.ThrowIfCancellationRequested();
                uint count = receive(type, index, channel, memory, 64, 50);
                if (count == uint.MaxValue || count > 64) throw new IOException("CAN 接收失败，检查 USB 连接和驱动。");
                if (watch.Elapsed - lastFrame > TimeSpan.FromMilliseconds(200)) decoder.Reset();
                for (int i = 0; i < count; i++)
                {
                    var frame = Marshal.PtrToStructure<CanFrame>(memory + i * frameSize);
                    if (frame.Id != rxId || frame.ExternFlag != 0 || frame.RemoteFlag != 0 || frame.DataLength is < 3 or > 8) continue;
                    lastFrame = watch.Elapsed;
                    var reply = decoder.Feed(frame.Data.AsSpan(0, frame.DataLength));
                    if (reply is not null && ReplyMatch.Is(reply, request)) return reply;
                }
                if (count == 0 && cancellation.WaitHandle.WaitOne(2)) cancellation.ThrowIfCancellationRequested();
            }
            throw new TimeoutException("CAN 应答超时，请检查节点号、波特率、终端电阻和 Bootloader 状态。");
        }
        finally { Marshal.FreeHGlobal(memory); }
    }, cancellation);
    public ValueTask DisposeAsync()
    {
        if (!disposed) { disposed = true; if (bridge is not null) bridge.Dispose(); else { try { reset(type, index, channel); close(type, index); } finally { NativeLibrary.Free(library); } } }
        return ValueTask.CompletedTask;
    }
}
