#include "StdAfx.h"
#include "Matrix3x2.h"

CMatrix3x2::CMatrix3x2(void)
{
}

CMatrix3x2::~CMatrix3x2(void)
{
}

CMatrix3x2::CMatrix3x2(CMatrix3x2& m)
{
	M[0][0]=m.M[0][0];
	M[0][1]=m.M[0][1];
	M[1][0]=m.M[1][0];
	M[1][1]=m.M[1][1];
	M[2][0]=m.M[2][0];
	M[2][1]=m.M[2][1];
}

CMatrix3x2& CMatrix3x2::operator=(CMatrix3x2& m)
{
	//TODO: insert return statement here
	M[0][0]=m.M[0][0];
	M[0][1]=m.M[0][1];
	M[1][0]=m.M[1][0];
	M[1][1]=m.M[1][1];
	M[2][0]=m.M[2][0];
	M[2][1]=m.M[2][1];
	return *this;
}

CMatrix3x2::CMatrix3x2(float r11, float r12, float r21, float r22, float r31, float r32)
{
	M[0][0]=r11;
	M[0][1]=r12;
	M[1][0]=r21;
	M[1][1]=r22;
	M[2][0]=r31;
	M[2][1]=r32;
}
