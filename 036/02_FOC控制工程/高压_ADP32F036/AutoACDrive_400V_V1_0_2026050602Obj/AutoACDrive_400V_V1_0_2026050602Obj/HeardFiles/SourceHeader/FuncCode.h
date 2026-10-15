
#ifndef __F_FUNCCODE_H__
#define __F_FUNCCODE_H__


#include <HeardFiles/ADP32F03xHeader/ADP32F03x_Device.h>
#include "Debug.h"


// 请不要随意修改这两个值，否则EEPROM中的值会全部(包括FF组、FP组、各种记录)恢复出厂值。
#define EEPROM_CHECK_WORD1     (0x0008)                         // EEPROM校验字1
#define EEPROM_CHECK_WORD2     (0xFFFF - EEPROM_CHECK_WORD1)    // EEPROM校验字2


// 加减速方式
#define FUNCCODE_accDecSpdCurve_LINE        0   // 直线加减速
#define FUNCCODE_accDecSpdCurve_S_CURVE_A   1   // S曲线1，普通二次方
#define FUNCCODE_accDecSpdCurve_S_CURVE_B   2   // S曲线2，参考三菱S曲线B

#define TIME_UNIT_ACC_DEC_SPEED         10  // 加减速时间单位, ms


//=====================================================================
// (电机)控制方式
#define FUNCCODE_motorCtrlMode_SVC  0   // SVC
#define FUNCCODE_motorCtrlMode_VC   1   // VC
#define FUNCCODE_motorCtrlMode_VF   2   // VF


#endif  // __F_FUNCCODE_H__
