/****************************************************************
文件功能：和电机控制相关的程序文件
文件版本：
最新更新：
	
****************************************************************/
#include "MotorInclude.h"

/************************************************************
************************************************************/

/*******************************************************************
Date Type Q12(保证幅值不变的坐标变换)
	Alph= U * (3/2)^0.5 
	Beta= 2^0.5/2 * (U + 2*V) = 2^0.5/2 * (V - W)
********************************************************************/
void UVWToAlphBetaAxes(UVW_STRUCT * uvw, ALPHABETA_STRUCT * AlphBeta)
{
	long m_Long;

	//m_Long = ((long)(uvw->U) * 20066L)>>14;	
	m_Long = ((long)(uvw->U) * 23170L)>>15;	
	AlphBeta->Alph = __IQsat(m_Long,32767,-32767);

	//m_Long = (((long)uvw->V - (long)uvw->W) * 23170L)>>15;
	m_Long = (((long)uvw->V - (long)uvw->W) * 13377L)>>15;
	AlphBeta->Beta = __IQsat(m_Long,32767,-32767);
}

/*******************************************************************
Date Type Q12 （q轴超d轴90度）
	d= cos(theta)*alph + sin(theta)*beta;
	q= -sin(theta)*alph + cos(theta)*beta;
********************************************************************/
void AlphBetaToDQ(ALPHABETA_STRUCT * AlphBeta, int angle, MT_STRUCT * MT)
{
	int m_sin,m_cos;
	long m_Long;

	m_sin  = qsin(angle);
	m_cos  = qsin(16384 - angle);
	m_Long = ( ((long)m_cos * (long)(AlphBeta->Alph)) + ((long)m_sin * (long)(AlphBeta->Beta)))>>15;
	MT->M  = __IQsat(m_Long,32767,-32767);
	m_Long = (-((long)m_sin * (long)(AlphBeta->Alph)) + ((long)m_cos * (long)(AlphBeta->Beta)))>>15;
	MT->T  = __IQsat(m_Long,32767,-32767);
}

/*******************************************************************
Date Type Q13
	A= (d*d + q*q)^0.5
	q= atan(q/d)
********************************************************************/
void DQToAmpTheta(MT_STRUCT * MT,AMPTHETA_STRUCT * AmpTheta)
{
	long m_Input;

	m_Input = (((long)MT->M * (long)MT->M) + ((long)MT->T * (long)MT->T));
	AmpTheta->Amp = (Uint)qsqrt(m_Input);

	AmpTheta->Theta = atan(MT->M,MT->T);
}

