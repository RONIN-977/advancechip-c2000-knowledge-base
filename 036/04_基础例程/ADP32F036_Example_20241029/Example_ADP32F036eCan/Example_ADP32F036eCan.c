//工程演示说明
//1.本示例演示CAN的收发功能。使用GPIO0作为CANRX, GPIO32作为CANTX.
//2.代码执行流程:
//  2.1 首先是系统时钟的选择,这里我们选取的是外部无源时钟(10MHZ),他通过12倍频后再2分频最终得到60MHZ的系统时钟
//  2.2 然后是PIE中断向量表的初始化
//  2.3 接下来是一些外设的初的初始化,如果使用flash固化模式,还要执行一些flash相关的操作;
//  2.4 最后是一个while(1)超级循环,LED1 闪灯程序,CAN测试发送程序,处理CAN接收程序;
//3.其他说明
//  3.1 CAN差分线不能直接短接做自收自发试验,否则可能损坏CAN收发器,将USB转CAN的CANH,CANL差分线,
//      对应接到ADP32F036开发板的CN4A的CANH,CANL(也就是H-H,L-L)
//  3.2 重要函数说明
//      在CANcomm.h选择 CAN 发送函数是否等待
//      #define CANSENDNOWAIT   //CAN 发送不等待发送完毕 TA 标志 (默认测试程序选用)
//      #define CANNOWWAIT      //CAN 发送等待当前邮箱发送完毕 TA 标志
//      #define CANLASTWAIT     // CAN 发送等待上一个ID发送完毕 TA 标志，推荐方式
//      邮箱1-6为发送,标准帧
//      邮箱16为接收,接收ID为0xA0,过滤ID机制,标准帧
//      CAN 波特率500K,8字节数据长度
//  3.3 以HEX（十六进制）发送,最大4个字节长度,每个字节的范围 00到0F
//  3.4 串口调试助手设置:115200 8N1
//4.观察实验现象
//  4.1 将仿真器与开发板连接后,编译程序并下载并运行;
//  4.2 将EVM板与USB转串口线进行连接调试。
//  4.3 请在USB CAN或者其他CAN节点,以十六进制发送内容CAN消息,接收ID为0xA0,数据长度为8,
//      8个字节里的最低字节控制数码管显示的内容
//  4.4 用户可以更改Debug_Cfg.h中的DEBUG_EN宏定义,来确定是进行仿真模式还是flash固化模式
//      如果DEBUG_EN定义为1则进行Jtag在线仿真模式,如果DEBUG_EN为0则程序不能在线仿真而是
//      固化到Flash中，断电后重新上电程序也可以运行。

//头文件包含区
#include "OnChip_Driver_Inc.h"
#include "BspInc.h"

//函数声明区
void InitLED(void);
void InitBeep(void);
void InitKEY(void);

//预编译区
#if DEBUG_EN    > 0
#else
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
#endif

//变量声明区
Uint16 Beepcnt=0;

