/******************************************************************************
文件名                     : AVP33x_DMA.h
作者                         : 湖南进芯电子科技有限公司
功能描述                 : AVP33x芯片DMA模块初始化及函数定义
创建时间                 : 06-25-2019
版本                         : V1.0
修订时间                 :
修订内容                 :
*******************************************************************************/

#include "AVP33x_Device.h"     // Headerfile Include File
#include "AVP33x_Examples.h"   // Examples Include File

//---------------------------------------------------------------------------
// DMAInitialize:
//---------------------------------------------------------------------------
// 初始化DMA寄存器
//
void DMAInitialize(void)
{
    EALLOW;

    DmaRegs.DMACTRL.bit.HARDRESET = 1;  // 硬件复位DMA
    asm (" nop");                       // 一个延时确保复位完成
    DmaRegs.DEBUGCTRL.bit.FREE = 1;     // DMA允许不受仿真挂起影响

    EDIS;
}

//---------------------------------------------------------------------------
// DMACH1AddrConfig:
//---------------------------------------------------------------------------
// DMA通道1传送地址配置函数
//
void DMACH1AddrConfig(volatile Uint16 *DMA_Dest,volatile Uint16 *DMA_Source)
{
    EALLOW;
    // 设置DMA源地址
    DmaRegs.CH1.SRC_BEG_ADDR_SHADOW = (Uint32)DMA_Source;   // 指向源的起始地址
    DmaRegs.CH1.SRC_ADDR_SHADOW =     (Uint32)DMA_Source;

    // 设置DMA目标地址
    DmaRegs.CH1.DST_BEG_ADDR_SHADOW = (Uint32)DMA_Dest;	    // 指向目标的起始地址
    DmaRegs.CH1.DST_ADDR_SHADOW =     (Uint32)DMA_Dest;

    EDIS;
}

//---------------------------------------------------------------------------
// DMACH1BurstConfig:
//---------------------------------------------------------------------------
// DMA通道1突发配置函数
//
void DMACH1BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep)
{
    EALLOW;

    // 配置突发寄存器
    DmaRegs.CH1.BURST_SIZE.all = bsize;         // 配置一次突发传送的数据个数
    DmaRegs.CH1.SRC_BURST_STEP = srcbstep;	    // 配置在一次突发传送结束后，源地址指针增加/减少的步长
    DmaRegs.CH1.DST_BURST_STEP = desbstep;      // 配置在一次突发传送结束后，目标地址指针增加/减少的步长

    EDIS;
}

//---------------------------------------------------------------------------
// DMACH1TransferConfig:
//---------------------------------------------------------------------------
// DMA通道1外环传送配置函数
//
void DMACH1TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep)
{
    EALLOW;

	// 配置外环传送寄存器
	DmaRegs.CH1.TRANSFER_SIZE = tsize;          // 配置在一次传送过程中突发传送的次数
	DmaRegs.CH1.SRC_TRANSFER_STEP = srctstep;   // 配置在一次突发传送结束后，源地址指针增加/减少的步长
	DmaRegs.CH1.DST_TRANSFER_STEP = deststep;   // 配置在一次突发传送结束后，目标地址指针增加/减少的步长

	EDIS;
}

//---------------------------------------------------------------------------
// DMACH1WrapConfig:
//---------------------------------------------------------------------------
// DMA通道1返回配置函数
//
void DMACH1WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize, int16 deswstep)
{
	EALLOW;

	// 配置返回寄存器
	DmaRegs.CH1.SRC_WRAP_SIZE = srcwsize;	    // 配置在返回源的开始地址指针前突发传送的次数。如果这个值比TRANSFER_SIZE大，则源地址不会返回；反之，如果这个值比TRANSFER_SIZE小，则SRC_TRANSFER_STEP不起作用。
    DmaRegs.CH1.SRC_WRAP_STEP = srcwstep;	    // 配置在返回寄存器归零后，源地址指针增加/减少的步长

	DmaRegs.CH1.DST_WRAP_SIZE = deswsize;	    // 配置在返回目标的开始地址指针前突发传送的次数如果这个值比TRANSFER_SIZE大，则目标地址不会返回；反之，如果这个值比TRANSFER_SIZE小，则DST_TRANSFER_STEP不起作用。
	DmaRegs.CH1.DST_WRAP_STEP = deswstep;	    // 配置在返回寄存器归零后，目标地址指针增加/减少的步长

	EDIS;
}

