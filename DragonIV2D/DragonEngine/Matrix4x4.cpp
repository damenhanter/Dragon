#include "stdafx.h"
#include"Matrix4x4.h"

CMatrix4x4::CMatrix4x4(const CMatrix4x4 &m)
{
	matrix[0][0]  = m.matrix[0][0];
	matrix[0][1]  = m.matrix[0][1];
	matrix[0][2]  = m.matrix[0][2];
	matrix[0][3]  = m.matrix[0][3];

	matrix[1][0]  = m.matrix[1][0];
	matrix[1][1]  = m.matrix[1][1];
	matrix[1][2]  = m.matrix[1][2];
	matrix[1][3]  = m.matrix[1][3];

	matrix[2][0]  = m.matrix[2][0];
	matrix[2][1]  = m.matrix[2][1];
	matrix[2][2]  = m.matrix[2][2];
	matrix[2][3]  = m.matrix[2][3];

	matrix[3][0]  = m.matrix[3][0];
	matrix[3][1]  = m.matrix[3][1];
	matrix[3][2]  = m.matrix[3][2];
	matrix[3][3]  = m.matrix[3][3];
}


CMatrix4x4::CMatrix4x4(float r11, float r12, float r13, float r14,
					   float r21, float r22, float r23, float r24,
					   float r31, float r32, float r33, float r34,
					   float r41, float r42, float r43, float r44)
{
	matrix[0][0]  = r11; matrix[0][1]  = r12; matrix[0][2]  = r13;matrix[0][3] = r14;
	matrix[1][0]  = r21; matrix[1][1]  = r22; matrix[1][2]  = r23;matrix[1][3] = r24;
	matrix[2][0]  = r31; matrix[2][1]  = r32; matrix[2][2]  = r33;matrix[2][3] = r34;
	matrix[3][0]  = r41; matrix[3][1]  = r42; matrix[3][2]  = r43;matrix[3][3] = r44;
}


void CMatrix4x4::Identity()
{
	matrix[0][0]  = 1.0f; matrix[0][1]  = 0.0f; matrix[0][2]  = 0.0f;matrix[0][3] = 0.0f;
	matrix[1][0]  = 0.0f; matrix[1][1]  = 1.0f; matrix[1][2]  = 0.0f;matrix[1][3] = 0.0f;
	matrix[2][0]  = 0.0f; matrix[2][1]  = 0.0f; matrix[2][2]  = 1.0f;matrix[2][3] = 0.0f;
	matrix[3][0]  = 0.0f; matrix[3][1]  = 0.0f; matrix[3][2]  = 0.0f;matrix[3][3] = 1.0f;
}


void CMatrix4x4::operator =(CMatrix4x4 &m)
{
	matrix[0][0]  = m.matrix[0][0];
	matrix[0][1]  = m.matrix[0][1];
	matrix[0][2]  = m.matrix[0][2];
	matrix[0][3]  = m.matrix[0][3];

	matrix[1][0]  = m.matrix[1][0];
	matrix[1][1]  = m.matrix[1][1];
	matrix[1][2]  = m.matrix[1][2];
	matrix[1][3]  = m.matrix[1][3];

	matrix[2][0]  = m.matrix[2][0];
	matrix[2][1]  = m.matrix[2][1];
	matrix[2][2]  = m.matrix[2][2];
	matrix[2][3]  = m.matrix[2][3];

	matrix[3][0]  = m.matrix[3][0];
	matrix[3][1]  = m.matrix[3][1];
	matrix[3][2]  = m.matrix[3][2];
	matrix[3][3]  = m.matrix[3][3];
}


CMatrix4x4 CMatrix4x4::operator -(CMatrix4x4 &m)
{
	return CMatrix4x4(matrix[0][0] - m.matrix[0][0], matrix[0][1] - m.matrix[0][1], matrix[0][2] - m.matrix[0][2],
		matrix[0][3] - m.matrix[0][3], matrix[1][0] - m.matrix[1][0], matrix[1][1] - m.matrix[1][1],
		matrix[1][2] - m.matrix[1][2], matrix[1][3] - m.matrix[1][3], matrix[2][0] - m.matrix[2][0],
		matrix[2][1] - m.matrix[2][1], matrix[2][2] - m.matrix[2][2], matrix[2][3] - m.matrix[2][3],
		matrix[3][0] - m.matrix[3][0], matrix[3][1] - m.matrix[3][1],
		matrix[3][2] - m.matrix[3][2], matrix[3][3] - m.matrix[3][3]);
}