//主函数入口处
void main(void) 
{
    // 第一步: 初始化系统系统时钟,包括：PLL, 看门狗时钟, 外设时钟的控制;
    MemCopy(&RamfuncsLoadStart,&RamfuncsLoadEnd,&RamfuncsRunStart);
     InitFlash();
    InitSysCtrl();
    InitLED();

   /* EALLOW;
    GpioCtrlRegs.GPBMUX1.bit.GPIO32= 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1;
    EDIS;
    GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;// led 0ff

    GpioDataRegs.GPBSET.bit.GPIO32= 1; //led on
    GpioDataRegs.GPBTOGGLE.bit.GPIO32 =1;*/
     // 这里根据宏定义判断是使用哪种模式,如果DEBUG_EN > 0则什么都不做,
     // 否则进入Flash固化模式,即要执行flash的初始化并将设定的部分在
     // RAM里面运行的代码拷贝到flash运行
#if DEBUG_EN    > 0
     //
#else
   //  MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    // InitFlash();            //初始化flash
#endif

     // 第二步: 清除所有中断标志并初始化PIE中断向量表
     DINT;           //先禁止CPU中断
     InitPieCtrl();  //给PIE寄存器赋初值
     IER = 0x0000;   //禁止CPU的中断并清楚所有相关的中断标志
     IFR = 0x0000;
     InitPieVectTable();//初始化中断向量表

    // 第三步：初始化其他使用的外设
    // InitLED();

     LED1_ON();

     Timer0_init();
#if 1// gxq
     SCI_Init();
     open_uart_debug();
     printf("\r\n\r\nADP32F036 is ready!");
     printf("\r\n--------ADP32F036片上CAN外设的基本使用方法-------\r\n");
     printf("\r\n\r\nCAN 差分线不能直接短接做自收自发试验，否则可能损坏CAN收发器");
     printf("\r\n请在USB CAN或者其他CAN节点， 以十六进制发送内容CAN消息，接收ID为0xA0,数据长度为8");
#endif
     CANInit();//CAN初始化
     //初始化发送参数
#if TESTMODE
     PtrCanTx-> MBXnbr = 1;       //1号邮箱发送
#else

     PtrCanTx-> MBXnbr = 16;       //16号邮箱发送
#endif
    // PtrCanTx-> RevMBXnID = 0x50; //邮箱ID 取值范围 0x00 到 0x3FF , 11bit 标准帧ID
     PtrCanTx-> RevMBXnID = 0x709;
     PtrCanTx->DA = 0x0001;
     PtrCanTx->DB = 0x0B02;
     PtrCanTx->DC = 0x0C03;
     PtrCanTx->DD = 0x0D04;


     PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
     EINT;  // Enable Global interrupt INTM
     ERTM;

    EALLOW;
 //  ECanaRegs.CANME.bit.ME14= 0;
 //  ECanaRegs.CANME.all |= 0x400;
 //   __asm(" RPT #8 || NOP");
 //  ECanaMboxes.MBOX14.MSGID.bit.STDMSGID = 0x714;
   // __asm(" RPT #8 || NOP");
 //  ECanaRegs.CANME.bit.ME14= 1;
   EDIS;

     while(1)
     {
         if(timer0Base.Mark_Para.Status_Bits.OnemsdFlag == 1)
         {
             timer0Base.Mark_Para.Status_Bits.OnemsdFlag = 0;

             if(timer0Base.msCounter >= 100)//ms
             {
                 timer0Base.msCounter = 0;
                 LED1_TOGGLE();

                 //CAN测试发送
                  if(PtrCanTx->MBXnbr <= 19)
                  {
                      CAN_Send(PtrCanTx);
                      PtrCanTx->DA ++;   //改变发送数据
                      PtrCanTx->DB =  ECanaRegs.CANRMP.all ;
                      PtrCanTx->DC= ECanaMboxes.MBOX14.MSGID.bit.STDMSGID;
                      PtrCanTx->MBXnbr++;//改变发送邮箱
                      PtrCanTx->RevMBXnID++;//改变接收邮箱
                      if(PtrCanTx->MBXnbr == 19)
                      {
                          PtrCanTx-> MBXnbr = 16;       //1号邮箱发送
                          PtrCanTx-> RevMBXnID = 0x709; //邮箱ID
                      }
                  }
             }
           }


     }
}


void InitKEY(void)
{
    EALLOW;
    K1_MUX = 0;//GPIO功能
    K1_DIR = 0;//输入
    K1_PUD = 1;//由于开发板外置了上拉电阻，禁用DSP内部上拉

    K2_MUX = 0;//GPIO功能
    K2_DIR = 0;//输入
    K2_PUD = 1;//由于开发板外置了上拉电阻，禁用DSP内部上拉



    EDIS;

}

void InitLED(void)
{
    EALLOW;
    //4个普通LED
    LED1_MUX = 0;//GPIO功能
    LED1_DIR = 1;//输出


    EDIS;
}

void InitBeep(void)
{
    BEEP_OFF();//默认关闭蜂鸣器输出
    EALLOW;
    BEEP_MUX = 0;//普通IO功能
    BEEP_DIR = 1;   //输出
    EDIS;
}

//===========================================================================
// No more.
//===========================================================================
