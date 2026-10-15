//###########################################################################
/*本例程演示PWM波形输出实验。
工程演示说明：
1.本示例演示PWM波形输出，通过配置，ADP32F035的PWM1A和PWM1B输出约22.5KHZ的PWM波,采用中央对其PWM模式（即计数器增减模式）
2. AVP32F069有多达8个通道16路PWM波输出，在本实验中，我们的PWM1A和PWM1B分别是GPIO0和GPIO1。
3.本示例输出的波形为带死区设置，用户可通过修改死区时间或者去掉死区进行测试。
3.代码执行流程:
3.1 首先是系统时钟的选择,这里我们选取的是外部无源时钟(10MHZ),他通过19倍频后再2分频最终得到90MHZ的系统时钟；
3.2 然后是PIE中断向量表的初始化；
3.3 接下来是一些外设的初的初始化；
3.4 最后是一个空循环，pwm中断相关操作都在中断服务程序中完成。
4.其他说明
4.1 在本示例中PWM频率 = CPU主频/pwm周期，pwm周期取决于周期值EPWM1_TIMER_TBPRD，如果：TBPRD = 2000，
那么频率就是90 000KHZ/(2000*2) = 22.5KHZ，PWM占空比 = 输入占空比值pwm/周期值d，比如：周期 = 2000,pwm =1000时，
占空比即为50%，若有设置死区则应该把死区时间减去再计算占空比。
4.2 PWM试验重要函数说明：InitEPwm1Gpio ()（初始化外设 PWM1以及GPIO0/1作为PWM7B引脚输出）。
InitEPwm1Example() 函数中 设置响应输出方式，当TBCTR到达CxU/CxD时，AQ的PWMxA/PWMxB输出的高低：
EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;
EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;
EPwm1Regs.AQCTLB.bit.CBU = AQ_SET ;
EPwm1Regs.AQCTLB.bit.CBD = AQ_CLEAR;
4.观察实验现象：
4.1 将仿真器与开发板连接后，编译程序并下载并运行；
4.2 用户可以更改DEBUG_EN宏定义，来确定是进行仿真模式还是flash固化模式，如果定义了DEBUG_EN，则进行JTAG在线仿真模式。
否则程序不能在线仿真而是固化到Flash中，断电后重新上电程序也可以运行。
4.3 用户可通过示波器观测输出波形，注意查看死区时间。
 */
//###########################################################################

#include "AVP32F069_Project.h"
#include "string.h"

//EPwm1初始化
void InitEPwm1Example(void);
//EPwm1中断服务程序
__interrupt void epwm1_isr(void);

// 配置周期值
#define EPWM1_TIMER_TBPRD  2000  // 周期值
#define EPWM1_CMPA         1000     //比较寄存器A的值
#define EPWM1_CMPB         1000     //比较寄存器B的值


void main(void)
{
    // 第一步: 初始化系统系统时钟,包括：PLL, 看门狗时钟, 外设时钟的控制;
    InitSysCtrl();
    // 这里根据宏定义判断是使用哪种模式,如果没有定义DEBUG_EN则什么都不做,
    // 否则进入Flash固化模式,即要执行flash的初始化并将设定的部分在
    // RAM里面运行的代码拷贝到flash运行
    #ifndef  DEBUG_EN
    memcpy(&RamfuncsRunStart , &RamfuncsLoadStart, (Uint32)RamfuncsLoadSize);
    InitFlash();
    #endif

    // 第二步： 为ePWM1配置 GPIO 管脚
    InitEPwm1Gpio();

    // 第三步：清除所有中断标志并初始化PIE中断向量表
    DINT;  //先禁止CPU中断
    InitPieCtrl();//给PIE寄存器赋初值

    IER = 0x0000; //禁止CPU的中断并清楚所有相关的中断标志
    IFR = 0x0000;

    InitPieVectTable();//初始化中断向量表

    // 为PWM中断设置中断服务程序入口
    EALLOW;
    PieVectTable.EPWM1_INT = &epwm1_isr;
    EDIS;

    // 初始化ePwm配置
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0; //时钟与系统时钟同步
    EDIS;
    InitEPwm1Example();

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;


    //使能cpu中断组3
    IER |= M_INT3;

    // 使能中断组1中的中断1
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;


    EINT;   // 是能全局中断
    ERTM;   // 使能实时中断
    EALLOW;
    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;         // GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO20 = 0;          // 设置为输入
    GpioCtrlRegs.GPAQSEL2.bit.GPIO20 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO20 =1;           //由于开发板外置了上拉电阻，禁用DSP内部上拉

    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;         // GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO21 = 0;          // 设置为输入

    GpioCtrlRegs.GPAQSEL2.bit.GPIO21 = 0;
    GpioCtrlRegs.GPAPUD.bit.GPIO21 =1;           //由于开发板外置了上拉电阻，禁用DSP内部上拉
    EDIS;

    // 空循环
    for(;;)
    {
    __asm("          NOP");
    }

} 

__interrupt void epwm1_isr(void)
{

    //  清除此次中断标志
    EPwm1Regs.ETCLR.bit.INT = 1;

    // 接收同组其他中断
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}



//设置为带死区 EPWM1A/1B输出互补波形
void InitEPwm1Example()
{

    // 设置TBCLK
    EPwm1Regs.TBPRD = EPWM1_TIMER_TBPRD;           // 设置周期寄存器 周期为 2000*2*11.11us =44.44us
    EPwm1Regs.TBPHS.half.TBPHS = 0x0000;           // Phase is 0
    EPwm1Regs.TBCTR = 0x0000;                      // 计数器清零

    // 设置比较寄存器
    EPwm1Regs.CMPA.half.CMPA = EPWM1_CMPA;         // 设置比较寄存器A的值
    EPwm1Regs.CMPB = EPWM1_CMPB;                   // 设置比较寄存器B的值

    // 设置计数模式
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // 增减模式
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;        // 禁止相位使能
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;       // 时钟与系统时钟同步
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;          // 时钟与系统时钟同步

    // Setup shadowing
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;  // Load on Zero
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    // 设置响应方式
    EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;             // 计数器递增，当计数器等于CMPA时PWM1A置1
    EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;           // 计数器递减，当计数器等于CMPA时PWM1A置0

    EPwm1Regs.AQCTLB.bit.CBU = AQ_SET ;             // 计数器递增，当计数器等于CMPB时PWMB置1
    EPwm1Regs.AQCTLB.bit.CBD = AQ_CLEAR;           // 计数器递减，当计数器等于CMPB时PWM1A置0

    // 设置死区时间
    EPwm2Regs.DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;      // 对输入信号进行边沿控制，实现死区操作  若不需死区将该值设置为 DB_DISABLE
    EPwm2Regs.DBCTL.bit.IN_MODE = DBA_RED_DBB_FED;     // EPWMA作为上升沿和EPWMB作为上下降沿延时的信号源
    EPwm2Regs.DBCTL.bit.POLSEL = DB_ACTV_HIC;          // EPWMB反向输出
    EPwm2Regs.DBRED = 500;                             // 上升沿死区时间:500*TBCLK
    EPwm2Regs.DBFED = 500;                             // 下降沿死区时间:500*TBCLK

    //当需要在中断改变比较寄存器值的设置
    //EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;       // 当TBCTL=0时，产生EPWM1_INT中断事件
    // EPwm1Regs.ETSEL.bit.INTEN = 1;                  // 使能产生中断信号EPWM1_INT
    //EPwm1Regs.ETPS.bit.INTPRD = ET_3RD;            // 每发生三次事件，产生EPWM1_INT中断

}

//===========================================================================

//===========================================================================
