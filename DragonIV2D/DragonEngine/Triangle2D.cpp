#include "StdAfx.h"
#include "Triangle2D.h"
extern RECT g_rRectClip;

CTriangle2D::CTriangle2D(void)
: state(0)
, x0(0)
, y0(0)
, Vx(0)
, Vy(0)
{
}

CTriangle2D::CTriangle2D(int state, int x0, int y0, int Vx, int Vy)
{
	this->state=state;
	this->x0=x0;
	this->y0=y0;
	this->Vx=Vx;
	this->Vy=Vy;
}
CTriangle2D::~CTriangle2D(void)
{
}


CTriangle2D::CTriangle2D(CTriangle2D& t)
{
	state=t.state;
	x0=t.x0;y0=t.y0;
	Vx=t.Vx;Vy=t.Vy;
}


CTriangle2D& CTriangle2D::operator=(CTriangle2D& t)
{
	//TODO: insert return statement here
	state=t.state;
	x0=t.x0;y0=t.y0;
	Vx=t.Vx;Vy=t.Vy;
	return *this;
}


void CTriangle2D::DrawTopTri(int x1, int y1, int x2, int y2, int x3, int y3,
						   int color,UCHAR* destbuffer,int lpitch)
{	
	float dxright,dxleft,xs,xe,height;
	int right,left;
	UCHAR* destaddr=NULL;
	if(x2<x1)
	{
		swap(x1,x2);
	}
	height=(float)(y3-y1);
	dxleft=(x3-x1)/height;
	dxright=(x3-x2)/height;
	xs=(float)x1;
	xe=(float)x2+(float)0.5;
	//对y进行裁剪
	if(y1<g_rRectClip.top)
	{
		xs=xs+dxleft*(float)(g_rRectClip.top-y1);
		xe=xe+dxright*(float)(g_rRectClip.top-y1);
		y1=g_rRectClip.top;
	}
	if(y3>g_rRectClip.bottom)
	{
		y3=g_rRectClip.bottom;
	}
	destaddr=destbuffer+y1*lpitch;
	//测试x是否需要裁剪
	if(x1>=g_rRectClip.left && x1<=g_rRectClip.right 
		&& x2>=g_rRectClip.left && x2<=g_rRectClip.right
		&& x3>=g_rRectClip.left && x3<=g_rRectClip.right)
	{
		for(int i=y1; i<=y3; i++,destaddr+=lpitch)
		{
			memset((UCHAR*)destaddr+(unsigned int)xs,color,(unsigned int)(xe-xs+1));
			xs+=dxleft;
			xe+=dxright;
		}
	}
	else
	{
		for(int i=y1; i<=y3; i++,destaddr+=lpitch)
		{
			left=(int)xs;
			right=(int)xe;
			xs+=dxleft;
			xe+=dxright;
			if(left<g_rRectClip.left)
			{
				left=g_rRectClip.left;
				if(right<g_rRectClip.left)
					continue;
			}
			if(right>g_rRectClip.right)
			{
				right=g_rRectClip.right;
				if(left>g_rRectClip.right)
					continue;
			}
			memset((UCHAR*)destaddr+(unsigned int)left,color,(unsigned int)(right-left+1));
		}
	}
}

void CTriangle2D::DrawBottomTri(int x1, int y1, int x2, int y2, int x3, int y3, 
							  int color, UCHAR* destbuffer, int lpitch)
{
	float dxright,dxleft,xs,xe,height;
	int right,left;
	UCHAR* destaddr=NULL;
	if(x3<x2)
	{
		swap(x3,x2);
	}
	height=(float)(y3-y1);
	dxleft=(x2-x1)/height;
	dxright=(x3-x1)/height;
	xs=(float)x1;
	xe=(float)x1;
	//对y进行裁剪
	if(y1<g_rRectClip.top)
	{
		xs=xs+dxleft*(float)(g_rRectClip.top-y1);
		xe=xe+dxright*(float)(g_rRectClip.top-y1);
		y1=g_rRectClip.top;
	}
	if(y3>g_rRectClip.bottom)
	{
		y3=g_rRectClip.bottom;
	}
	destaddr=destbuffer+y1*lpitch;
	//测试x是否需要裁剪
	if(x1>=g_rRectClip.left && x1<=g_rRectClip.right 
		&& x2>=g_rRectClip.left && x2<=g_rRectClip.right
		&& x3>=g_rRectClip.left && x3<=g_rRectClip.right)
	{
		for(int i=y1; i<=y3; i++,destaddr+=lpitch)
		{
			memset((UCHAR*)destaddr+(unsigned int)xs,color,(unsigned int)(xe-xs+1));
			xs+=dxleft;
			xe+=dxright;
		}
	}
	else
	{
		for(int i=y1; i<=y3; i++,destaddr+=lpitch)
		{
			left=(int)xs;
			right=(int)xe;
			xs+=dxleft;
			xe+=dxright;
			if(left<g_rRectClip.left)
			{
				left=g_rRectClip.left;
				if(right<g_rRectClip.left)
					continue;
			}
			if(right>g_rRectClip.right)
			{
				right=g_rRectClip.right;
				if(left>g_rRectClip.right)
					continue;
			}
			memset((UCHAR*)destaddr+(unsigned int)left,color,(unsigned int)(right-left+1));
		}
	}
}

