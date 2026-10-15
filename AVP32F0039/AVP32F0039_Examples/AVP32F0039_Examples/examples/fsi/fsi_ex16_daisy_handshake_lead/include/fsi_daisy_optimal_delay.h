/******************************* Copyright(C) *********************************************
*                          AVP32F0043_Example_V1.01
*                           湖南进芯电子科技有限公司
*                         http://www.advancechip.com/
*
*                         FILE:  fsi_daisy_optimal_delay.h
*                         TITLE: 
*******************************************************************************************/

#ifndef FSI_DAISY_OPTIMAL_DELAY_H_
#define FSI_DAISY_OPTIMAL_DELAY_H_

#include "device.h"
#include "fsi_optimal_delay.h"


//*****************************************************************************
//
//! \brief Forwards frames to next device in daisy-chain topology
//!
//! \details Handles the forwarding of frames to the next device in chain. Used
//!     when calibrating other devices in a daisy-chain topology. Will look for
//!     configured ping Tag to know when to stop forwarding and begin calibrating
//!     or transmittng.
//!
//! \param[in] rxbase is the FSI Rx module base address
//! \param[in] txbase is the FSI Tx module base address
//! \param[in] reliableTXPrescalar is the FSI Tx clock prescalar that
//!            that gurauntees successful communication
//! \param[in] CalibrateTag that when received will end the Pass Through sequence
//!
//! \return None.
//
//*****************************************************************************
void FSI_daisy_calibratePassThrough(uint32_t rxbase, uint32_t txbase,
                                   uint16_t txPrescalar, FSI_FrameTag CalibrateTag);

//*****************************************************************************
//
//! \brief Sends data packet with tag to indicate completion of event
//!
//! \details This function sends a 1-word data packet with a provided tag
//! to indicate devices in the signal chain the completion of a predefined event.
//!
//! \param[in] rxbase is the FSI Rx module base address
//! \param[in] txbase is the FSI Tx module base address
//! \param[in] reliableTXPrescalar is the FSI Tx clock prescalar that
//!            that gurauntees successful communication
//! \param[in] CalibrateTag to transmit for signaling next calibration device
//!
//! \return None.
//
//*****************************************************************************
void FSI_signalNextCalibrate(uint32_t rxbase, uint32_t txbase,
                             uint16_t txPrescalar, FSI_FrameTag CalibrateTag);


#endif /* FSI_DAISY_OPTIMAL_DELAY_H_ */