//---------------------------------------------------------------------------
// DMACH1ModeConfig:
//---------------------------------------------------------------------------
// DMA通道1模式配置函数
//
void DMACH1ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte)
{
	EALLOW;

	// 配置模式寄存器
	DmaRegs.CH1.MODE.bit.PERINTSEL = persel;	    // 将外设中断源传送给DMA通道1
	DmaRegs.CH1.MODE.bit.PERINTE = perinte;       	// 使能外设中断触发信号
	DmaRegs.CH1.MODE.bit.ONESHOT = oneshot;       	// 使能单次触发传送
	DmaRegs.CH1.MODE.bit.CONTINUOUS = cont;    		// 使能连续传送
	DmaRegs.CH1.MODE.bit.SYNCE = synce;         	// 外设同步信号使能
	DmaRegs.CH1.MODE.bit.SYNCSEL = syncsel;       	// 同步模式选择
	DmaRegs.CH1.MODE.bit.OVRINTE = ovrinte;         // 溢出中断使能
	DmaRegs.CH1.MODE.bit.DATASIZE = datasize;      	// 选择DMA通道的数据宽度
	DmaRegs.CH1.MODE.bit.CHINTMODE = chintmode;		// 选择通道的中断模式（即在一次传送的开始还是结束时产生中断）
	DmaRegs.CH1.MODE.bit.CHINTE = chinte;        	// 通道中断使能

	// 清除标志
	DmaRegs.CH1.CONTROL.bit.PERINTCLR = 1;  		// 清除被锁存的外部中断事件，并将PEIINTFLG清零
	DmaRegs.CH1.CONTROL.bit.SYNCCLR = 1;    		// 清除被锁存的同步事件，并将SYNCFLG清零
	DmaRegs.CH1.CONTROL.bit.ERRCLR = 1; 	     	// 清除同步错误标志位，并将OVRFLG清零

	// 初始化PIE向量
	PieCtrlRegs.PIEIER7.bit.INTx1 = 1;              // 使能DMA通道1的PIE级中断

	EDIS;
}

//---------------------------------------------------------------------------
// StartDMACH1:
//---------------------------------------------------------------------------
// DMA通道1启动函数
//
void StartDMACH1(void)
{
	EALLOW;
	DmaRegs.CH1.CONTROL.bit.RUN = 1;                // 启动DMA通道1
	EDIS;
}


//---------------------------------------------------------------------------
// DMACH2AddrConfig:
//---------------------------------------------------------------------------
// DMA通道2传送地址配置函数
//
void DMACH2AddrConfig(volatile Uint16 *DMA_Dest,volatile Uint16 *DMA_Source)
{
	EALLOW;

	// 设置DMA源地址
	DmaRegs.CH2.SRC_BEG_ADDR_SHADOW = (Uint32)DMA_Source;   // 指向源的起始地址
	DmaRegs.CH2.SRC_ADDR_SHADOW =     (Uint32)DMA_Source;

	// 设置DMA目标地址
	DmaRegs.CH2.DST_BEG_ADDR_SHADOW = (Uint32)DMA_Dest;	    // 指向目标的起始地址
	DmaRegs.CH2.DST_ADDR_SHADOW =     (Uint32)DMA_Dest;


	EDIS;
}

//---------------------------------------------------------------------------
// DMACH2BurstConfig:
//---------------------------------------------------------------------------
// DMA通道2突发配置函数
//
void DMACH2BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep)
{
	EALLOW;

	// 配置突发寄存器
	DmaRegs.CH2.BURST_SIZE.all = bsize;         // 配置一次突发传送的数据个数
	DmaRegs.CH2.SRC_BURST_STEP = srcbstep;		// 配置在一次突发传送结束后，源地址指针增加/减少的步长
	DmaRegs.CH2.DST_BURST_STEP = desbstep;      // 配置在一次突发传送结束后，目标地址指针增加/减少的步长


	EDIS;
}

