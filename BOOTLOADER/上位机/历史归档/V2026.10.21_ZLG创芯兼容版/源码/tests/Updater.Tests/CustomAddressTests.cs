using System.Buffers.Binary;
using System.Text;
using Updater.Core;

internal static class CustomAddressTests
{
    public static async Task Run()
    {
        int passed = 0;
        void Check(bool value, string message) { if (!value) throw new Exception(message); passed++; }
        void Reject(Action action) { try { action(); } catch (InvalidDataException) { passed++; return; } throw new Exception("Invalid address/image accepted"); }
        foreach (var standard in new[] { DeviceProfile.F280039C, DeviceProfile.F280049, DeviceProfile.F280035, DeviceProfile.F2800157 })
        {
            foreach (uint start in new[] { standard.AppStart + 0x1000U, standard.AppStart == 0x3e8000 ? standard.AppStart : standard.AppStart - 0x400U })
            {
                var target = standard.WithAppStart(start);
                var image = FirmwareImage.Demo(target) with { Words = new ushort[64] };
                image.Validate(target);
                Check(BinaryPrimitives.ReadUInt32LittleEndian(image.Manifest().AsSpan(12)) == start, "Manifest start");
                Check(BinaryPrimitives.ReadUInt32LittleEndian(image.Manifest().AsSpan(20)) == start, "Manifest entry");
                Reject(() => image.Validate(standard.WithAppStart(start+4)));
                Reject(() => (image with { EntryWordAddress = start+4 }).Validate());
                Reject(() => (image with { Words = new ushort[target.AppEnd-start+4] }).Validate());
                var bytes = new List<byte>();
                void W(ushort v) { bytes.Add((byte)v); bytes.Add((byte)(v>>8)); }
                void A(uint v) { W((ushort)(v>>16)); W((ushort)v); }
                W(0x08aa); for(int i=0;i<8;i++)W(0); A(start); W(4); A(start);
                W(0x76);W(0);W(1);W(2);W(0);
                string table = string.Join(" ", bytes.Select(b=>b.ToString("X2")));
                var parsed = TiBootTable.Parse(table, target:target);
                Check(parsed.Target==target && parsed.StartWordAddress==start && parsed.Words.Length==4, "SCI8 custom start");
                Reject(()=>TiBootTable.Parse(table, target:standard.WithAppStart(start+4)));
                string path=Path.GetTempFileName();
                string json=Path.ChangeExtension(path,"json");
                try { parsed.Save(json); Check(FirmwareImage.Load(json,target).Target==target,"JSON selected target"); Reject(()=>FirmwareImage.Load(json,standard.WithAppStart(start+4))); }
                finally { File.Delete(path); File.Delete(json); }
                foreach(bool can in new[]{false,true})
                {
                    await using var wire=new SimulatedTransport(can,true,0,target);
                    await new UpdaterClient(wire).UpdateAsync(image,null,CancellationToken.None);
                    Check(wire.Device.State==BootState.Valid && wire.Device.EraseCount==1 && wire.Device.CommitCount==1,"Custom SCI/CAN lost ACK update");
                    await using var wrong=new SimulatedTransport(can,false,0,standard.WithAppStart(start+4));
                    try { await new UpdaterClient(wrong).UpdateAsync(image,null,CancellationToken.None); throw new Exception("Mismatch accepted"); }
                    catch(InvalidDataException) { Check(wrong.Device.EraseCount==0,"Mismatch must not erase"); }
                }
            }
            Reject(()=>standard.WithAppStart(0)); Reject(()=>standard.WithAppStart(uint.MaxValue));
            Reject(()=>standard.WithAppStart(standard.AppEnd)); Reject(()=>standard.WithAppStart(standard.AppStart+1));
            (FirmwareImage.Demo(standard) with { Words=new ushort[4] }).Validate();
            Check(true,"Legacy default");
        }
        foreach(uint start in new[]{0x300000U,0x310000U})
        {
            byte[] file=new byte[158];
            void W16(int p,ushort v)=>BinaryPrimitives.WriteUInt16LittleEndian(file.AsSpan(p),v);
            void W32(int p,uint v)=>BinaryPrimitives.WriteUInt32LittleEndian(file.AsSpan(p),v);
            W16(0,0xc2);W16(2,2);W16(16,28);W16(18,2);W16(20,0x9d);
            Encoding.ASCII.GetBytes("codestart").AsSpan(0,8).CopyTo(file.AsSpan(50));
            // codestart is a long COFF name, put it in the string table.
            W32(50,0);W32(54,4);W32(8,152);
            Array.Resize(ref file,166);W32(152,14);Encoding.ASCII.GetBytes("codestart\0").CopyTo(file,156);
            W32(62,start);W32(66,2);W32(70,146);W32(90,0x20);
            Encoding.ASCII.GetBytes("BOOTFLAG").CopyTo(file,98);W32(110,Avp335Profile.FlagAddress);W32(114,1);W32(118,150);W32(138,0x20);W16(150,0xaa);
            var fw=Avp335OutLoader.Parse(file,start);
            Check(fw.StartWordAddress==start && fw.Bytes.Length==4,"335 COFF custom start");
            Reject(()=>Avp335OutLoader.Parse(file,start+4));
            await using var transport=new Avp335SimulatedTransport(false);
            await new Avp335UpdaterClient(transport).UpdateAsync(fw,null,CancellationToken.None);
            Check(transport.CommitCount==1,"335 sequential protocol unchanged");
        }
        Reject(()=>Avp335Profile.ValidateStart(0));Reject(()=>Avp335Profile.ValidateStart(0x337ffe));
        Console.WriteLine($"PASS {passed} custom-address checks: parsing, manifests, SCI/CAN, JSON, defaults, bounds, no erase on mismatch, 335 COFF.");
    }
}
