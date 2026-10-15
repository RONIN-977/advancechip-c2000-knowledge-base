/*************************************工程示例说明 *************************************/
// 功能：ADP32F03x芯片CANFD滤波配置与数据收发例程
//       1. 初始化CANFD为500KHz(仲裁段)/2MHz(数据段)波特率，GPIO0=RX、GPIO32=TX
//       2. 配置双滤波器：通道0（标准帧）+通道4（扩展帧），仅接收匹配ID的CANFD数据帧
//       3. 实现“接收-回传”功能：过滤接收指定ID数据后原路发送，LED1翻转指示收发完成
// 滤波规则：
//       - 滤波器0（标准帧）：接收代码0x125，掩码0x100
//       - 滤波器4（扩展帧）：接收代码0x101C23C，掩码0x0000011
/**********************************************************************************/
//头文件包含区
#include "OnChip_Driver_Inc.h"
#include "ECanFunc.h"

//函数声明区
void delay_loop(void);
void LED_example(void);
void InitLED(void);
void InitKEY(void);
Uint16 rxData[32];
Uint16 txData[32];
Uint16 tmpbuf[16][32];
//预编译区
#if DEBUG_EN    > 0
#else
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
#endif
int temp;

//变量声明区

//主函数入口处
void main(void)
{
    // 第一步: 初始化系统系统时钟,包括：PLL, 看门狗时钟, 外设时钟的控制;
    MemCopy(&RamfuncsLoadStart,&RamfuncsLoadEnd,&RamfuncsRunStart);
    InitFlash();        //初始化flash
    InitSysCtrl();
    // 这里根据宏定义判断是使用哪种模式,如果DEBUG_EN > 0则什么都不做,否则进入Flash固化模式,
    // 即要执行flash的初始化并将设定的部分在RAM里面运行的代码拷贝到flash运行
#if DEBUG_EN    > 0
    //
#else

#endif
    InitGpio();
    // 第二步: 清除所有中断标志并初始化PIE中断向量表
    DINT;           //先禁止CPU中断
    InitPieCtrl();  //给PIE寄存器赋初值
    IER = 0x0000;   //禁止CPU的中断并清楚所有相关的中断标志
    IFR = 0x0000;
    InitPieVectTable();//初始化中断向量表


    // 第三步：初始化其他使用的外设
    InitCanfd();
    InitCanfdGpio();

    InitLED();
    InitKEY();

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;    // Enable the PIE block
    PieCtrlRegs.PIEACK.all = 0xFFFF;

    EINT;                                 // Enable Global interrupt INTM
    ERTM;



    //配置接收滤波参数

    CanfdRegs.CFG_STAT.bit.RESET = 1;     // 进入CANFD复位模式（配置滤波前必须复位）
    CanfdRegs.ACF_EN.all = 0x11;          // 使能滤波器通道0和4（bit0=1、bit4=1）

    // 8.1 配置滤波器0（标准帧滤波）
    CanfdRegs.CIA_ACF_CFG.bit.SELMASK = 0;// 选择ACF寄存器为“接收代码”模式
    CanfdRegs.CIA_ACF_CFG.bit.ACFADR = 0; // 选择滤波器通道0
    CanfdRegs.ACF_0.all = 0x125;          // 标准帧接收代码：0x125（11位ID）
    CanfdRegs.ACF_1.all = 0;              // 标准帧无扩展位，置0

    CanfdRegs.CIA_ACF_CFG.bit.SELMASK = 1;// 切换ACF寄存器为“接收掩码”模式
    CanfdRegs.CIA_ACF_CFG.bit.ACFADR = 0; // 选择滤波器通道0
    CanfdRegs.ACF_0.all = 0x100;          // 标准帧掩码：0x100（仅匹配bit8位）
    CanfdRegs.ACF_1.all = 0;              // 标准帧掩码扩展位置0

    // 8.2 配置滤波器4（扩展帧滤波）
    CanfdRegs.CIA_ACF_CFG.bit.SELMASK = 0;// 切换为“接收代码”模式
    CanfdRegs.CIA_ACF_CFG.bit.ACFADR = 4; // 选择滤波器通道4
    CanfdRegs.ACF_0.all = 0xC23C;         // 扩展帧接收代码低16位：0xC23C
    CanfdRegs.ACF_1.all = 0x101;          // 扩展帧接收代码高13位：0x101

    CanfdRegs.CIA_ACF_CFG.bit.SELMASK = 1;// 切换为“接收掩码”模式
    CanfdRegs.CIA_ACF_CFG.bit.ACFADR = 4; // 选择滤波器通道4
    CanfdRegs.ACF_0.all = 0x11;           // 扩展帧掩码低16位：0x11
    CanfdRegs.ACF_1.all = 0;              // 扩展帧掩码高13位置0
    CanfdRegs.ACF_1.bit.rsvd_AIDEE_14 = 1;// 扩展帧使能位：强制接收扩展帧
    CanfdRegs.ACF_1.bit.rsvd_AIDE_13 = 1; // 扩展帧标识位：rsvd_AIDEE_14=1时生效

    CanfdRegs.CFG_STAT.bit.RESET = 0;     // 退出CANFD复位模式，滤波配置生效


    int i;
    for(i=0; i<32; i++)
    {
        rxData[i] = 0x00;
        txData[i] = 0x1200+i;
    }
   sCANFDDataTx(&txData[0],8);  //发送数据


    // 第四步: 执行系统任务
    while(1)
    {
        temp = 0;
        while(temp==0)
        {
            temp = sCANFDDataRx(rxData);  //接收数据
        }
        sCANFDDataTx(&rxData[0],temp);     // 发送接收到数据
        LED1_TOGGLE();   //收到一次数据翻转一次led
    }
}

void delay_loop(void)
{   //延时
    Uint32 i;
    for (i = 0; i < 100000; i++) 
    {
        ;//执行空操作
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
    //1个普通LED
    LED1_MUX = 0;//GPIO功能
    LED1_DIR = 1;//输出
    EDIS;
}


//===========================================================================
// No more.
//===========================================================================

