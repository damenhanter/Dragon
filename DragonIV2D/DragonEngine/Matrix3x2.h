#pragma once
#include "datatype.h"

class CMatrix3x2
{
public:
	CMatrix3x2(void);
	~CMatrix3x2(void);
public:
	float M[3][2];
public:
	CMatrix3x2(CMatrix3x2& m);
	CMatrix3x2& operator=(CMatrix3x2& m);
	CMatrix3x2(float r11, float r12, float r21, float r22, float r31, float r32);
};
