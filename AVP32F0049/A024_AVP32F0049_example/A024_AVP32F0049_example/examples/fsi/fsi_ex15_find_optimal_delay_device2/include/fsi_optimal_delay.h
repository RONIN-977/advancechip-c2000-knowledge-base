/******************************* Copyright(C) *********************************************
*                           AVP32F0049_Example_V1.01
*                         深圳市进芯微电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  fsi_optimal_delay.h
*                         TITLE: FSI Optimal Delay
*******************************************************************************************/

#ifndef FSI_OPTIMAL_DELAY_H
#define FSI_OPTIMAL_DELAY_H

#include "device.h"

//! \brief The structure containing the FSI Execution Point
//!
//! \details The delay values for RX0, RX1 and CLK lines.
//!
typedef struct FSIExecutionPoint{
    uint16_t RX0Delay;
    uint16_t RX1Delay;
    uint16_t CLKDelay;
    bool Valid;
}FSIExecutionPoint;

typedef struct XYPoint{
    int16_t X;
    int16_t Y;
}XYPoint;


//*****************************************************************************
//
//! \brief Calibrates the FSIRX module using nLanes data width
//!
//! \details Sets up the FSIRX module to listern for traffic in order to 
//!          calibrate the delay elements and find the optimal 
//!          FSI execution point. Use this function with 
//!          FSI_transmitToCalibrate.
//!
//! \param[in] rxbase is the FSI Rx module base address
//! \param[in] txbase is the FSI Tx module base address
//! \param[in] nLanes is the FSI Rx and Tx module data width for communication
//! \param[in] reliableTXPrescalar is the FSI Tx clock prescalar that
//!            that gurauntees successful communication
//!
//! \return None.
//
//*****************************************************************************
FSIExecutionPoint FSI_calibrateExecutionPoint(uint32_t rxbase, uint32_t txbase, FSI_DataWidth nLanes,
                                              uint16_t reliableTXPrescalar);


//*****************************************************************************
//
//! \brief Transmits frames to calibrates the FSIRX module of the receiving 
//!        device
//!
//! \details Sets up the FSITX module to transmit frames to 
//!          calibrate the delay elements and find the optimal 
//!          FSI execution point of the receiving FSIRX module. 
//!          Use this function with FSI_calibrateExecutionPoint.
//!
//! \param[in] rxbase is the FSI Rx module base address
//! \param[in] txbase is the FSI Tx module base address
//! \param[in] txPrescalar is the FSI Tx clock prescalar that
//!            that is used for fast communication and requires FSIRX module
//!            to calibrate its delay lines
//!
//! \return None.
//
//*****************************************************************************
void FSI_transmitToCalibrate(uint32_t rxbase, uint32_t txbase, uint16_t txPrescalar);

//*****************************************************************************
//
//! \brief Sets the FSITX clock prescalar
//!
//! \param[in] base is the FSI Tx module base address
//!
//! \return None.
//
//*****************************************************************************
static inline void FSI_setTxPrescale(uint32_t base, uint16_t preScaleValue)
{
    //
    // Check the arguments.
    //
    ASSERT(FSI_isTxBaseValid(base));

    HWREGH(base + FSI_O_TX_CLK_CTRL) = (HWREGH(base + FSI_O_TX_CLK_CTRL) &
           (~FSI_TX_CLK_CTRL_PRESCALE_VAL_M)) |
           (preScaleValue <<
           FSI_TX_CLK_CTRL_PRESCALE_VAL_S);

    return;
}

//*****************************************************************************
//
//! \brief Returns the current FSITX clock prescalar
//!
//! \param[in] base is the FSI Tx module base address
//!
//! \return the current FSITX clock prescalar
//
//*****************************************************************************
static inline uint16_t FSI_getTxPrescale(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(FSI_isTxBaseValid(base));


    return ((HWREGH(base + FSI_O_TX_CLK_CTRL) &
            (FSI_TX_CLK_CTRL_PRESCALE_VAL_M)) >>
            FSI_TX_CLK_CTRL_PRESCALE_VAL_S);

}

//*****************************************************************************
//
//! \brief Returns the current FSITX Frame Tag
//!
//! \param[in] base is the FSI Tx module base address
//!
//! \return the current FSITX Frame Tag
//
//*****************************************************************************
static inline FSI_FrameTag FSI_getTxFrameTag(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(FSI_isTxBaseValid(base));

    return (FSI_FrameTag)(HWREGH(base + FSI_O_TX_FRAME_TAG_UDATA) &
                          (FSI_TX_FRAME_TAG_UDATA_FRAME_TAG_M));
}


//*****************************************************************************
//
//! \brief Returns the current FSITX Frame Type
//!
//! \param[in] base is the FSI Tx module base address
//!
//! \return the current FSITX Frame Type
//
//*****************************************************************************
static inline FSI_FrameType
FSI_getTxFrameType(uint32_t base)
{
    //
    // Check the arguments.
    //
    ASSERT(FSI_isTxBaseValid(base));

    return (FSI_FrameType)(HWREGH(base + FSI_O_TX_FRAME_CTRL) &
                                 (FSI_TX_FRAME_CTRL_FRAME_TYPE_M));
}


//*****************************************************************************
//
//! \brief Returns the current FSITX Interrupt event that has occured
//!
//! \param[in] base is the FSI Tx module base address
//! \param[in] intNum is the type of interrupt to be generated
//!                    interrupt1 or interrupt2
//!
//! \return the current FSITX Interrupt event that has occured
//
//*****************************************************************************
static inline uint16_t FSI_getTxInterrupt(uint32_t base, FSI_InterruptNum intNum)
{
    uint16_t intStatus = 0;

    //
    // Check the arguments.
    //
    ASSERT(FSI_isTxBaseValid(base));

    EALLOW;

    if(intNum == FSI_INT1)
    {
        intStatus = (HWREGH(base + FSI_O_TX_INT_CTRL) & (FSI_TX_EVTMASK));
    }
    else
    {
        intStatus = (HWREGH(base + FSI_O_TX_INT_CTRL) >> FSI_TX_INT2_CTRL_S) & (FSI_TX_EVTMASK);
    }

    EDIS;

    return intStatus;
}

//*****************************************************************************
//
//! \brief Returns the current FSITX Interrupt event that has occured
//!
//! \param[in] base is the FSI Rx module base address
//! \param[in] intNum is the type of interrupt to be generated
//!                    interrupt1 or interrupt2
//!
//! \return the current FSIRX Interrupt event that has occured
//
//*****************************************************************************
static inline uint16_t
FSI_getRxInterrupt(uint32_t base, FSI_InterruptNum intNum)
{
    uint16_t intStatus = 0;

    //
    // Check the arguments.
    //
    ASSERT(FSI_isRxBaseValid(base));

    EALLOW;

    if(intNum == FSI_INT1)
    {
        intStatus = (HWREGH(base + FSI_O_RX_INT1_CTRL) & FSI_RX_EVTMASK);
    }
    else
    {
        intStatus = (HWREGH(base + FSI_O_RX_INT2_CTRL) & FSI_RX_EVTMASK);
    }

    EDIS;

    return intStatus;
}
#endif
