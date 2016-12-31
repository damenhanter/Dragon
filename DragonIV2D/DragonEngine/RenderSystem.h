#pragma once
#include "datatype.h"
#include "BitmapFile.h"

class CRenderSystem
{
public:
	CRenderSystem(void);
	~CRenderSystem(void);

	USHORT (*RGB16Bit)(int r,int g,int b);

	int CreateDDraw(HWND hwnd,int screenWidth, int screenheight, int screenBPP,BOOL windowed);
	int DDrawFillSurface(LPDIRECTDRAWSURFACE7 lpdds,DWORD color);
	LPDIRECTDRAWSURFACE7 CreateSurface(int width, int height, int memflags, USHORT colorKeyValue);

	int FlipSurface(void);
	UCHAR* DDrawLockSurface(LPDIRECTDRAWSURFACE7 lpdds, int& lpitch);
	int DDrawUnLockSurface(LPDIRECTDRAWSURFACE7 lpdds);
	UCHAR* DDrawLockPrimarySurface();
	int DDrawUnLockPrimarySurface();
	UCHAR* DDrawLockBackSurface();
	int DDrawUnLockBackSurface();
	int GetClock(void);
	DWORD StartClock(void);
	DWORD WaitClock(DWORD count);
	int FillRectangle(int x1, int y1, int x2, int y2, int color, LPDIRECTDRAWSURFACE7 lpdds);
	
	int RotateColor(int startIndex, int endIndex);
	int BlinkColors(Blinker command, LPBLINKER newLight, int id);
	int ScanBitmapImage(CBitmapFile* lpbitmap, LPDIRECTDRAWSURFACE7 lpdds, int cx, int cy);
	int ScreenTransitions(ScreenAttr effect,UCHAR* vbuffer,int lpitch);
	
	int RenderRectangle(int sourceX1, int sourceY1, int sourceX2, int sourceY2, 
		int destX1, int destY1, int destX2, int destY2, 
		LPDIRECTDRAWSURFACE7 sourceSurface, LPDIRECTDRAWSURFACE7 destSurface);
	void LoadPaletteFromFile(char* filename);
	int SavePalette(LPPALETTEENTRY setPalette);
	int SetPalette(LPPALETTEENTRY setPalette);
	LPDIRECTDRAWSURFACE7 lpddsback;
	LPDIRECTDRAWSURFACE7 lpddsprimary;
	UCHAR* primaryBuffer;
	UCHAR* backBuffer;
	int primarylpitch;
	int backlpitch;
private:
	static inline USHORT RGB16Bit555(int r,int g,int b)
	{
		r>>=3; g>>=3; b>>=3;
		return (_RGB16BIT555(r,g,b));
	}
	static inline USHORT RGB16Bit565(int r,int g,int b)
	{
		r>>=3; g>>=2; b>>=3;
		return (_RGB16BIT565(r,g,b));
	}
	//static inline 
	int SetPaletteEntry(int colorIndex, LPPALETTEENTRY colorPalette);
	int GetPaletteEntry(int colorIndex, LPPALETTEENTRY colorPalette);
	
	void VLine(int y1, int y2, int x, int color, UCHAR* vbuffer, int lpitch);
	void VLine16(int y1, int y2, int x, int color, UCHAR* vbuffer, int lpitch);
	void HLine(int x1, int x2, int y, int color, UCHAR* vbuffer, int lpitch);
	void HLine16(int x1, int x2, int y, int color, UCHAR* vbuffer, int lpitch);
	void DDrawAttachClipper(LPDIRECTDRAWSURFACE7 lpdds, int numRects,LPRECT clipList);

private:
	LPDIRECTDRAW7 lpdd;
	
	LPDIRECTDRAWPALETTE lpddpal;
	LPDIRECTDRAWCLIPPER lpddclipper;          //用于裁剪后备缓存的裁剪器
	LPDIRECTDRAWCLIPPER lpddclipperwin;       //用于裁剪窗口
	DDSURFACEDESC2  ddsd;
	PALETTEENTRY palette[256];
	DDBLTFX ddbltfx;                          //硬件加速器的填充信息
	DDSCAPS2 ddscaps;
	
	int ddpixelformat;
	int screenWidth,screenHeight,screenBPP;
	BOOL windowed;
	DWORD startClockCount;
public:
	
	void Release(void);
	void DrawPixel(int x,int y,int color);
};