//---------------------------------------------------------------------------
// DMACH2TransferConfig:
//---------------------------------------------------------------------------
// DMA通道2外环传送配置函数
//
void DMACH2TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep)
{
	EALLOW;

	// 配置外环传送寄存器
	DmaRegs.CH2.TRANSFER_SIZE = tsize;          // 配置在一次传送过程中突发传送的次数
	DmaRegs.CH2.SRC_TRANSFER_STEP = srctstep;	// 配置在一次突发传送结束后，源地址指针增加/减少的步长
	DmaRegs.CH2.DST_TRANSFER_STEP = deststep;	// 配置在一次突发传送结束后，目标地址指针增加/减少的步长

	EDIS;
}

//---------------------------------------------------------------------------
// DMACH2WrapConfig:
//---------------------------------------------------------------------------
// DMA通道2返回配置函数
//
void DMACH2WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize, int16 deswstep)
{
	EALLOW;

	// 配置返回寄存器
	DmaRegs.CH2.SRC_WRAP_SIZE = srcwsize;		// 配置在返回源的开始地址指针前突发传送的次数
    DmaRegs.CH2.SRC_WRAP_STEP = srcwstep;		// 配置在返回寄存器归零后，源地址指针增加/减少的步长

	DmaRegs.CH2.DST_WRAP_SIZE = deswsize;		// 配置在返回目标的开始地址指针前突发传送的次数
	DmaRegs.CH2.DST_WRAP_STEP = deswstep;		// 配置在返回寄存器归零后，目标地址指针增加/减少的步长

	EDIS;
}

//---------------------------------------------------------------------------
// DMACH2ModeConfig:
//---------------------------------------------------------------------------
// DMA通道2模式配置函数
//
void DMACH2ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte)
{
	EALLOW;

	// 配置模式寄存器
	DmaRegs.CH2.MODE.bit.PERINTSEL = persel;	    // 将外设中断源传送给DMA通道2
	DmaRegs.CH2.MODE.bit.PERINTE = perinte;       	// 使能外设中断触发信号
	DmaRegs.CH2.MODE.bit.ONESHOT = oneshot;       	// 使能单次触发传送
	DmaRegs.CH2.MODE.bit.CONTINUOUS = cont;    		// 使能连续传送
	DmaRegs.CH2.MODE.bit.SYNCE = synce;         	// 外设同步信号使能
	DmaRegs.CH2.MODE.bit.SYNCSEL = syncsel;       	// 同步模式选择
	DmaRegs.CH2.MODE.bit.OVRINTE = ovrinte;         // 溢出中断使能
	DmaRegs.CH2.MODE.bit.DATASIZE = datasize;      	// 选择DMA通道的数据宽度
	DmaRegs.CH2.MODE.bit.CHINTMODE = chintmode;		// 选择通道的中断模式（即在一次传送的开始还是结束时产生中断）
	DmaRegs.CH2.MODE.bit.CHINTE = chinte;        	// 通道中断使能

	// 清除标志
	DmaRegs.CH2.CONTROL.bit.PERINTCLR = 1;  		// 清除被锁存的外部中断事件，并将PEIINTFLG清零
	DmaRegs.CH2.CONTROL.bit.SYNCCLR = 1;    		// 清除被锁存的同步事件，并将SYNCFLG清零
	DmaRegs.CH2.CONTROL.bit.ERRCLR = 1; 	     	// 清除同步错误标志位，并将OVRFLG清零

	// 初始化PIE向量
	PieCtrlRegs.PIEIER7.bit.INTx2 = 1;              // 使能DMA通道2的PIE级中断

	EDIS;
}

//---------------------------------------------------------------------------
// StartDMACH2:
//---------------------------------------------------------------------------
// DMA通道2启动函数
//
void StartDMACH2(void)
{
	EALLOW;
	DmaRegs.CH2.CONTROL.bit.RUN = 1;                // 启动DMA通道2
	EDIS;
}


//---------------------------------------------------------------------------
// DMACH3AddrConfig:
//---------------------------------------------------------------------------
// DMA通道3传送地址配置函数
//
void DMACH3AddrConfig(volatile Uint16 *DMA_Dest,volatile Uint16 *DMA_Source)
{
	EALLOW;

	// 设置DMA源地址
	DmaRegs.CH3.SRC_BEG_ADDR_SHADOW = (Uint32)DMA_Source;   // 指向源的起始地址
	DmaRegs.CH3.SRC_ADDR_SHADOW =     (Uint32)DMA_Source;

	// 设置DMA目标地址
	DmaRegs.CH3.DST_BEG_ADDR_SHADOW = (Uint32)DMA_Dest;	    // 指向目标的起始地址
	DmaRegs.CH3.DST_ADDR_SHADOW =     (Uint32)DMA_Dest;

	EDIS;
}

