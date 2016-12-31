#pragma once
#include "datatype.h"

class CMatrix3x3
{
public:
	CMatrix3x3(void);
	~CMatrix3x3(void);
public:
	float M[3][3];
public:
	CMatrix3x3(CMatrix3x3& m);
	CMatrix3x3(float r11, float r12, float r13, float r21, float r22, float r23, float r31, float r32, float r33);
	CMatrix3x3& operator=(CMatrix3x3& m);
	CMatrix3x3 operator*(CMatrix3x3& m);
};
