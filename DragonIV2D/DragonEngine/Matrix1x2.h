#pragma once
#include "datatype.h"
#include "Matrix3x2.h"

class CMatrix1x2
{
public:
	CMatrix1x2(void);
	~CMatrix1x2(void);
public:
	float M[2];
public:
	CMatrix1x2(float x, float y);
	CMatrix1x2(CMatrix1x2& m);
	CMatrix1x2& operator=(CMatrix1x2& m);
	CMatrix1x2 operator*(CMatrix3x2& m);
};