//---------------------------------------------------------------------------
// DMACH3BurstConfig:
//---------------------------------------------------------------------------
// DMA通道3突发配置函数
//
void DMACH3BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep)
{
	EALLOW;

	// 配置突发寄存器
	DmaRegs.CH3.BURST_SIZE.all = bsize;	                // 配置一次突发传送的数据个数
	DmaRegs.CH3.SRC_BURST_STEP = srcbstep;			    // 配置在一次突发传送结束后，源地址指针增加/减少的步长
	DmaRegs.CH3.DST_BURST_STEP = desbstep;              // 配置在一次突发传送结束后，目标地址指针增加/减少的步长

	EDIS;
}

//---------------------------------------------------------------------------
// DMACH3TransferConfig:
//---------------------------------------------------------------------------
// DMA通道3外环传送配置函数
//
void DMACH3TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep)
{
	EALLOW;

	// 配置外环传送寄存器
	DmaRegs.CH3.TRANSFER_SIZE = tsize;                  // 配置在一次传送过程中突发传送的次数
	DmaRegs.CH3.SRC_TRANSFER_STEP = srctstep;			// 配置在一次突发传送结束后，源地址指针增加/减少的步长
	DmaRegs.CH3.DST_TRANSFER_STEP = deststep;			// 配置在一次突发传送结束后，目标地址指针增加/减少的步长

	EDIS;
}

//---------------------------------------------------------------------------
// DMACH3WrapConfig:
//---------------------------------------------------------------------------
// DMA通道3返回配置函数
//
void DMACH3WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize, int16 deswstep)
{
	EALLOW;

	// 配置返回寄存器
	DmaRegs.CH3.SRC_WRAP_SIZE = srcwsize;       // 配置在返回源的开始地址指针前突发传送的次数
    DmaRegs.CH3.SRC_WRAP_STEP = srcwstep;		// 配置在返回寄存器归零后，源地址指针增加/减少的步长

	DmaRegs.CH3.DST_WRAP_SIZE = deswsize;		// 配置在返回目标的开始地址指针前突发传送的次数
	DmaRegs.CH3.DST_WRAP_STEP = deswstep;		// 配置在返回寄存器归零后，目标地址指针增加/减少的步长

	EDIS;
}

//---------------------------------------------------------------------------
// DMACH3ModeConfig:
//---------------------------------------------------------------------------
// DMA通道3模式配置函数
//
void DMACH3ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte)
{
	EALLOW;

	// 配置模式寄存器
	DmaRegs.CH3.MODE.bit.PERINTSEL = persel;	    // 将外设中断源传送给DMA通道3
	DmaRegs.CH3.MODE.bit.PERINTE = perinte;       	// 使能外设中断触发信号
	DmaRegs.CH3.MODE.bit.ONESHOT = oneshot;       	// 使能单次触发传送
	DmaRegs.CH3.MODE.bit.CONTINUOUS = cont;    		// 使能连续传送
	DmaRegs.CH3.MODE.bit.SYNCE = synce;         	// 外设同步信号使能
	DmaRegs.CH3.MODE.bit.SYNCSEL = syncsel;       	// 同步模式选择
	DmaRegs.CH3.MODE.bit.OVRINTE = ovrinte;         // 溢出中断使能
	DmaRegs.CH3.MODE.bit.DATASIZE = datasize;      	// 选择DMA通道的数据宽度
	DmaRegs.CH3.MODE.bit.CHINTMODE = chintmode;		// 选择通道的中断模式（即在一次传送的开始还是结束时产生中断）
	DmaRegs.CH3.MODE.bit.CHINTE = chinte;        	// 通道中断使能

	// 清除标志
	DmaRegs.CH3.CONTROL.bit.PERINTCLR = 1;  		// 清除被锁存的外部中断事件，并将PEIINTFLG清零
	DmaRegs.CH3.CONTROL.bit.SYNCCLR = 1;    		// 清除被锁存的同步事件，并将SYNCFLG清零
	DmaRegs.CH3.CONTROL.bit.ERRCLR = 1; 	     	// 清除同步错误标志位，并将OVRFLG清零

	// 初始化PIE向量
	PieCtrlRegs.PIEIER7.bit.INTx3 = 1;              // 使能DMA通道3的PIE级中断

	EDIS;
}

