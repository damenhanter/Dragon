#pragma once
#include "datatype.h"
extern RECT g_rRectClip;

class CLine2D
{
public:
	CLine2D(void);
	~CLine2D(void);
	int DrawLine2D(int x1, int y1, int x2, int y2, UCHAR color, UCHAR* vb_start, int lpitch);// »­Ö±Ïß
	int DrawLine162D(int x1, int y1, int x2, int y2, int color, UCHAR* vb_start, int lpitch);
	int DrawClipLine2D(int x1, int y1, int x2, int y2, int color, UCHAR* vb_start, int lpitch);
	int DrawClipLine162D(int x1, int y1, int x2, int y2, int color, UCHAR* vb_start, int lpitch);
private:
	int ClipLine2D(int& x1, int& y1, int& x2, int& y2);
	POINT ClipMid2D(int x1, int y1, int x2, int y2);
	inline int IsInArea2D(int x,int y)
	{
		if(x >= g_rRectClip.left && x <=  g_rRectClip.right  &&  y >= g_rRectClip.bottom && y <= g_rRectClip.top)
			return true;
		else
			return false;
	}
	inline int NotIntersect2D(int x1,int y1,int x2,int y2)
	{
		int maxx,maxy,minx,miny;
		maxx = (x1>x2) ? x1 : x2 ;
		minx = (x1<x2) ? x1 : x2 ;
		maxy = (y1>y2) ? y1 : y2 ;
		miny = (y1<y2) ? y1 : y2 ;

		if(maxx<g_rRectClip.left || minx > g_rRectClip.right || miny > g_rRectClip.bottom || maxy< g_rRectClip.top )
			return 1;
		else 
			return 0;
	}
};
