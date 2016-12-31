#include "StdAfx.h"
#include "Matrix1x2.h"
#include "Matrix3x2.h"


CMatrix1x2::CMatrix1x2(void)
{
	M[0]=M[1]=0.0f;
}

CMatrix1x2::~CMatrix1x2(void)
{
}

CMatrix1x2::CMatrix1x2(float x, float y)
{
	M[0]=x;
	M[1]=y;
}

CMatrix1x2::CMatrix1x2(CMatrix1x2& m)
{
	M[0]=m.M[0];
	M[1]=m.M[1];
}

CMatrix1x2& CMatrix1x2::operator=(CMatrix1x2& m)
{
	//TODO: insert return statement here
	M[0]=m.M[0];
	M[1]=m.M[1];
	return *this;
}

CMatrix1x2 CMatrix1x2::operator*(CMatrix3x2& m)
{
	CMatrix1x2 mat;
	int j,k;
	for(j=0; j<2; j++)
	{
		for(k=0; k<2; k++)
		{
			mat.M[j]+=M[k]*m.M[k][j];
		}
	}
	return mat;
}