//---------------------------------------------------------------------------
// StartDMACH3:
//---------------------------------------------------------------------------
// DMA通道3启动函数
//
void StartDMACH3(void)
{
	EALLOW;
	DmaRegs.CH3.CONTROL.bit.RUN = 1;                // 启动DMA通道3
	EDIS;
}


//---------------------------------------------------------------------------
// DMACH4AddrConfig:
//---------------------------------------------------------------------------
// DMA通道4传送地址配置函数
//
void DMACH4AddrConfig(volatile Uint16 *DMA_Dest,volatile Uint16 *DMA_Source)
{
	EALLOW;

	// 设置DMA源地址
	DmaRegs.CH4.SRC_BEG_ADDR_SHADOW = (Uint32)DMA_Source;	// 指向源的起始地址
	DmaRegs.CH4.SRC_ADDR_SHADOW =     (Uint32)DMA_Source;

	// 设置DMA目标地址
	DmaRegs.CH4.DST_BEG_ADDR_SHADOW = (Uint32)DMA_Dest;	    // 指向目标的起始地址
	DmaRegs.CH4.DST_ADDR_SHADOW =     (Uint32)DMA_Dest;

	EDIS;
}

//---------------------------------------------------------------------------
// DMACH4BurstConfig:
//---------------------------------------------------------------------------
// DMA通道4突发配置函数
//
void DMACH4BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep)
{
	EALLOW;

	// 配置突发寄存器
	DmaRegs.CH4.BURST_SIZE.all = bsize;	        // 配置一次突发传送的数据个数
	DmaRegs.CH4.SRC_BURST_STEP = srcbstep;		// 配置在一次突发传送结束后，源地址指针增加/减少的步长
	DmaRegs.CH4.DST_BURST_STEP = desbstep;      // 配置在一次突发传送结束后，目标地址指针增加/减少的步长

	EDIS;
}

//---------------------------------------------------------------------------
// DMACH4TransferConfig:
//---------------------------------------------------------------------------
// DMA通道4外环传送配置函数
//
void DMACH4TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep)
{
	EALLOW;

	// 配置外环传送寄存器
	DmaRegs.CH4.TRANSFER_SIZE = tsize;          // 配置在一次传送过程中突发传送的次数
	DmaRegs.CH4.SRC_TRANSFER_STEP = srctstep;	// 配置在一次突发传送结束后，源地址指针增加/减少的步长
	DmaRegs.CH4.DST_TRANSFER_STEP = deststep;	// 配置在一次突发传送结束后，目标地址指针增加/减少的步长

	EDIS;
}

//---------------------------------------------------------------------------
// DMACH4WrapConfig:
//---------------------------------------------------------------------------
// DMA通道4返回配置函数
//
void DMACH4WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize, int16 deswstep)
{
	EALLOW;

	// 配置返回寄存器
	DmaRegs.CH4.SRC_WRAP_SIZE = srcwsize;	    // 配置在返回源的开始地址指针前突发传送的次数
    DmaRegs.CH4.SRC_WRAP_STEP = srcwstep;		// 配置在返回寄存器归零后，源地址指针增加/减少的步长

	DmaRegs.CH4.DST_WRAP_SIZE = deswsize;		// 配置在返回目标的开始地址指针前突发传送的次数
	DmaRegs.CH4.DST_WRAP_STEP = deswstep;		// 配置在返回寄存器归零后，目标地址指针增加/减少的步长

	EDIS;
}

