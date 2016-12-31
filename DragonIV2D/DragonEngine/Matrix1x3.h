#pragma once
#include "datatype.h"
#include "Matrix3x3.h"

class CMatrix1x3
{
public:
	CMatrix1x3(void);
	~CMatrix1x3(void);
public:
	float M[3];
public:
	CMatrix1x3(float x, float y, float z);
	CMatrix1x3(CMatrix1x3& m);
	CMatrix1x3& operator=(CMatrix1x3& m);
	CMatrix1x3 operator*(CMatrix3x3& m);
};
