using System;
using System.IO;
using System.Runtime.InteropServices;

// .NET Framework x86 helper: binary stdio, no device access during --check.
internal static class Program
{
    [DllImport("kernel32", CharSet = CharSet.Unicode, SetLastError = true)] static extern IntPtr LoadLibraryEx(string path, IntPtr file, uint flags);
    [DllImport("kernel32", CharSet = CharSet.Ansi)] static extern IntPtr GetProcAddress(IntPtr module, string name);
    [DllImport("kernel32")] static extern bool FreeLibrary(IntPtr module);
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)] struct ActivationContext
    {
        public uint Size, Flags;
        public string Source;
        public ushort ProcessorArchitecture, LanguageId;
        public string AssemblyDirectory;
        public IntPtr ResourceName;
        public string ApplicationName;
        public IntPtr Module;
    }
    [DllImport("kernel32", CharSet = CharSet.Unicode, SetLastError = true)] static extern IntPtr CreateActCtx(ref ActivationContext context);
    [DllImport("kernel32", SetLastError = true)] static extern bool ActivateActCtx(IntPtr context, out IntPtr cookie);
    [DllImport("kernel32")] static extern bool DeactivateActCtx(uint flags, IntPtr cookie);
    [DllImport("kernel32")] static extern void ReleaseActCtx(IntPtr context);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] delegate uint Open(uint t, uint i, uint r);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] delegate uint Close(uint t, uint i);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] delegate uint Channel(uint t, uint i, uint c);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] delegate uint Init(uint t, uint i, uint c, IntPtr config);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] delegate uint Tx(uint t, uint i, uint c, IntPtr frames, uint count);
    [UnmanagedFunctionPointer(CallingConvention.StdCall)] delegate uint Rx(uint t, uint i, uint c, IntPtr frames, uint count, int wait);
    static T Bind<T>(IntPtr lib, string name) where T : class
    {
        IntPtr address = GetProcAddress(lib, name);
        if (address == IntPtr.Zero) throw new EntryPointNotFoundException(name);
        return (T)(object)Marshal.GetDelegateForFunctionPointer(address, typeof(T));
    }
    static int Main(string[] args)
    {
        IntPtr lib = IntPtr.Zero, memory = IntPtr.Zero;
        IntPtr activation = IntPtr.Zero, cookie = IntPtr.Zero;
        Close close = null; Channel reset = null;
        bool opened = false, initialized = false;
        uint type = 0, index = 0, channel = 0;
        try
        {
            if (args.Length != 2) throw new ArgumentException("Expected --check/--serve and DLL path");
            string manifest = Path.Combine(Path.GetDirectoryName(Path.GetFullPath(args[1])), "runtime.manifest");
            if (File.Exists(manifest))
            {
                var context = new ActivationContext { Size = (uint)Marshal.SizeOf(typeof(ActivationContext)), Source = manifest };
                activation = CreateActCtx(ref context);
                if (activation == new IntPtr(-1)) { activation = IntPtr.Zero; throw new System.ComponentModel.Win32Exception(Marshal.GetLastWin32Error()); }
                if (!ActivateActCtx(activation, out cookie)) throw new System.ComponentModel.Win32Exception(Marshal.GetLastWin32Error());
            }
            lib = LoadLibraryEx(Path.GetFullPath(args[1]), IntPtr.Zero, 8);
            if (lib == IntPtr.Zero) throw new System.ComponentModel.Win32Exception(Marshal.GetLastWin32Error());
            Open open = Bind<Open>(lib, "VCI_OpenDevice"); close = Bind<Close>(lib, "VCI_CloseDevice");
            Init init = Bind<Init>(lib, "VCI_InitCAN"); Channel start = Bind<Channel>(lib, "VCI_StartCAN");
            reset = Bind<Channel>(lib, "VCI_ResetCAN"); Tx tx = Bind<Tx>(lib, "VCI_Transmit"); Rx rx = Bind<Rx>(lib, "VCI_Receive");
            if (args[0] == "--check")
            {
                Console.WriteLine("OK: x86 DLL loaded; all 7 VCI exports resolved; no device opened.");
                Console.WriteLine("Runtime: " + typeof(object).Assembly.Location);
                foreach (System.Diagnostics.ProcessModule module in System.Diagnostics.Process.GetCurrentProcess().Modules)
                    if (module.ModuleName.StartsWith("msvc", StringComparison.OrdinalIgnoreCase) || module.ModuleName.StartsWith("mfc", StringComparison.OrdinalIgnoreCase) || module.ModuleName.Equals("usbcan.dll", StringComparison.OrdinalIgnoreCase))
                        Console.WriteLine(module.FileName);
                return 0;
            }
            if (args[0] != "--serve") throw new ArgumentException("Unknown mode");
            memory = Marshal.AllocHGlobal(24 * 64);
            var reader = new BinaryReader(Console.OpenStandardInput());
            var writer = new BinaryWriter(Console.OpenStandardOutput());
            writer.Write(0x43414E31u); writer.Flush();
            while (true)
            {
                int op;
                try { op = reader.ReadByte(); } catch (EndOfStreamException) { break; }
                uint result;
                if (op == 1)
                {
                    if (opened) throw new InvalidOperationException("Already opened");
                    type = reader.ReadUInt32(); index = reader.ReadUInt32(); channel = reader.ReadUInt32();
                    byte t0 = reader.ReadByte(), t1 = reader.ReadByte();
                    if ((type != 3 && type != 4) || index > 15 || channel > (type == 3 ? 0u : 1u)) throw new InvalidDataException("Invalid device");
                    result = open(type, index, 0); opened = result == 1;
                    if (opened)
                    {
                        byte[] config = new byte[16]; for (int i = 4; i < 8; i++) config[i] = 255;
                        config[12] = 1; config[13] = t0; config[14] = t1;
                        Marshal.Copy(config, 0, memory, 16); result = init(type, index, channel, memory);
                        initialized = result == 1;
                        if (initialized) result = start(type, index, channel);
                    }
                    writer.Write(result);
                }
                else if (op == 2 && opened)
                {
                    byte[] frame = reader.ReadBytes(24); if (frame.Length != 24) throw new EndOfStreamException();
                    Marshal.Copy(frame, 0, memory, 24); writer.Write(tx(type, index, channel, memory, 1));
                }
                else if (op == 3 && opened)
                {
                    uint count = rx(type, index, channel, memory, 64, 50); writer.Write(count);
                    if (count <= 64) { byte[] frames = new byte[count * 24]; Marshal.Copy(memory, frames, 0, frames.Length); writer.Write(frames); }
                }
                else if (op == 4) { writer.Write(1u); writer.Flush(); break; }
                else throw new InvalidDataException("Invalid operation");
                writer.Flush();
            }
            return 0;
        }
        catch (Exception ex) { Console.Error.WriteLine(ex.Message); return 1; }
        finally
        {
            if (opened) { if (initialized) reset(type, index, channel); close(type, index); }
            if (memory != IntPtr.Zero) Marshal.FreeHGlobal(memory);
            if (lib != IntPtr.Zero) FreeLibrary(lib);
            if (cookie != IntPtr.Zero) DeactivateActCtx(0, cookie);
            if (activation != IntPtr.Zero) ReleaseActCtx(activation);
        }
    }
}