//---------------------------------------------------------------------------
// DMACH4ModeConfig:
//---------------------------------------------------------------------------
// DMA通道4模式配置函数
//
void DMACH4ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte)
{
	EALLOW;

	// 配置模式寄存器
	DmaRegs.CH4.MODE.bit.PERINTSEL = persel;	    // 将外设中断源传送给DMA通道4
	DmaRegs.CH4.MODE.bit.PERINTE = perinte;       	// 使能外设中断触发信号
	DmaRegs.CH4.MODE.bit.ONESHOT = oneshot;       	// 使能单次触发传送
	DmaRegs.CH4.MODE.bit.CONTINUOUS = cont;    		// 使能连续传送
	DmaRegs.CH4.MODE.bit.SYNCE = synce;         	// 外设同步信号使能
	DmaRegs.CH4.MODE.bit.SYNCSEL = syncsel;       	// 同步模式选择
	DmaRegs.CH4.MODE.bit.OVRINTE = ovrinte;         // 溢出中断使能
	DmaRegs.CH4.MODE.bit.DATASIZE = datasize;      	// 选择DMA通道的数据宽度
	DmaRegs.CH4.MODE.bit.CHINTMODE = chintmode;		// 选择通道的中断模式（即在一次传送的开始还是结束时产生中断）
	DmaRegs.CH4.MODE.bit.CHINTE = chinte;        	// 通道中断使能

	// 清除标志
	DmaRegs.CH4.CONTROL.bit.PERINTCLR = 1;  		// 清除被锁存的外部中断事件，并将PEIINTFLG清零
	DmaRegs.CH4.CONTROL.bit.SYNCCLR = 1;    		// 清除被锁存的同步事件，并将SYNCFLG清零
	DmaRegs.CH4.CONTROL.bit.ERRCLR = 1; 	     	// 清除同步错误标志位，并将OVRFLG清零

	// 初始化PIE向量
	PieCtrlRegs.PIEIER7.bit.INTx4 = 1;              // 使能DMA通道4的PIE级中断

	EDIS;
}

//---------------------------------------------------------------------------
// StartDMACH4:
//---------------------------------------------------------------------------
// DMA通道4启动函数
//
void StartDMACH4(void)
{
	EALLOW;
	DmaRegs.CH4.CONTROL.bit.RUN = 1;                // 启动DMA通道4
	EDIS;
}


//---------------------------------------------------------------------------
// DMACH5AddrConfig:
//---------------------------------------------------------------------------
// DMA通道5传送地址配置函数
//
void DMACH5AddrConfig(volatile Uint16 *DMA_Dest,volatile Uint16 *DMA_Source)
{
	EALLOW;

	// 设置DMA源地址
	DmaRegs.CH5.SRC_BEG_ADDR_SHADOW = (Uint32)DMA_Source;	// 指向源的起始地址
	DmaRegs.CH5.SRC_ADDR_SHADOW =     (Uint32)DMA_Source;

	// 设置DMA目标地址
	DmaRegs.CH5.DST_BEG_ADDR_SHADOW = (Uint32)DMA_Dest;	    // 指向目标的起始地址
	DmaRegs.CH5.DST_ADDR_SHADOW =     (Uint32)DMA_Dest;


	EDIS;
}

//---------------------------------------------------------------------------
// DMACH5BurstConfig:
//---------------------------------------------------------------------------
// DMA通道5突发配置函数
//
void DMACH5BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep)
{
	EALLOW;

	// 配置突发寄存器
	DmaRegs.CH5.BURST_SIZE.all = bsize;         // 配置一次突发传送的数据个数
	DmaRegs.CH5.SRC_BURST_STEP = srcbstep;		// 配置在一次突发传送结束后，源地址指针增加/减少的步长
	DmaRegs.CH5.DST_BURST_STEP = desbstep;      // 配置在一次突发传送结束后，目标地址指针增加/减少的步长

	EDIS;
}

//---------------------------------------------------------------------------
// DMACH5TransferConfig:
//---------------------------------------------------------------------------
// DMA通道5外环传送配置函数
//
void DMACH5TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep)
{
	EALLOW;

	// 配置外环传送寄存器
	DmaRegs.CH5.TRANSFER_SIZE = tsize;          // 配置在一次传送过程中突发传送的次数
	DmaRegs.CH5.SRC_TRANSFER_STEP = srctstep;	// 配置在一次突发传送结束后，源地址指针增加/减少的步长
	DmaRegs.CH5.DST_TRANSFER_STEP = deststep;	// 配置在一次突发传送结束后，目标地址指针增加/减少的步长

	EDIS;
}

