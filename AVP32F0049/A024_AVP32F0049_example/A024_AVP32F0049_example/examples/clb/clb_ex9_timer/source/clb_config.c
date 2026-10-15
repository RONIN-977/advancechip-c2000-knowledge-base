/*
 *  ======== clb.c ========
 *
 */

#include "driverlib.h"
#include "device.h"
#include "clb_config.h"
#include "clb.h"


const uint32_t TILE1_HLC_initFIFOData[4] = {TILE1_HLC_FIFO0_INIT, TILE1_HLC_FIFO1_INIT, TILE1_HLC_FIFO2_INIT, TILE1_HLC_FIFO3_INIT};

uint16_t TILE1HLCInstr[CLB_NUM_HLC_INSTR + 1] =
{
    TILE1_HLCINSTR_0,
    TILE1_HLCINSTR_1,
    TILE1_HLCINSTR_2,
    TILE1_HLCINSTR_3,
    TILE1_HLCINSTR_4,
    TILE1_HLCINSTR_5,
    TILE1_HLCINSTR_6,
    TILE1_HLCINSTR_7,
    TILE1_HLCINSTR_8,
    TILE1_HLCINSTR_9,
    TILE1_HLCINSTR_10,
    TILE1_HLCINSTR_11,
    TILE1_HLCINSTR_12,
    TILE1_HLCINSTR_13,
    TILE1_HLCINSTR_14,
    TILE1_HLCINSTR_15,
    TILE1_HLCINSTR_16,
    TILE1_HLCINSTR_17,
    TILE1_HLCINSTR_18,
    TILE1_HLCINSTR_19,
    TILE1_HLCINSTR_20,
    TILE1_HLCINSTR_21,
    TILE1_HLCINSTR_22,
    TILE1_HLCINSTR_23,
    TILE1_HLCINSTR_24,
    TILE1_HLCINSTR_25,
    TILE1_HLCINSTR_26,
    TILE1_HLCINSTR_27,
    TILE1_HLCINSTR_28,
    TILE1_HLCINSTR_29,
    TILE1_HLCINSTR_30,
    TILE1_HLCINSTR_31
};



void initTILE1(uint32_t base)
{
    uint16_t i;
    //
    //  Output LUT
    //
    CLB_configOutputLUT(base, CLB_OUT0, TILE1_CFG_OUTLUT_0);

    CLB_configOutputLUT(base, CLB_OUT1, TILE1_CFG_OUTLUT_1);

    CLB_configOutputLUT(base, CLB_OUT2, TILE1_CFG_OUTLUT_2);

    CLB_configOutputLUT(base, CLB_OUT3, TILE1_CFG_OUTLUT_3);

    //
    //  Equation for Output Look-Up Table Block 4 for TILE1: i0
    //
    CLB_configOutputLUT(base, CLB_OUT4, TILE1_CFG_OUTLUT_4);

    CLB_configOutputLUT(base, CLB_OUT5, TILE1_CFG_OUTLUT_5);
    
    CLB_configOutputLUT(base, CLB_OUT6, TILE1_CFG_OUTLUT_6);

    CLB_configOutputLUT(base, CLB_OUT7, TILE1_CFG_OUTLUT_7);

    //
    //  AOC
    //
    CLB_configAOC(base, CLB_AOC0, TILE1_OUTPUT_COND_CTR_0);
    CLB_configAOC(base, CLB_AOC1, TILE1_OUTPUT_COND_CTR_1);
    CLB_configAOC(base, CLB_AOC2, TILE1_OUTPUT_COND_CTR_2);
    CLB_configAOC(base, CLB_AOC3, TILE1_OUTPUT_COND_CTR_3);
    CLB_configAOC(base, CLB_AOC4, TILE1_OUTPUT_COND_CTR_4);
    CLB_configAOC(base, CLB_AOC5, TILE1_OUTPUT_COND_CTR_5);
    CLB_configAOC(base, CLB_AOC6, TILE1_OUTPUT_COND_CTR_6);
    CLB_configAOC(base, CLB_AOC7, TILE1_OUTPUT_COND_CTR_7);

    //
    // LUT 0 - 2 are configured as macros in clb_config.h; these macros are used in
    // CLB_selectLUT4Inputs and CLB_configLUT4Function
    //

    //
    //  LUT Configuration
    //
    CLB_selectLUT4Inputs(base, TILE1_CFG_LUT4_IN0, TILE1_CFG_LUT4_IN1, TILE1_CFG_LUT4_IN2, TILE1_CFG_LUT4_IN3);
    CLB_configLUT4Function(base, TILE1_CFG_LUT4_FN10, TILE1_CFG_LUT4_FN2);

    //
    // FSM 0 - 2 are configured in <file>
    //

    //
    //  FSM
    //
    CLB_selectFSMInputs(base, TILE1_CFG_FSM_EXT_IN0, TILE1_CFG_FSM_EXT_IN1, TILE1_CFG_FSM_EXTRA_IN0, TILE1_CFG_FSM_EXTRA_IN1);
    CLB_configFSMNextState(base, TILE1_CFG_FSM_NEXT_STATE_0, TILE1_CFG_FSM_NEXT_STATE_1, TILE1_CFG_FSM_NEXT_STATE_2);
    CLB_configFSMLUTFunction(base, TILE1_CFG_FSM_LUT_FN10, TILE1_CFG_FSM_LUT_FN2);

    //
    // Counter 0 - 2 are configured in <file>
    //

    //
    //  Counters
    //
    CLB_selectCounterInputs(base, TILE1_CFG_COUNTER_RESET, TILE1_CFG_COUNTER_EVENT, TILE1_CFG_COUNTER_MODE_0, TILE1_CFG_COUNTER_MODE_1);
    CLB_configMiscCtrlModes(base, TILE1_CFG_MISC_CONTROL);
    CLB_configCounterLoadMatch(base, CLB_CTR0, TILE1_COUNTER_0_LOAD_VAL, TILE1_COUNTER_0_MATCH1_VAL, TILE1_COUNTER_0_MATCH2_VAL);
    CLB_configCounterLoadMatch(base, CLB_CTR1, TILE1_COUNTER_1_LOAD_VAL, TILE1_COUNTER_1_MATCH1_VAL, TILE1_COUNTER_1_MATCH2_VAL);
    CLB_configCounterLoadMatch(base, CLB_CTR2, TILE1_COUNTER_2_LOAD_VAL, TILE1_COUNTER_2_MATCH1_VAL, TILE1_COUNTER_2_MATCH2_VAL);
    CLB_configCounterTapSelects(base, TILE1_CFG_TAP_SEL);

    //
    // HLC is configured in <file>
    //
    //  User Description for the High Level Controller for TILE1
    /*
        
    */

    //
    // HLC
    //
    CLB_configHLCEventSelect(base, TILE1_HLC_EVENT_SEL);
    CLB_setHLCRegisters(base, TILE1_HLC_R0_INIT, TILE1_HLC_R1_INIT, TILE1_HLC_R2_INIT, TILE1_HLC_R3_INIT);
    CLB_writeFIFOs(base, TILE1_HLC_initFIFOData);

    for(i = 0; i <= CLB_NUM_HLC_INSTR; i++)
    {
        CLB_programHLCInstruction(base, i, TILE1HLCInstr[i]);
    }
}