void CTriangle2D::DrawTriangle2D(int x1, int y1, int x2, int y2, int x3, int y3,
							 int color , UCHAR* destbuffer, int lpitch)
{
	int newx;
	if((x1==x2 && x2==x3)||(y1==y2 && y2==y3))
		return ;
	if(y2<y1)
	{
		swap(x1,x2);
		swap(y1,y2);
	}
	if(y3<y1)
	{
		swap(y1,y3);
		swap(x1,x3);
	}
	if(y3<y2)
	{
		swap(x3,x2);
		swap(y3,y2);
	}
	if(y3<g_rRectClip.top || y1>g_rRectClip.bottom
		|| (x1<g_rRectClip.left && x2<g_rRectClip.left && x3<g_rRectClip.left)
		|| (x1>g_rRectClip.right && x2>g_rRectClip.right && x3>g_rRectClip.right))
		return ;
	if(y1==y2)
	{
		DrawTopTri(x1,y1,x2,y2,x3,y3,color,destbuffer,lpitch);
	}
	else if(y2==y3)
	{
		DrawBottomTri(x1,y1,x2,y2,x3,y3,color,destbuffer,lpitch);
	}
	else
	{
		newx=x1+(int)(0.5+(float)(y2-y1)*(float)(x3-x1)/(float)(y3-y1));
		DrawBottomTri(x1,y1,newx,y2,x2,y2,color,destbuffer,lpitch);
		DrawTopTri(x2,y2,newx,y2,x3,y3,color,destbuffer,lpitch);
	}
}
void CTriangle2D::DrawTopTri16(int x1, int y1, int x2, int y2, int x3, int y3,int color,UCHAR* destbuffer,int lpitch)
{
	float dxright,dxleft,xs,xe,height;
	int right,left;
	USHORT* destaddr=NULL;
	USHORT* destbuffer16=(USHORT*)destbuffer;
	lpitch>>=1;
	if(x2<x1)
	{
		swap(x1,x2);
	}
	height=(float)(y3-y1);
	dxleft=(x3-x1)/height;
	dxright=(x3-x2)/height;
	xs=(float)x1;
	xe=(float)x2+(float)0.5;
	//对y进行裁剪
	if(y1<g_rRectClip.top)
	{
		xs=xs+dxleft*(float)(g_rRectClip.top-y1);
		xe=xe+dxright*(float)(g_rRectClip.top-y1);
		y1=g_rRectClip.top;
	}
	if(y3>g_rRectClip.bottom)
	{
		y3=g_rRectClip.bottom;
	}
	destaddr=destbuffer16+y1*lpitch;
	//测试x是否需要裁剪
	if(x1>=g_rRectClip.left && x1<=g_rRectClip.right 
		&& x2>=g_rRectClip.left && x2<=g_rRectClip.right
		&& x3>=g_rRectClip.left && x3<=g_rRectClip.right)
	{
		for(int i=y1; i<=y3; i++,destaddr+=lpitch)
		{
			MemSetWORD((USHORT*)destaddr+(unsigned int)xs,color,(unsigned int)(xe-xs+1));
			xs+=dxleft;
			xe+=dxright;
		}
	}
	else
	{
		for(int i=y1; i<=y3; i++,destaddr+=lpitch)
		{
			left=(int)xs;
			right=(int)xe;
			xs+=dxleft;
			xe+=dxright;
			if(left<g_rRectClip.left)
			{
				left=g_rRectClip.left;
				if(right<g_rRectClip.left)
					continue;
			}
			if(right>g_rRectClip.right)
			{
				right=g_rRectClip.right;
				if(left>g_rRectClip.right)
					continue;
			}
			MemSetWORD((USHORT*)destaddr+(unsigned int)left,color,(unsigned int)(right-left+1));
		}
	}
}