//---------------------------------------------------------------------------
// DMACH5WrapConfig:
//---------------------------------------------------------------------------
// DMA通道5返回配置函数
//
void DMACH5WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize, int16 deswstep)
{
	EALLOW;

	// 配置返回寄存器
	DmaRegs.CH5.SRC_WRAP_SIZE = srcwsize;       // 配置在返回源的开始地址指针前突发传送的次数
    DmaRegs.CH5.SRC_WRAP_STEP = srcwstep;		// 配置在返回寄存器归零后，源地址指针增加/减少的步长

	DmaRegs.CH5.DST_WRAP_SIZE = deswsize;		// 配置在返回目标的开始地址指针前突发传送的次数
	DmaRegs.CH5.DST_WRAP_STEP = deswstep;		// 配置在返回寄存器归零后，目标地址指针增加/减少的步长

	EDIS;
}

//---------------------------------------------------------------------------
// DMACH5ModeConfig:
//---------------------------------------------------------------------------
// DMA通道5模式配置函数
//
void DMACH5ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte)
{
	EALLOW;

	// 配置模式寄存器
	DmaRegs.CH5.MODE.bit.PERINTSEL = persel;	    // 将外设中断源传送给DMA通道5
	DmaRegs.CH5.MODE.bit.PERINTE = perinte;       	// 使能外设中断触发信号
	DmaRegs.CH5.MODE.bit.ONESHOT = oneshot;       	// 使能单次触发传送
	DmaRegs.CH5.MODE.bit.CONTINUOUS = cont;    		// 使能连续传送
	DmaRegs.CH5.MODE.bit.SYNCE = synce;         	// 外设同步信号使能
	DmaRegs.CH5.MODE.bit.SYNCSEL = syncsel;       	// 同步模式选择
	DmaRegs.CH5.MODE.bit.OVRINTE = ovrinte;         // 溢出中断使能
	DmaRegs.CH5.MODE.bit.DATASIZE = datasize;      	// 选择DMA通道的数据宽度
	DmaRegs.CH5.MODE.bit.CHINTMODE = chintmode;		// 选择通道的中断模式（即在一次传送的开始还是结束时产生中断）
	DmaRegs.CH5.MODE.bit.CHINTE = chinte;        	// 通道中断使能

	// 清除标志
	DmaRegs.CH5.CONTROL.bit.PERINTCLR = 1;  		// 清除被锁存的外部中断事件，并将PEIINTFLG清零
	DmaRegs.CH5.CONTROL.bit.SYNCCLR = 1;    		// 清除被锁存的同步事件，并将SYNCFLG清零
	DmaRegs.CH5.CONTROL.bit.ERRCLR = 1; 	     	// 清除同步错误标志位，并将OVRFLG清零

	// 初始化PIE向量
	PieCtrlRegs.PIEIER7.bit.INTx5 = 1;              // 使能DMA通道5的PIE级中断

	EDIS;
}

//---------------------------------------------------------------------------
// StartDMACH5:
//---------------------------------------------------------------------------
// DMA通道5启动函数
//
void StartDMACH5(void)
{
	EALLOW;
	DmaRegs.CH5.CONTROL.bit.RUN = 1;                 // 启动DMA通道5
	EDIS;
}


//---------------------------------------------------------------------------
// DMACH6AddrConfig:
//---------------------------------------------------------------------------
// DMA通道6传送地址配置函数
//
void DMACH6AddrConfig(volatile Uint16 *DMA_Dest,volatile Uint16 *DMA_Source)
{
	EALLOW;

	// 设置DMA源地址
	DmaRegs.CH6.SRC_BEG_ADDR_SHADOW = (Uint32)DMA_Source;	// 指向源的起始地址
	DmaRegs.CH6.SRC_ADDR_SHADOW =     (Uint32)DMA_Source;

	// 设置DMA目标地址
	DmaRegs.CH6.DST_BEG_ADDR_SHADOW = (Uint32)DMA_Dest;	    // 指向目标的起始地址
	DmaRegs.CH6.DST_ADDR_SHADOW =     (Uint32)DMA_Dest;

	EDIS;
}

//---------------------------------------------------------------------------
// DMACH6BurstConfig:
//---------------------------------------------------------------------------
// DMA通道6突发配置函数
//
void DMACH6BurstConfig(Uint16 bsize,Uint16 srcbstep, int16 desbstep)
{
	EALLOW;

	// 配置突发寄存器
	DmaRegs.CH6.BURST_SIZE.all = bsize;	        // 配置一次突发传送的数据个数
	DmaRegs.CH6.SRC_BURST_STEP = srcbstep;		// 配置在一次突发传送结束后，源地址指针增加/减少的步长
	DmaRegs.CH6.DST_BURST_STEP = desbstep;      // 配置在一次突发传送结束后，目标地址指针增加/减少的步长


	EDIS;
}

