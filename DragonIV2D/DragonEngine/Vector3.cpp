#include "stdafx.h"
#include"Vector3.h"

CVector3::CVector3()
{
   x = y = z = 0.0;
}


CVector3::CVector3(float X, float Y, float Z)
{
   x = X;
   y = Y;
   z = Z;
}


void CVector3::operator =(CVector3 &v)
{
   x = v.x;
   y = v.y;
   z = v.z;
}


CVector3 CVector3::operator -(CVector3 &v)
{
   return CVector3(x - v.x, y - v.y, z - v.z);
}


CVector3 CVector3::operator +(CVector3 &v)
{
   return CVector3(x + v.x, y + v.y, z + v.z);
}


CVector3 CVector3::operator *(CVector3 &v)
{
   return CVector3(x * v.x, y * v.y, z * v.z);
}


CVector3 CVector3::operator /(CVector3 &v)
{
   return CVector3(x / v.x, y / v.y, z / v.z);
}


CVector3 CVector3::operator +(float f)
{
   return CVector3(x + f, y + f, z + f);
}


CVector3 CVector3::operator -(float f)
{
   return CVector3(x - f, y - f, z - f);
}


CVector3 CVector3::operator *(float f)
{
   return CVector3(x * f, y * f, z * f);
}


CVector3 CVector3::operator /(float f)
{
   f = 1/f;

   return CVector3(x * f, y * f, z * f);
}


void CVector3::operator +=(CVector3 &v)
{
   x += v.x;
   y += v.y;
   z += v.z;
}


void CVector3::operator -=(CVector3 &v)
{
   x -= v.x;
   y -= v.y;
   z -= v.z;
}


void CVector3::operator *=(CVector3 &v)
{
   x *= v.x;
   y *= v.y;
   z *= v.z;
}


void CVector3::operator /=(CVector3 &v)
{
   x /= v.x;
   y /= v.y;
   z /= v.z;
}


void CVector3::operator +=(float f)
{
   x += f;
   y += f;
   z += f;
}


void CVector3::operator -=(float f)
{
   x -= f;
   y -= f;
   z -= f;
}


void CVector3::operator *=(float f)
{
   x *= f;
   y *= f;
   z *= f;
}


void CVector3::operator /=(float f)
{
   f = 1/f;

   x *= f;
   y *= f;
   z *= f;
}


void CVector3::CrossProduct(CVector3 &v1, CVector3 &v2)
{
   x = ((v1.y * v2.z) - (v1.z * v2.y));
   y = ((v1.z * v2.x) - (v1.x * v2.z));
   z = ((v1.x * v2.y) - (v1.y * v2.x));
}


float CVector3::DotProduct3(CVector3 &v1)
{
   return x * v1.x + y * v1.y + z * v1.z;
}

/*浮点数乘法占用1~3个CPU时钟周期
sqrt库函数占用70个CPU时钟周期，所以这里用泰勒公式的麦克劳林级数近似计算
误差为8%
*/
float CVector3::GetLength()
{
   int X,Y,Z;
   //确保它们是正值
   X=(int)fabs(x)*1024;
   Y=(int)fabs(y)*1024;
   Z=(int)fabs(z)*1024;

   if(Y<X)
	   swap(Y,X);
   if(Z<Y)
	   swap(Z,Y);
   if(Y<X)
	   swap(X,Y);

   int dist=(Z + 11*(Y>>5)+(X>>2));

   return ((float)(dist>>10));
}


void CVector3::Normal()
{
   float lenght = GetLength();

   if(lenght == 0.0f) lenght = 1.0f;

   x = x/lenght;
   y = y/lenght;
   z = z/lenght;
}


void CVector3::Normalize(CVector3 &v1, CVector3 &v2, CVector3 &v3)
{
   CVector3 e1, e2;

   e1.x = v2.x - v1.x;
   e1.y = v2.y - v1.y;
   e1.z = v2.z - v1.z;
   e1.Normal();

   e2.x = v3.x - v1.x;
   e2.y = v3.y - v1.y;
   e2.z = v3.z - v1.z;
   e2.Normal();

   CrossProduct(e1, e2);
   Normal();
}