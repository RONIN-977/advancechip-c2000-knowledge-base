using System.Runtime.InteropServices;
using System.Buffers.Binary;
using System.Text.Json;
using Updater.Core;

if(args.Length!=5)throw new ArgumentException("dll OUT hex2000 scenario result.json");
void Check(bool value,string why){if(!value)throw new Exception(why);}
using var model=new Model(args[0]);
var profile=DeviceProfile.F280039C.WithAppStart(0x087000);
var image=await TiOutConverter.LoadAsync(args[1],args[2],target:profile);
Check(image.StartWordAddress==0x087000&&image.EntryWordAddress==0x087000,"APP entry");
model.Reset();
await using var wire=new Wire(model);
var client=new UpdaterClient(wire);
string scenario=args[3];
if(scenario=="app_ack_timeout"){
    model.Fault(5);bool timeout=false;
    try{await client.EnterBootAsync(CancellationToken.None);}catch(TimeoutException){timeout=true;}
    Check(timeout&&model.Stat(0)==0&&model.Stat(5)==0&&model.Stat(11)==0,"No reset/hook before complete ACK");
}else{
    await client.EnterBootAsync(CancellationToken.None);
    Check(model.Stat(0)==1&&model.Stat(5)==1&&model.Stat(11)==1,"Real adapter ENTER/callback/reset");
    Check(model.Stat(13)==0x084000&&model.Stat(14)==0x087000,"Pair partition");
    if(scenario.StartsWith("flash_fault_")){
        model.Fault(ushort.Parse(scenario[12..]));bool failed=false;
        try{await client.UpdateAsync(image,null,CancellationToken.None);}catch(IOException){failed=true;}
        Check(failed&&model.Stat(3)==0&&model.Stat(4)==0,"Flash model failure must not commit");
    }else if(scenario=="wrong_partition"){
        bool failed=false;
        try{await client.UpdateAsync(FirmwareImage.Demo(),null,CancellationToken.None);}catch(InvalidDataException){failed=true;}
        Check(failed&&model.Stat(1)==0,"Default 090000 host partition rejected before erase");
    }else{
        if(scenario=="lost_ack")wire.Drop.UnionWith([Command.Begin,Command.Data,Command.Finish]);
        if(scenario=="cancel_recover"){
            using var cancel=new CancellationTokenSource();bool failed=false;
            try{await client.UpdateAsync(image,new ProgressNow(p=>{if(p.Percent>0)cancel.Cancel();}),cancel.Token);}catch(OperationCanceledException){failed=true;}
            Check(failed&&model.Stat(3)==0&&model.Stat(4)==0,"Cancellation leaves invalid image");
        }
        int percent=0;
        await client.UpdateAsync(image,new ProgressNow(p=>percent=p.Percent),CancellationToken.None);
        Check(percent==100&&model.Stat(3)==1&&model.Stat(4)==1,"Commit");
        Check(model.Stat(6)==image.Words.Length&&model.Stat(7)==Crc32.Compute(image.Bytes),"Image word count/CRC");
        for(int i=0;i<image.Words.Length;i++)Check(model.Word(image.StartWordAddress+(uint)i)==image.Words[i],"WORD "+i);
        if(scenario!="cancel_recover")Check(model.Stat(1)==1&&model.Stat(2)==(image.Words.Length+59)/60,"No duplicate erase/program");
        if(scenario=="lost_ack")Check(wire.Dropped==3,"Retry injections");
        await client.ResetAsync(CancellationToken.None);Check(model.Stat(0)==0&&model.Stat(5)==2,"RESET after commit enters APP model");
    }
}
var result=new{status="PASS",scenario,firmware=Path.GetFullPath(args[1]),start=$"0x{image.StartWordAddress:X6}",words=image.Words.Length,crc32=$"0x{Crc32.Compute(image.Bytes):X8}",erase=model.Stat(1),program=model.Stat(2),commit=model.Stat(3),reset=model.Stat(5),shutdownHook=model.Stat(11),canRx=model.Stat(8),canTx=model.Stat(9),firstCommands=wire.Commands.Take(4),lastCommands=wire.Commands.TakeLast(3),hardware="NOT RUN"};
File.WriteAllText(args[4],JsonSerializer.Serialize(result,new JsonSerializerOptions{WriteIndented=true}));
Console.WriteLine(JsonSerializer.Serialize(result));

sealed class ProgressNow(Action<UpdateProgress> f):IProgress<UpdateProgress>{public void Report(UpdateProgress p)=>f(p);}
sealed class Wire(Model model):ITransport{
    byte transfer;
    public HashSet<Command> Drop=[];public int Dropped;public List<string> Commands=[];
    public Task<Packet> ExchangeAsync(Packet request,TimeSpan timeout,CancellationToken cancellation){
        cancellation.ThrowIfCancellationRequested();
        Commands.Add(((Command)request.Command).ToString());
        var reassembler=new CanReassembler();Packet? reply=null;
        foreach(var frame in CanFraming.Fragment(request.Encode(),++transfer))model.Feed(frame);
        if(model.Stat(0)==0&&model.Stat(5)==0)model.Step(600);
        foreach(var frame in model.Pop())reply=reassembler.Feed(frame)??reply;
        if(reply is null)throw new TimeoutException("No complete CAN model reply");
        if(Drop.Remove((Command)request.Command)){Dropped++;throw new TimeoutException("Injected lost completed ACK");}
        return Task.FromResult(reply);
    }
    public ValueTask DisposeAsync()=>ValueTask.CompletedTask;
}
sealed class Model:IDisposable{
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]delegate void ResetFn();
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]delegate void FaultFn(ushort x);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]delegate uint StatFn(ushort x);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]delegate ushort WordFn(uint x);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]delegate void FeedFn([In]ushort[] frame,ushort n);
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]delegate ushort PopFn([Out]ushort[] frame);
    readonly nint lib;readonly ResetFn reset;readonly FaultFn fault,step;readonly StatFn stat;readonly WordFn word;readonly FeedFn feed;readonly PopFn pop;
    public Model(string path){lib=NativeLibrary.Load(Path.GetFullPath(path));T Get<T>(string name)where T:Delegate=>Marshal.GetDelegateForFunctionPointer<T>(NativeLibrary.GetExport(lib,name));reset=Get<ResetFn>("native_reset");fault=Get<FaultFn>("native_fault");step=Get<FaultFn>("native_step");stat=Get<StatFn>("native_stat");word=Get<WordFn>("native_read_word");feed=Get<FeedFn>("native_feed_frame");pop=Get<PopFn>("native_pop_frame");}
    public void Reset()=>reset();public void Fault(ushort x)=>fault(x);public void Step(ushort x)=>step(x);public uint Stat(ushort x)=>stat(x);public ushort Word(uint x)=>word(x);
    public void Feed(byte[] f)=>feed(f.Select(x=>(ushort)x).ToArray(),(ushort)f.Length);
    public IEnumerable<byte[]> Pop(){ushort[] f=new ushort[8];ushort n;while((n=pop(f))!=0)yield return f.Take(n).Select(x=>(byte)x).ToArray();}
    public void Dispose()=>NativeLibrary.Free(lib);
}
