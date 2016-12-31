#pragma once
#include "datatype.h"

class CTriangle2D
{
public:
	CTriangle2D(void);	
	CTriangle2D(int state, int x0, int y0, int Vx, int Vy);
	CTriangle2D(CTriangle2D& t);
	CTriangle2D& operator=(CTriangle2D& t);
	~CTriangle2D(void);
private:
	void DrawTopTri(int x1, int y1, int x2, int y2, int x3, int y3,int color,UCHAR* destbuffer,int lpitch);
	void DrawBottomTri(int x1, int y1, int x2, int y2, int x3, int y3, int color, UCHAR* destbuffer, int lpitch);
	void DrawTopTri16(int x1, int y1, int x2, int y2, int x3, int y3,int color,UCHAR* destbuffer,int lpitch);
	void DrawBottomTri16(int x1, int y1, int x2, int y2, int x3, int y3, int color, UCHAR* destbuffer, int lpitch);
	void DrawTopTriFP(int x1, int y1, int x2, int y2, int x3, int y3,int color,UCHAR* destbuffer,int lpitch);
	void DrawBottomTriFP(int x1, int y1, int x2, int y2, int x3, int y3, int color, UCHAR* destbuffer, int lpitch);
	void DrawTopTriFP16(int x1, int y1, int x2, int y2, int x3, int y3,int color,UCHAR* destbuffer,int lpitch);
	void DrawBottomTriFP16(int x1, int y1, int x2, int y2, int x3, int y3, int color, UCHAR* destbuffer, int lpitch);
public:
	void DrawTriangle2D(int x1, int y1, int x2, int y2, int x3, int y3, int color , UCHAR* destbuffer, int lpitch);
	void DrawTriangle162D(int x1, int y1, int x2, int y2, int x3, int y3, int color , UCHAR* destbuffer, int lpitch);
	void DrawTriangleFP2D(int x1, int y1, int x2, int y2, int x3, int y3, int color , UCHAR* destbuffer, int lpitch);
	void DrawTriangleFP162D(int x1, int y1, int x2, int y2, int x3, int y3, int color , UCHAR* destbuffer, int lpitch);
protected:
	int state;	// ¿É¼ûÐÔ
	int x0;
	int y0;
	int Vx;
	int Vy;
};
