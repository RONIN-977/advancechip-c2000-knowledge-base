using System;
using System.IO;
using System.Runtime.InteropServices;

namespace Updater.Discovery
{
    // Separate discovery process: no InitCAN, StartCAN, ResetCAN or transmit calls.
    internal static class CanDiscovery
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
        [UnmanagedFunctionPointer(CallingConvention.StdCall)] delegate uint Open(uint type, uint index, uint reserved);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)] delegate uint Close(uint type, uint index);
        [UnmanagedFunctionPointer(CallingConvention.StdCall)] delegate uint ReadBoard(uint type, uint index, IntPtr info);
        static T Bind<T>(IntPtr library, string name) where T : class
        {
            IntPtr address = GetProcAddress(library, name);
            if (address == IntPtr.Zero) throw new EntryPointNotFoundException(name);
            return (T)(object)Marshal.GetDelegateForFunctionPointer(address, typeof(T));
        }
        public static int Run(string path)
        {
            IntPtr library = IntPtr.Zero, info = IntPtr.Zero, activation = IntPtr.Zero, cookie = IntPtr.Zero;
            try
            {
                path = Path.GetFullPath(path);
                string manifest = Path.Combine(Path.GetDirectoryName(path) ?? "", "runtime.manifest");
                if (File.Exists(manifest))
                {
                    var context = new ActivationContext { Size = (uint)Marshal.SizeOf(typeof(ActivationContext)), Source = manifest };
                    activation = CreateActCtx(ref context);
                    if (activation == new IntPtr(-1)) { activation = IntPtr.Zero; throw new System.ComponentModel.Win32Exception(Marshal.GetLastWin32Error()); }
                    if (!ActivateActCtx(activation, out cookie)) throw new System.ComponentModel.Win32Exception(Marshal.GetLastWin32Error());
                }
                library = LoadLibraryEx(path, IntPtr.Zero, 8);
                if (library == IntPtr.Zero) throw new System.ComponentModel.Win32Exception(Marshal.GetLastWin32Error());
                Open open = Bind<Open>(library, "VCI_OpenDevice");
                Close close = Bind<Close>(library, "VCI_CloseDevice");
                ReadBoard read = Bind<ReadBoard>(library, "VCI_ReadBoardInfo");
                // VCI_BOARD_INFO: five ushorts, can_Num, serial[20], hw_type[40], reserved[4].
                info = Marshal.AllocHGlobal(80);
                foreach (uint type in new uint[] { 3, 4 })
                {
                    for (uint index = 0; index < 16; index++)
                    {
                        if (open(type, index, 0) != 1) continue;
                        try
                        {
                            byte[] bytes = new byte[80]; Marshal.Copy(bytes, 0, info, bytes.Length);
                            if (read(type, index, info) != 1) continue;
                            Marshal.Copy(info, bytes, 0, bytes.Length);
                            byte channels = bytes[10];
                            if (channels != (type == 3 ? 1 : 2)) continue;
                            Console.WriteLine(type + "\t" + index + "\t" + channels + "\t" + Convert.ToBase64String(bytes, 11, 20));
                        }
                        finally { close(type, index); }
                    }
                }
                return 0;
            }
            catch (Exception ex) { Console.Error.WriteLine(ex.Message); return 1; }
            finally
            {
                if (info != IntPtr.Zero) Marshal.FreeHGlobal(info);
                if (library != IntPtr.Zero) FreeLibrary(library);
                if (cookie != IntPtr.Zero) DeactivateActCtx(0, cookie);
                if (activation != IntPtr.Zero) ReleaseActCtx(activation);
            }
        }
    }
}