//---------------------------------------------------------------------------
// DMACH6TransferConfig:
//---------------------------------------------------------------------------
// DMA通道6外环传送配置函数
//
void DMACH6TransferConfig(Uint16 tsize, int16 srctstep, int16 deststep)
{
	EALLOW;

	// 配置外环传送寄存器
	DmaRegs.CH6.TRANSFER_SIZE = tsize;          // 配置在一次传送过程中突发传送的次数
	DmaRegs.CH6.SRC_TRANSFER_STEP = srctstep;	// 配置在一次突发传送结束后，源地址指针增加/减少的步长
	DmaRegs.CH6.DST_TRANSFER_STEP = deststep;	// 配置在一次突发传送结束后，目标地址指针增加/减少的步长

	EDIS;
}

//---------------------------------------------------------------------------
// DMACH6WrapConfig:
//---------------------------------------------------------------------------
// DMA通道6返回配置函数
//
void DMACH6WrapConfig(Uint16 srcwsize, int16 srcwstep, Uint16 deswsize, int16 deswstep)
{
	EALLOW;

	// 配置返回寄存器
	DmaRegs.CH6.SRC_WRAP_SIZE = srcwsize;	    // 配置在返回源的开始地址指针前突发传送的次数
    DmaRegs.CH6.SRC_WRAP_STEP = srcwstep;		// 配置在返回寄存器归零后，源地址指针增加/减少的步长

	DmaRegs.CH6.DST_WRAP_SIZE = deswsize;		// 配置在返回目标的开始地址指针前突发传送的次数
	DmaRegs.CH6.DST_WRAP_STEP = deswstep;		// 配置在返回寄存器归零后，目标地址指针增加/减少的步长

	EDIS;
}

//---------------------------------------------------------------------------
// DMACH6ModeConfig:
//---------------------------------------------------------------------------
// DMA通道6模式配置函数
//
void DMACH6ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte)
{
	EALLOW;

	// 配置模式寄存器
	DmaRegs.CH6.MODE.bit.PERINTSEL = persel;	    // 将外设中断源传送给DMA通道6
	DmaRegs.CH6.MODE.bit.PERINTE = perinte;       	// 使能外设中断触发信号
	DmaRegs.CH6.MODE.bit.ONESHOT = oneshot;       	// 使能单次触发传送
	DmaRegs.CH6.MODE.bit.CONTINUOUS = cont;    		// 使能连续传送
	DmaRegs.CH6.MODE.bit.SYNCE = synce;         	// 外设同步信号使能
	DmaRegs.CH6.MODE.bit.SYNCSEL = syncsel;       	// 同步模式选择
	DmaRegs.CH6.MODE.bit.OVRINTE = ovrinte;         // 溢出中断使能
	DmaRegs.CH6.MODE.bit.DATASIZE = datasize;      	// 选择DMA通道的数据宽度
	DmaRegs.CH6.MODE.bit.CHINTMODE = chintmode;		// 选择通道的中断模式（即在一次传送的开始还是结束时产生中断）
	DmaRegs.CH6.MODE.bit.CHINTE = chinte;        	// 通道中断使能

	// Clear any spurious flags:
	DmaRegs.CH6.CONTROL.bit.PERINTCLR = 1;  		// 清除被锁存的外部中断事件，并将PEIINTFLG清零
	DmaRegs.CH6.CONTROL.bit.SYNCCLR = 1;    		// 清除被锁存的同步事件，并将SYNCFLG清零
	DmaRegs.CH6.CONTROL.bit.ERRCLR = 1; 	     	// 清除同步错误标志位，并将OVRFLG清零

	// Initialize PIE vector for CPU interrupt:
	PieCtrlRegs.PIEIER7.bit.INTx6 = 1;              // 使能DMA通道6的PIE级中断

	EDIS;
}

//---------------------------------------------------------------------------
// StartDMACH6:
//---------------------------------------------------------------------------
// DMA通道6启动函数
//
void StartDMACH6(void)
{
	EALLOW;
	DmaRegs.CH6.CONTROL.bit.RUN = 1;                // 启动DMA通道6
	EDIS;
}

//===========================================================================
// No more.
//===========================================================================