void CTriangle2D::DrawBottomTri16(int x1, int y1, int x2, int y2, int x3, int y3, int color, UCHAR* destbuffer, int lpitch)
{
	float dxright,dxleft,xs,xe,height;
	int right,left;
	USHORT* destaddr=NULL;
	USHORT* destbuffer16=(USHORT*)destbuffer;
	lpitch>>=1;
	if(x3<x2)
	{
		swap(x3,x2);
	}
	height=(float)(y3-y1);
	dxleft=(x2-x1)/height;
	dxright=(x3-x1)/height;
	xs=(float)x1;
	xe=(float)x1;
	//对y进行裁剪
	if(y1<g_rRectClip.top)
	{
		xs=xs+dxleft*(float)(g_rRectClip.top-y1);
		xe=xe+dxright*(float)(g_rRectClip.top-y1);
		y1=g_rRectClip.top;
	}
	if(y3>g_rRectClip.bottom)
	{
		y3=g_rRectClip.bottom;
	}
	destaddr=destbuffer16+y1*lpitch;
	//测试x是否需要裁剪
	if(x1>=g_rRectClip.left && x1<=g_rRectClip.right 
		&& x2>=g_rRectClip.left && x2<=g_rRectClip.right
		&& x3>=g_rRectClip.left && x3<=g_rRectClip.right)
	{
		for(int i=y1; i<=y3; i++,destaddr+=lpitch)
		{
			MemSetWORD((USHORT*)destaddr+(unsigned int)xs,color,(unsigned int)(xe-xs+1));
			xs+=dxleft;
			xe+=dxright;
		}
	}
	else
	{
		for(int i=y1; i<=y3; i++,destaddr+=lpitch)
		{
			left=(int)xs;
			right=(int)xe;
			xs+=dxleft;
			xe+=dxright;
			if(left<g_rRectClip.left)
			{
				left=g_rRectClip.left;
				if(right<g_rRectClip.left)
					continue;
			}
			if(right>g_rRectClip.right)
			{
				right=g_rRectClip.right;
				if(left>g_rRectClip.right)
					continue;
			}
			MemSetWORD((USHORT*)destaddr+(unsigned int)left,color,(unsigned int)(right-left+1));
		}
	}
}

