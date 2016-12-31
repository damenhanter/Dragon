#include "StdAfx.h"
#include "Matrix3x3.h"

CMatrix3x3::CMatrix3x3(void)
{
	M[0][0]=0.0f;
	M[0][1]=0.0f;
	M[0][2]=0.0f;
	M[1][0]=0.0f;
	M[1][1]=0.0f;
	M[1][2]=0.0f;
	M[2][0]=0.0f;
	M[2][1]=0.0f;
	M[2][2]=0.0f;
}

CMatrix3x3::~CMatrix3x3(void)
{
}

CMatrix3x3::CMatrix3x3(CMatrix3x3& m)
{
	M[0][0]=m.M[0][0];
	M[0][1]=m.M[0][1];
	M[0][2]=m.M[0][2];
	M[1][0]=m.M[1][0];
	M[1][1]=m.M[1][1];
	M[1][2]=m.M[1][2];
	M[2][0]=m.M[2][0];
	M[2][1]=m.M[2][1];
	M[2][2]=m.M[2][2];
}

CMatrix3x3::CMatrix3x3(float r11, float r12, float r13, float r21, float r22, float r23,
					   float r31, float r32, float r33)
{
	M[0][0]=r11;
	M[0][1]=r12;
	M[0][2]=r13;
	M[1][0]=r21;
	M[1][1]=r22;
	M[1][2]=r23;
	M[2][0]=r31;
	M[2][1]=r32;
	M[2][2]=r33;
}

CMatrix3x3& CMatrix3x3::operator=(CMatrix3x3& m)
{
	//TODO: insert return statement here
	M[0][0]=m.M[0][0];
	M[0][1]=m.M[0][1];
	M[0][2]=m.M[0][2];
	M[1][0]=m.M[1][0];
	M[1][1]=m.M[1][1];
	M[1][2]=m.M[1][2];
	M[2][0]=m.M[2][0];
	M[2][1]=m.M[2][1];
	M[2][2]=m.M[2][2];
	return *this;
}

CMatrix3x3 CMatrix3x3::operator*(CMatrix3x3& m)
{
	//TODO: insert return statement here
	int i,j,k;
	CMatrix3x3 mat;
	for(i=0; i<3; i++)
	{
		for(j=0; j<3; j++)
		{
			for(k=0; k<3; k++)
			{
				mat.M[i][j]+=M[i][k]*m.M[k][j];
			}
		}
	}
	return mat;
}
