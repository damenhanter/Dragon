#include "StdAfx.h"
#include "Matrix1x3.h"
#include "Matrix3x3.h"

CMatrix1x3::CMatrix1x3(void)
{
	M[0]=M[1]=M[2]=0.0f;
}

CMatrix1x3::~CMatrix1x3(void)
{
}

CMatrix1x3::CMatrix1x3(float x, float y, float z)
{
	M[0]=x;
	M[1]=y;
	M[2]=z;
}

CMatrix1x3::CMatrix1x3(CMatrix1x3& m)
{
	M[0]=m.M[0];
	M[1]=m.M[1];
	M[2]=m.M[3];
}

CMatrix1x3& CMatrix1x3::operator=(CMatrix1x3& m)
{
	//TODO: insert return statement here
	M[0]=m.M[0];
	M[1]=m.M[1];
	M[2]=m.M[3];
	return *this;
}

CMatrix1x3 CMatrix1x3::operator*(CMatrix3x3& m)
{
	//TODO: insert return statement here
	int j,k;
	CMatrix1x3 mat;
	for(j=0; j<3; j++)
	{
		for(k=0; k<3; k++)
		{
			mat.M[j]+=M[k]*m.M[k][j];
		}
	}
	return mat;
}