void CTriangle2D::DrawTriangle162D(int x1, int y1, int x2, int y2, int x3, int y3, int color , UCHAR* destbuffer, int lpitch)
{
	int newx;
	if((x1==x2 && x2==x3)||(y1==y2 && y2==y3))
		return ;
	if(y2<y1)
	{
		swap(x1,x2);
		swap(y1,y2);
	}
	if(y3<y1)
	{
		swap(y1,y3);
		swap(x1,x3);
	}
	if(y3<y2)
	{
		swap(x3,x2);
		swap(y3,y2);
	}
	if(y3<g_rRectClip.top || y1>g_rRectClip.bottom
		|| (x1<g_rRectClip.left && x2<g_rRectClip.left && x3<g_rRectClip.left)
		|| (x1>g_rRectClip.right && x2>g_rRectClip.right && x3>g_rRectClip.right))
		return ;
	if(y1==y2)
	{
		DrawTopTri16(x1,y1,x2,y2,x3,y3,color,destbuffer,lpitch);
	}
	else if(y2==y3)
	{
		DrawBottomTri16(x1,y1,x2,y2,x3,y3,color,destbuffer,lpitch);
	}
	else
	{
		newx=x1+(int)(0.5+(float)(y2-y1)*(float)(x3-x1)/(float)(y3-y1));
		DrawBottomTri16(x1,y1,newx,y2,x2,y2,color,destbuffer,lpitch);
		DrawTopTri16(x2,y2,newx,y2,x3,y3,color,destbuffer,lpitch);
	}
}
void CTriangle2D::DrawTopTriFP(int x1, int y1, int x2, int y2, int x3, int y3,
						   int color,UCHAR* destbuffer,int lpitch)
{	
	int dxright,dxleft,xs,xe,height;
	int right,left;
	UCHAR* destaddr=NULL;
	if(x2<x1)
	{
		swap(x1,x2);
	}
	height=(y3-y1);
	dxleft=((x3-x1)<<FIXP16_SHIFT)/height;
	dxright=((x3-x2)<<FIXP16_SHIFT)/height;
	xs=x1<<FIXP16_SHIFT;
	xe=x2<<FIXP16_SHIFT;
	//对y进行裁剪
	if(y1<g_rRectClip.top)
	{
		xs=xs+dxleft*(g_rRectClip.top-y1);
		xe=xe+dxright*(g_rRectClip.top-y1);
		y1=g_rRectClip.top;
	}
	if(y3>g_rRectClip.bottom)
	{
		y3=g_rRectClip.bottom;
	}
	destaddr=destbuffer+y1*lpitch;
	//测试x是否需要裁剪
	if(x1>=g_rRectClip.left && x1<=g_rRectClip.right 
		&& x2>=g_rRectClip.left && x2<=g_rRectClip.right
		&& x3>=g_rRectClip.left && x3<=g_rRectClip.right)
	{
		for(int i=y1; i<=y3; i++,destaddr+=lpitch)
		{
			memset((UCHAR*)destaddr+((xs+FIXP16_ROUND_UP)>>FIXP16_SHIFT),
				color,(((xe-xs+FIXP16_ROUND_UP)>>FIXP16_SHIFT)+1));
			xs+=dxleft;
			xe+=dxright;
		}
	}
	else
	{
		for(int i=y1; i<=y3; i++,destaddr+=lpitch)
		{
			left=(xs+FIXP16_ROUND_UP)>>FIXP16_SHIFT;
			right=(xe+FIXP16_ROUND_UP)>>FIXP16_SHIFT;
			xs+=dxleft;
			xe+=dxright;
			if(left<g_rRectClip.left)
			{
				left=g_rRectClip.left;
				if(right<g_rRectClip.left)
					continue;
			}
			if(right>g_rRectClip.right)
			{
				right=g_rRectClip.right;
				if(left>g_rRectClip.right)
					continue;
			}
			memset((UCHAR*)destaddr+(unsigned int)left,color,(unsigned int)(right-left+1));
		}
	}
}

void CTriangle2D::DrawBottomTriFP(int x1, int y1, int x2, int y2, int x3, int y3, 
							  int color, UCHAR* destbuffer, int lpitch)
{
	int dxright,dxleft,xs,xe,height;
	int right,left;
	UCHAR* destaddr=NULL;
	if(x3<x2)
	{
		swap(x3,x2);
	}
	height=(y3-y1);
	dxleft=((x2-x1)<<FIXP16_SHIFT)/height;
	dxright=((x3-x1)<<FIXP16_SHIFT)/height;
	xs=x1<<FIXP16_SHIFT;
	xe=x1<<FIXP16_SHIFT;
	//对y进行裁剪
	if(y1<g_rRectClip.top)
	{
		xs=xs+dxleft*(g_rRectClip.top-y1);
		xe=xe+dxright*(g_rRectClip.top-y1);
		y1=g_rRectClip.top;
	}
	if(y3>g_rRectClip.bottom)
	{
		y3=g_rRectClip.bottom;
	}
	destaddr=destbuffer+y1*lpitch;
	//测试x是否需要裁剪
	if(x1>=g_rRectClip.left && x1<=g_rRectClip.right 
		&& x2>=g_rRectClip.left && x2<=g_rRectClip.right
		&& x3>=g_rRectClip.left && x3<=g_rRectClip.right)
	{
		for(int i=y1; i<=y3; i++,destaddr+=lpitch)
		{
			memset((UCHAR*)destaddr+((xs+FIXP16_ROUND_UP)>>FIXP16_SHIFT),
				color,(((xe-xs+FIXP16_ROUND_UP)>>FIXP16_SHIFT)+1));
			xs+=dxleft;
			xe+=dxright;
		}
	}
	else
	{
		for(int i=y1; i<=y3; i++,destaddr+=lpitch)
		{
			left=(xs+FIXP16_ROUND_UP)>>FIXP16_SHIFT;
			right=(xe+FIXP16_ROUND_UP)>>FIXP16_SHIFT;
			xs+=dxleft;
			xe+=dxright;
			if(left<g_rRectClip.left)
			{
				left=g_rRectClip.left;
				if(right<g_rRectClip.left)
					continue;
			}
			if(right>g_rRectClip.right)
			{
				right=g_rRectClip.right;
				if(left>g_rRectClip.right)
					continue;
			}
			memset((UCHAR*)destaddr+(unsigned int)left,color,(unsigned int)(right-left+1));
		}
	}
}