CMatrix4x4 CMatrix4x4::operator +(CMatrix4x4 &m)
{
	return CMatrix4x4(matrix[0][0] + m.matrix[0][0], matrix[0][1] + m.matrix[0][1], matrix[0][2] + m.matrix[0][2],
		matrix[0][3] + m.matrix[0][3], matrix[1][0] + m.matrix[1][0], matrix[1][1] + m.matrix[1][1],
		matrix[1][2] + m.matrix[1][2], matrix[1][3] + m.matrix[1][3], matrix[2][0] + m.matrix[2][0],
		matrix[2][1] + m.matrix[2][1], matrix[2][2] + m.matrix[2][2], matrix[2][3] + m.matrix[2][3],
		matrix[3][0] + m.matrix[3][0], matrix[3][1] + m.matrix[3][1],
		matrix[3][2] + m.matrix[3][2], matrix[3][3] + m.matrix[3][3]);
}


CMatrix4x4 CMatrix4x4::operator *(CMatrix4x4 &m)
{
	int i,j,k;
	CMatrix4x4 mat; 
	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			mat.matrix[i][j]=0;
			for(k=0; k<4; k++)
			{
				mat.matrix[i][j]=mat.matrix[i][j]+matrix[i][k]*m.matrix[k][j];
			}
		}
	}
	return mat;
}


CMatrix4x4 CMatrix4x4::operator *(float f)
{
	return CMatrix4x4(matrix[0][0]  * f, matrix[0][1]  * f, matrix[0][2]  * f, matrix[0][3]  * f,
		matrix[1][0]  * f, matrix[1][1]  * f, matrix[1][2]  * f, matrix[1][3]  * f,
		matrix[2][0]  * f, matrix[2][1]  * f, matrix[2][2]  * f, matrix[2][3]  * f,
		matrix[3][0]  * f, matrix[3][1]  * f, matrix[3][2]  * f, matrix[3][3]  * f);
}


CMatrix4x4 CMatrix4x4::operator /(float f)
{
	if(f == 0) f = 1;
	f = 1/f;

	return CMatrix4x4(matrix[0][0]  * f, matrix[0][1]  * f, matrix[0][2]  * f, matrix[0][3]  * f,
		matrix[1][0]  * f, matrix[1][1]  * f, matrix[1][2]  * f, matrix[1][3]  * f,
		matrix[2][0]  * f, matrix[2][1]  * f, matrix[2][2]  * f, matrix[2][3]  * f,
		matrix[3][0]  * f, matrix[3][1]  * f, matrix[3][2]  * f, matrix[3][3]  * f);
}


void CMatrix4x4::operator +=(CMatrix4x4 &m)
{
	(*this) = (*this) + m;
}


void CMatrix4x4::operator -=(CMatrix4x4 &m)
{
	(*this) = (*this) - m;
}


void CMatrix4x4::operator *=(CMatrix4x4 &m)
{
	(*this) = (*this) * m;
}


void CMatrix4x4::operator *=(float f)
{
	(*this) = (*this) * f;
}


void CMatrix4x4::operator /=(float f)
{
	(*this) = (*this) / f;
}


void CMatrix4x4::Translate(CVector3 &Translate)
{
	matrix[3][0] = Translate.x;
	matrix[3][1] = Translate.y;
	matrix[3][2] = Translate.z;
	matrix[3][3] = 1.0f;
}


void CMatrix4x4::Translate(float x, float y, float z)
{
	matrix[3][0] = x;
	matrix[3][1] = y;
	matrix[3][2] = z;
	matrix[3][3] = 1.0f;
}


void CMatrix4x4::inverseTranslate()
{
	matrix[3][0] = -matrix[3][0];
	matrix[3][1] = -matrix[3][1];
	matrix[3][2] = -matrix[3][2];
}

CVector3 CMatrix4x4::VectorMatrixMultiply(CVector3 &v)
{
	CVector3 out;

	out.x = (v.x * matrix[0][0]) + (v.y * matrix[1][0]) + (v.z * matrix[2][0]) + matrix[3][0];
	out.y = (v.x * matrix[0][1]) + (v.y * matrix[1][1]) + (v.z * matrix[2][1]) + matrix[3][1];
	out.z = (v.x * matrix[0][2]) + (v.y * matrix[1][2]) + (v.z * matrix[2][2]) + matrix[3][2];

	return out;
}


CVector3 CMatrix4x4::VectorMatrixMultiply3x3(CVector3 &v)
{
	CVector3 out;

	out.x = (v.x * matrix[0][0]) + (v.y * matrix[1][0]) + (v.z * matrix[2][0]);
	out.y = (v.x * matrix[0][1]) + (v.y * matrix[1][1]) + (v.z * matrix[2][1]);
	out.z = (v.x * matrix[0][2]) + (v.y * matrix[1][2]) + (v.z * matrix[2][2]);

	return out;
}