void CTriangle2D::DrawTriangleFP2D(int x1, int y1, int x2, int y2, int x3, int y3,
							 int color , UCHAR* destbuffer, int lpitch)
{
	int newx;
	if((x1==x2 && x2==x3)||(y1==y2 && y2==y3))
		return ;
	if(y2<y1)
	{
		swap(x1,x2);
		swap(y1,y2);
	}
	if(y3<y1)
	{
		swap(y1,y3);
		swap(x1,x3);
	}
	if(y3<y2)
	{
		swap(x3,x2);
		swap(y3,y2);
	}
	if(y3<g_rRectClip.top || y1>g_rRectClip.bottom
		|| (x1<g_rRectClip.left && x2<g_rRectClip.left && x3<g_rRectClip.left)
		|| (x1>g_rRectClip.right && x2>g_rRectClip.right && x3>g_rRectClip.right))
		return ;
	if(y1==y2)
	{
		DrawTopTriFP(x1,y1,x2,y2,x3,y3,color,destbuffer,lpitch);
	}
	else if(y2==y3)
	{
		DrawBottomTriFP(x1,y1,x2,y2,x3,y3,color,destbuffer,lpitch);
	}
	else
	{
		newx=x1+(int)(0.5+(float)(y2-y1)*(float)(x3-x1)/(float)(y3-y1));
		DrawBottomTriFP(x1,y1,newx,y2,x2,y2,color,destbuffer,lpitch);
		DrawTopTriFP(x2,y2,newx,y2,x3,y3,color,destbuffer,lpitch);
	}
}
void CTriangle2D::DrawTopTriFP16(int x1, int y1, int x2, int y2, int x3, int y3,
						   int color,UCHAR* destbuffer,int lpitch)
{	
	int dxright,dxleft,xs,xe,height;
	int right,left;
	USHORT* destaddr=NULL;
	USHORT* destbuffer16=(USHORT*)destbuffer;
	lpitch>>=1;
	if(x2<x1)
	{
		swap(x1,x2);
	}
	height=(y3-y1);
	dxleft=((x3-x1)<<FIXP16_SHIFT)/height;
	dxright=((x3-x2)<<FIXP16_SHIFT)/height;
	xs=x1<<FIXP16_SHIFT;
	xe=x2<<FIXP16_SHIFT;
	//对y进行裁剪
	if(y1<g_rRectClip.top)
	{
		xs=xs+dxleft*(g_rRectClip.top-y1);
		xe=xe+dxright*(g_rRectClip.top-y1);
		y1=g_rRectClip.top;
	}
	if(y3>g_rRectClip.bottom)
	{
		y3=g_rRectClip.bottom;
	}
	destaddr=destbuffer16+y1*lpitch;
	//测试x是否需要裁剪
	if(x1>=g_rRectClip.left && x1<=g_rRectClip.right 
		&& x2>=g_rRectClip.left && x2<=g_rRectClip.right
		&& x3>=g_rRectClip.left && x3<=g_rRectClip.right)
	{
		for(int i=y1; i<=y3; i++,destaddr+=lpitch)
		{
			MemSetWORD((USHORT*)destaddr+((xs+FIXP16_ROUND_UP)>>FIXP16_SHIFT),
				color,(((xe-xs+FIXP16_ROUND_UP)>>FIXP16_SHIFT)+1));
			xs+=dxleft;
			xe+=dxright;
		}
	}
	else
	{
		for(int i=y1; i<=y3; i++,destaddr+=lpitch)
		{
			left=(xs+FIXP16_ROUND_UP)>>FIXP16_SHIFT;
			right=(xe+FIXP16_ROUND_UP)>>FIXP16_SHIFT;
			xs+=dxleft;
			xe+=dxright;
			if(left<g_rRectClip.left)
			{
				left=g_rRectClip.left;
				if(right<g_rRectClip.left)
					continue;
			}
			if(right>g_rRectClip.right)
			{
				right=g_rRectClip.right;
				if(left>g_rRectClip.right)
					continue;
			}
			MemSetWORD((USHORT*)destaddr+(unsigned int)left,color,(unsigned int)(right-left+1));
		}
	}
}

void CTriangle2D::DrawBottomTriFP16(int x1, int y1, int x2, int y2, int x3, int y3, 
							  int color, UCHAR* destbuffer, int lpitch)
{
	int dxright,dxleft,xs,xe,height;
	int right,left;
	USHORT* destaddr=NULL;
	lpitch>>=1;
	if(x3<x2)
	{
		swap(x3,x2);
	}
	height=(y3-y1);
	dxleft=((x2-x1)<<FIXP16_SHIFT)/height;
	dxright=((x3-x1)<<FIXP16_SHIFT)/height;
	xs=x1<<FIXP16_SHIFT;
	xe=x1<<FIXP16_SHIFT;
	//对y进行裁剪
	if(y1<g_rRectClip.top)
	{
		xs=xs+dxleft*(g_rRectClip.top-y1);
		xe=xe+dxright*(g_rRectClip.top-y1);
		y1=g_rRectClip.top;
	}
	if(y3>g_rRectClip.bottom)
	{
		y3=g_rRectClip.bottom;
	}
	destaddr=(USHORT*)destbuffer+y1*lpitch;
	//测试x是否需要裁剪
	if(x1>=g_rRectClip.left && x1<=g_rRectClip.right 
		&& x2>=g_rRectClip.left && x2<=g_rRectClip.right
		&& x3>=g_rRectClip.left && x3<=g_rRectClip.right)
	{
		for(int i=y1; i<=y3; i++,destaddr+=lpitch)
		{
			MemSetWORD((USHORT*)destaddr+((xs+FIXP16_ROUND_UP)>>FIXP16_SHIFT),
				color,(((xe-xs+FIXP16_ROUND_UP)>>FIXP16_SHIFT)+1));
			xs+=dxleft;
			xe+=dxright;
		}
	}
	else
	{
		for(int i=y1; i<=y3; i++,destaddr+=lpitch)
		{
			left=(xs+FIXP16_ROUND_UP)>>FIXP16_SHIFT;
			right=(xe+FIXP16_ROUND_UP)>>FIXP16_SHIFT;
			xs+=dxleft;
			xe+=dxright;
			if(left<g_rRectClip.left)
			{
				left=g_rRectClip.left;
				if(right<g_rRectClip.left)
					continue;
			}
			if(right>g_rRectClip.right)
			{
				right=g_rRectClip.right;
				if(left>g_rRectClip.right)
					continue;
			}
			MemSetWORD((USHORT*)destaddr+(unsigned int)left,color,(unsigned int)(right-left+1));
		}
	}
}

void CTriangle2D::DrawTriangleFP162D(int x1, int y1, int x2, int y2, int x3, int y3,
							 int color , UCHAR* destbuffer, int lpitch)
{
	int newx;
	if((x1==x2 && x2==x3)||(y1==y2 && y2==y3))
		return ;
	if(y2<y1)
	{
		swap(x1,x2);
		swap(y1,y2);
	}
	if(y3<y1)
	{
		swap(y1,y3);
		swap(x1,x3);
	}
	if(y3<y2)
	{
		swap(x3,x2);
		swap(y3,y2);
	}
	if(y3<g_rRectClip.top || y1>g_rRectClip.bottom
		|| (x1<g_rRectClip.left && x2<g_rRectClip.left && x3<g_rRectClip.left)
		|| (x1>g_rRectClip.right && x2>g_rRectClip.right && x3>g_rRectClip.right))
		return ;
	if(y1==y2)
	{
		DrawTopTriFP16(x1,y1,x2,y2,x3,y3,color,destbuffer,lpitch);
	}
	else if(y2==y3)
	{
		DrawBottomTriFP16(x1,y1,x2,y2,x3,y3,color,destbuffer,lpitch);
	}
	else
	{
		newx=x1+(int)(0.5+(float)(y2-y1)*(float)(x3-x1)/(float)(y3-y1));
		DrawBottomTriFP16(x1,y1,newx,y2,x2,y2,color,destbuffer,lpitch);
		DrawTopTriFP16(x2,y2,newx,y2,x3,y3,color,destbuffer,lpitch);
	}
}