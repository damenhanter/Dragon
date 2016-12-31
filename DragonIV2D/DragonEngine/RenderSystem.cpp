#include "StdAfx.h"
#include "RenderSystem.h"


extern HWND hWnd;
extern HANDLE hHandle;
extern RECT g_rRectClip;

#pragma warning (disable:4996)

CRenderSystem::CRenderSystem(void)
{
}

CRenderSystem::~CRenderSystem(void)
{
	Release();

}
void CRenderSystem::LoadPaletteFromFile(char* filename)
{
	FILE* pfile;
	if((pfile=fopen(filename,"r"))==NULL)
	{
		MessageBox(hWnd,"调色板文件打开失败!","error",MB_OK);
		PostMessage(hWnd,WM_DESTROY,0,0);
		return ;
	}
	for(int index=0; index<256; index++)
	{
		fscanf(pfile,"%d %d %d %d",&palette[index].peRed,
                                 &palette[index].peGreen,
                                 &palette[index].peBlue,                                
                                 &palette[index].peFlags);
	}
	fclose(pfile);
}
int CRenderSystem::CreateDDraw(HWND hWnd,int screenWidth, int screenHeight, int screenBPP,BOOL windowed)
{
	if(FAILED(DirectDrawCreateEx(NULL,(void**)&lpdd,IID_IDirectDraw7,NULL)))
	{
		MessageBox(hWnd,"创建DirectDraw对象失败！","error",MB_OK);
		return FALSE;
	}
	if(windowed)
	{
		if(FAILED(lpdd->SetCooperativeLevel(hWnd,DDSCL_NORMAL)))
		{
			MessageBox(hWnd,"设置与操作系统的协作失败！","error",MB_OK);
			return FALSE;
		}
	}
	else
	{
		if(FAILED(lpdd->SetCooperativeLevel(hWnd,DDSCL_ALLOWMODEX|
			DDSCL_ALLOWREBOOT|DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN|DDSCL_MULTITHREADED)))
		{
			MessageBox(hWnd,"设置与操作系统的协作失败！","error",MB_OK);
			return false;
		}
		if(FAILED(lpdd->SetDisplayMode(screenWidth,screenHeight,screenBPP,0,0)))
		{
			MessageBox(hWnd,"设置窗口的显示模式失败！","error",MB_OK);
			return false;
		}
	}
	this->screenWidth=screenWidth;
	this->screenHeight=screenHeight;
	this->screenBPP=screenBPP;
	this->windowed=windowed;

	DDRAW_INIT_STRUCT(ddsd);
	if (!windowed)
	{
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
		ddsd.dwBackBufferCount = 1;
	} 
	else
	{
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	} 
	if(FAILED(lpdd->CreateSurface(&ddsd,&lpddsprimary,NULL)))
	{
		MessageBox(hWnd,"创建主表面失败！","error",MB_OK);
		return false;
	}
	DDPIXELFORMAT ddpf;
	DDRAW_INIT_STRUCT(ddpf);
	lpddsprimary->GetPixelFormat(&ddpf);
	ddpixelformat=ddpf.dwRGBBitCount;
	if(ddpixelformat==DD_PIXEL_FORMAT555)
	{
		RGB16Bit=RGB16Bit555;
	}
	else if(ddpixelformat==DD_PIXEL_FORMAT565)
	{
		RGB16Bit=RGB16Bit565;
	}
	else if(ddpixelformat==DD_PIXEL_FORMAT888)
	{

	}
	if(!windowed)
	{
		ddscaps.dwCaps=DDSCAPS_BACKBUFFER;
		if(FAILED(lpddsprimary->GetAttachedSurface(&ddscaps,&lpddsback)))
		{
			MessageBox(hWnd,"获得后备表面失败！","error",MB_OK);
			return false;
		}
	}
	else
	{
		lpddsback=CreateSurface(screenWidth,screenHeight,DDSCAPS_VIDEOMEMORY,0);
	}
	if(screenBPP==DD_PIXEL_FORMAT8)
	{
		memset((void*)palette,0,256*sizeof(PALETTEENTRY));
		LoadPaletteFromFile(PALETTE_DEFAULT);
		if(windowed)
		{
			for (int index=0; index < 10; index++)
			{
				palette[index].peFlags = palette[index+246].peFlags = PC_EXPLICIT;       
			}
			if (FAILED(lpdd->CreatePalette(DDPCAPS_8BIT | DDPCAPS_INITIALIZE,
				palette,&lpddpal,NULL)))
				return	TRUE;
		} 
		else
		{
			if (FAILED(lpdd->CreatePalette(DDPCAPS_8BIT | DDPCAPS_INITIALIZE | DDPCAPS_ALLOW256,
				palette,&lpddpal,NULL)))
				return true;

		}
		if (FAILED(lpddsprimary->SetPalette(lpddpal)))
		{
			MessageBox(hWnd,"设置调色板失败!","",MB_OK);
			return false;
		}
	}
	if(!windowed)
	{
		DDrawFillSurface(lpddsback,0);
		DDrawFillSurface(lpddsprimary,0);
	}
	else
	{
		DDrawFillSurface(lpddsback,0);
	}
	g_rRectClip.top=0;
	g_rRectClip.left=0;
	g_rRectClip.right=screenWidth-1;
	g_rRectClip.bottom=screenHeight-1;
	DDrawAttachClipper(lpddsback,1,&g_rRectClip);
	if (windowed)
	{
		if (FAILED(lpdd->CreateClipper(0,&lpddclipperwin,NULL)))
			return	FALSE;

		if (FAILED(lpddclipperwin->SetHWnd(0, hWnd)))
			return FALSE;

		if (FAILED(lpddsprimary->SetClipper(lpddclipperwin)))
			return FALSE;
	}
	return TRUE;
}

LPDIRECTDRAWSURFACE7 CRenderSystem::CreateSurface(int width, int height, int memflags, USHORT colorKeyValue)
{
	DDSURFACEDESC2 ddsd;      
	LPDIRECTDRAWSURFACE7 lpdds; 

	DDRAW_INIT_STRUCT(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;

	ddsd.dwWidth  =  width;
	ddsd.dwHeight =  height;

	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | memflags;

	if (FAILED(lpdd->CreateSurface(&ddsd,&lpdds,NULL)))
	{
		MessageBox(hWnd,"CreateSurface失败！","error",MB_OK);
		return(NULL);
	}

	DDCOLORKEY color_key; 
	color_key.dwColorSpaceLowValue  = colorKeyValue;
	color_key.dwColorSpaceHighValue = colorKeyValue;
	lpdds->SetColorKey(DDCKEY_SRCBLT, &color_key);
	return lpdds;
}
int CRenderSystem::DDrawFillSurface(LPDIRECTDRAWSURFACE7 lpdds,DWORD color)
{
	DDBLTFX ddbltfx;
	DDRAW_INIT_STRUCT(ddbltfx);
	ddbltfx.dwFillColor=color;
	if(FAILED(lpdds->Blt(NULL,NULL,NULL,DDBLT_COLORFILL|DDBLT_WAIT,&ddbltfx)))
	{
		MessageBox(NULL,"硬件加速器调用失败！","error",MB_OK);
		return false;
	}
	return true;
}
void CRenderSystem::DDrawAttachClipper(LPDIRECTDRAWSURFACE7 lpdds, int numRects,LPRECT clipList)
{                    
	LPRGNDATA regionData;            
	if (FAILED(lpdd->CreateClipper(0,&lpddclipper,NULL)))
		return ; 
	regionData = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER)+numRects*sizeof(RECT));
	memcpy(regionData->Buffer, clipList, sizeof(RECT)*numRects);

	regionData->rdh.dwSize          = sizeof(RGNDATAHEADER);
	regionData->rdh.iType           = RDH_RECTANGLES;
	regionData->rdh.nCount          = numRects;
	regionData->rdh.nRgnSize        = numRects*sizeof(RECT);

	regionData->rdh.rcBound.left    =  64000;
	regionData->rdh.rcBound.top     =  64000;
	regionData->rdh.rcBound.right   = -64000;
	regionData->rdh.rcBound.bottom  = -64000;

	for (int index=0; index<numRects; index++)
	{
		if (clipList[index].left < regionData->rdh.rcBound.left)
			regionData->rdh.rcBound.left = clipList[index].left;

		if (clipList[index].right > regionData->rdh.rcBound.right)
			regionData->rdh.rcBound.right = clipList[index].right;

		if (clipList[index].top < regionData->rdh.rcBound.top)
			regionData->rdh.rcBound.top = clipList[index].top;

		if (clipList[index].bottom > regionData->rdh.rcBound.bottom)
			regionData->rdh.rcBound.bottom = clipList[index].bottom;

	}
	if (FAILED(lpddclipper->SetClipList(regionData, 0)))
	{
		free(regionData);
		return ;
	} 
	if (FAILED(lpdds->SetClipper(lpddclipper)))
	{
		free(regionData);
		return ;
	} 
	free(regionData);

}
int CRenderSystem::FlipSurface(void)
{
	if(!lpddsback)
	{
		MessageBox(hWnd,"后背表面为空!","error",MB_OK);
		return FALSE;
	}
	if(!windowed)
	{
		while(FAILED(lpddsprimary->Flip(lpddsback,DDFLIP_WAIT)))
		{
			MessageBox(hWnd,"表面反转失败!","error",MB_OK);
			return FALSE;
		}
	}
	else
	{
		RECT destRect;
		GetClientRect(hWnd,&destRect);
		POINT point1,point2;
		point1.x=destRect.left;
		point1.y=destRect.top;
		point2.x=destRect.right;
		point2.y=destRect.bottom;
		ClientToScreen(hWnd,&point1);
		ClientToScreen(hWnd,&point2);
		destRect.left=point1.x;
		destRect.top=point1.y;
		destRect.right=point2.x;
		destRect.bottom=point2.y;

		if(FAILED(lpddsprimary->Blt(&destRect,lpddsback,NULL,DDBLT_WAIT,NULL)))
		{
			MessageBox(hWnd,"FlipSurface--->Blt()调用失败!","error",MB_OK);
			return FALSE;
		}
	}
	return TRUE;
}

UCHAR* CRenderSystem::DDrawLockSurface(LPDIRECTDRAWSURFACE7 lpdds, int& lpitch)
{
	if(!lpdds) return NULL;
	DDRAW_INIT_STRUCT(ddsd);
	if(FAILED(lpdds->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT,NULL)))
	{
		MessageBox(hWnd,"加锁内存失败!","error",MB_OK);
		return FALSE;
	}
	lpitch=ddsd.lPitch;
	return (UCHAR*)ddsd.lpSurface;
}

int CRenderSystem::DDrawUnLockSurface(LPDIRECTDRAWSURFACE7 lpdds)
{
	if(lpdds)
		return FALSE;
	if(FAILED(lpdds->Unlock(NULL)))
	{
		MessageBox(hWnd,"解锁内存失败!","error",MB_OK);
		return FALSE;
	}
	return TRUE;
}
UCHAR* CRenderSystem::DDrawLockPrimarySurface()
{
	DDRAW_INIT_STRUCT(ddsd);
	if(FAILED(lpddsprimary->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT,NULL)))
	{
		MessageBox(hWnd,"加锁主表面失败!","error",MB_OK);
		return FALSE;
	}
	primarylpitch=ddsd.lPitch;
	primaryBuffer=(UCHAR*)ddsd.lpSurface;
	return primaryBuffer;
}

int CRenderSystem::DDrawUnLockPrimarySurface()
{
	if(FAILED(lpddsprimary->Unlock(NULL)))
	{
		MessageBox(hWnd,"解锁主表面失败!","error",MB_OK);
		return FALSE;
	}
	return TRUE;
}
UCHAR* CRenderSystem::DDrawLockBackSurface()
{
	DDRAW_INIT_STRUCT(ddsd);
	if(FAILED(lpddsback->Lock(NULL,&ddsd,DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT,NULL)))
	{
		MessageBox(hWnd,"加锁后备表面失败!","error",MB_OK);
		return FALSE;
	}
	backlpitch=ddsd.lPitch;
	backBuffer=(UCHAR*)ddsd.lpSurface;
	return backBuffer;
}

int CRenderSystem::DDrawUnLockBackSurface()
{
	if(FAILED(lpddsback->Unlock(NULL)))
	{
		MessageBox(hWnd,"解锁后备表面失败!","error",MB_OK);
		return FALSE;
	}
	return TRUE;
}

int CRenderSystem::GetClock(void)
{
	return (GetTickCount());
}

DWORD CRenderSystem::StartClock(void)
{
	return (startClockCount=GetTickCount());
}

DWORD CRenderSystem::WaitClock(DWORD count)
{
	while(GetClock()-StartClock()<count);
	return GetClock();
}

int CRenderSystem::FillRectangle(int x1, int y1, int x2, int y2, int color, LPDIRECTDRAWSURFACE7 lpdds)
{
	DDBLTFX bltfx;
	RECT fillRect;
	DDRAW_INIT_STRUCT(bltfx);
	bltfx.dwFillColor=color;
	fillRect.left=x1;
	fillRect.top=y1;
	fillRect.right=x2;
	fillRect.bottom=y2;
	if(FAILED(lpdds->Blt(&fillRect,NULL,NULL, DDBLT_COLORFILL | DDBLT_WAIT,&bltfx)))
	{
		MessageBox(hWnd,"渲染矩形失败!","error",MB_OK);
		return FALSE;
	}
	return TRUE;

}

int CRenderSystem::SetPaletteEntry(int colorIndex, LPPALETTEENTRY colorPalette)
{

	lpddpal->SetEntries(0,colorIndex,1,colorPalette);

	memcpy(&palette[colorIndex],colorPalette,sizeof(PALETTEENTRY));

	return true;
}

int CRenderSystem::GetPaletteEntry(int colorIndex, LPPALETTEENTRY colorPalette)
{
	memcpy(colorPalette,&palette[colorIndex],sizeof(PALETTEENTRY));
	return TRUE;
}

int CRenderSystem::SavePalette(LPPALETTEENTRY savePalette)
{
	memcpy(savePalette,palette,sizeof(PALETTEENTRY)*256);
	return true;
}

int CRenderSystem::SetPalette(LPPALETTEENTRY setPalette)
{
	memcpy(palette,setPalette,sizeof(PALETTEENTRY)*256);
	lpddpal->SetEntries(0,0,256,palette);
	return true;
}


int CRenderSystem::RotateColor(int startIndex, int endIndex)
{
	int colors=endIndex-startIndex+1;
	PALETTEENTRY wrkPal[256];
	lpddpal->GetEntries(0,startIndex,colors,wrkPal);
	lpddpal->SetEntries(0,startIndex+1,colors-1,wrkPal);
	lpddpal->SetEntries(0,startIndex,1,&wrkPal[colors-1]);
	lpddpal->GetEntries(0,0,256,palette);
	return TRUE;
}

int CRenderSystem::BlinkColors(Blinker command, LPBLINKER newLight, int id)
{
	static BLINKER lights[256]; 
	static int initialized = 0; 

	if (!initialized)
	{
		initialized = 1;
		memset((void *)lights,0, sizeof(lights));
	} 
	switch (command)
	{
	case BLINKER_ADD:
		{
			for (int index=0; index < 256; index++)
			{
				if (lights[index].state == 0)
				{
					lights[index] = *newLight;
					lights[index].counter =  0;
					lights[index].state   = -1;
					lpddpal->SetEntries(0,lights[index].color_index,1,&lights[index].off_color);
					return index;
				}
			} 
		} break;

	case BLINKER_DELETE: 
		{
			if (lights[id].state != 0)
			{
				memset((void *)&lights[id],0,sizeof(BLINKER));
				return id;
			} 
			else
				return -1; 
		} break;

	case BLINKER_UPDATE: 
		{ 
			if (lights[id].state != 0)
			{
				lights[id].on_color  = newLight->on_color; 
				lights[id].off_color = newLight->off_color;
				lights[id].on_time   = newLight->on_time;
				lights[id].off_time  = newLight->off_time; 
				if (lights[id].state == -1)
					lpddpal->SetEntries(0,lights[id].color_index,1,&lights[id].off_color);
				else
					lpddpal->SetEntries(0,lights[id].color_index,1,&lights[id].on_color);
				return id;
			}
			else
				return -1;
		} break;

	case BLINKER_RUN: 
		{
			for (int index=0; index < 256; index++)
			{
				if (lights[index].state == -1)
				{
					if (++lights[index].counter >= lights[index].off_time)
					{
						lights[index].counter = 0;
				        lights[index].state = -lights[index].state;               
						lpddpal->SetEntries(0,lights[index].color_index,1,&lights[index].on_color);
					} 
				} 
				else
					if (lights[index].state == 1)
					{
						if (++lights[index].counter >= lights[index].on_time)
						{
							lights[index].counter = 0;
							lights[index].state = -lights[index].state;               
							lpddpal->SetEntries(0,lights[index].color_index,1,&lights[index].off_color);
						}
					} 
			} 
		} break;
	default: break;
	} 
	return TRUE;
}

int CRenderSystem::ScanBitmapImage(CBitmapFile* lpbitmap, LPDIRECTDRAWSURFACE7 lpdds, int cx, int cy)
{
	UCHAR* dest;
	UCHAR* sour;
	DDSURFACEDESC2 ddsd;
	DDRAW_INIT_STRUCT(ddsd);
	if(lpdds->Lock(NULL,&ddsd,DDLOCK_WAIT|DDLOCK_SURFACEMEMORYPTR,NULL))
	{
		MessageBoxA(hWnd,"离屏表面加载位图失败!","Tips",MB_OK);
		return 0;
	}
	cx=cx*(ddsd.dwWidth+1)+1;
	cy=cy*(ddsd.dwHeight+1)+1;
	sour=lpbitmap->buffer+cy*lpbitmap->bitmapinfoheader.biWidth+cx;
	dest=(UCHAR*)ddsd.lpSurface;
	for(DWORD index=0; index<ddsd.dwHeight; index++)
	{
		memcpy(dest,sour,ddsd.dwWidth);
		dest+=ddsd.lPitch;
		sour+=lpbitmap->bitmapinfoheader.biWidth;
	}
	lpdds->Unlock(NULL);
	return true;
}

int CRenderSystem::ScreenTransitions(ScreenAttr effect,UCHAR* vbuffer,int lpitch)
{
	int red,green,blue;
	PALETTEENTRY color;
	PALETTEENTRY wrkPal[256];
	switch(effect)
	{
	case SCREEN_DARKNESS:
		{
			for(int i=0; i<80; i++)
			{
				SavePalette(wrkPal);
				for(int palreg=1; palreg<256; palreg++)
				{
					color=wrkPal[palreg];
					if(color.peRed>4)
						color.peRed-=3;
					else
						color.peRed=0;
					if(color.peGreen>4)
						color.peGreen-=3;
					else
						color.peGreen=0;
					if(color.peBlue>4)
						color.peBlue-=3;
					else
						color.peBlue=0;
					wrkPal[palreg]=color;
				}
				SetPalette(wrkPal);
				StartClock();
				WaitClock(12);
			}
			return TRUE;
		}
	case SCREEN_WHITENESS:
		{
			for(int i=0; i<64; i++)
			{
				SavePalette(wrkPal);
				for(int palreg=0; palreg<256; palreg++)
				{
					color=wrkPal[palreg];
					red=color.peRed;
					green=color.peGreen;
					blue=color.peBlue;

					if((red+=4)>=255)
						red=255;
					
					if((green+=4)>=255)
						green=255;

					if((blue+=4)>=255)
						blue=255;

					color.peRed=red;
					color.peGreen=green;
					color.peBlue=blue;

					wrkPal[palreg]=color;
				}
				SetPalette(wrkPal);
				StartClock();
				WaitClock(12);
			}
			return TRUE;
		}
	case SCREEN_REDNESS:
		{
			for(int i=0; i<64; i++)
			{
				SavePalette(wrkPal);
				for(int palreg=0; palreg<256; palreg++)
				{
					color=wrkPal[palreg];
					red=color.peRed;
					green=color.peGreen;
					blue=color.peBlue;

					if((red+6)>=255)
						red=255;
					
					if((green-=4)<=0)
						green=0;

					if((blue-=4)<=0)
						blue=0;

					color.peRed=red;
					color.peGreen=green;
					color.peBlue=blue;

					wrkPal[palreg]=color;
				}
				SetPalette(wrkPal);
				StartClock();
				WaitClock(12);
			}
			return TRUE;
		}
	case SCREEN_BLUENESS:
		{
			for(int i=0; i<64; i++)
			{
				SavePalette(wrkPal);
				for(int palreg=0; palreg<256; palreg++)
				{
					color=wrkPal[palreg];
					red=color.peRed;
					green=color.peGreen;
					blue=color.peBlue;

					if((red-=4)<=0)
						red=0;
					
					if((green-=4)<=0)
						green=0;

					if((blue+=6)>=255)
						blue=255;

					color.peRed=red;
					color.peGreen=green;
					color.peBlue=blue;

					wrkPal[palreg]=color;
				}
				SetPalette(wrkPal);
				StartClock();
				WaitClock(12);
			}
			return TRUE;
		}
	case SCREEN_GREENNESS:
		{
			for(int i=0; i<64; i++)
			{
				SavePalette(wrkPal);
				for(int palreg=0; palreg<256; palreg++)
				{
					color=wrkPal[palreg];
					red=color.peRed;
					green=color.peGreen;
					blue=color.peBlue;

					if((red-=4)<=0)
						red=0;
					
					if((green+=6)>=255)
						green=255;

					if((blue-=4)<=0)
						blue=0;

					color.peRed=red;
					color.peGreen=green;
					color.peBlue=blue;

					wrkPal[palreg]=color;
				}
				SetPalette(wrkPal);
				StartClock();
				WaitClock(12);
			}
			return TRUE;
		}
	case SCREEN_SWIPE_X:
		{
			for(int index=0; index<(screenWidth/2); index+=2)
			{
				StartClock();
				WaitClock(12);
				if(screenBPP==8)
				{
					VLine(0,(screenHeight-1),screenWidth-1-index,0,vbuffer,lpitch);
					VLine(0,(screenHeight-1),index,0,vbuffer,lpitch);
					VLine(0,(screenHeight-1),screenWidth-1-(index+1),0,vbuffer,lpitch);
					VLine(0,(screenHeight-1),index+1,0,vbuffer,lpitch);
				}
				else if(screenBPP==16)
				{
					VLine16(0,(screenHeight-1),screenWidth-1-index,0,vbuffer,lpitch);
					VLine16(0,(screenHeight-1),index,0,vbuffer,lpitch);
					VLine16(0,(screenHeight-1),screenWidth-1-(index+1),0,vbuffer,lpitch);
					VLine16(0,(screenHeight-1),index+1,0,vbuffer,lpitch);
				}
			}
			return TRUE;
		}
	case SCREEN_SWIPE_Y:
		{
			for(int index=0; index<(screenHeight/2); index+=2)
			{
				StartClock();
				WaitClock(12);
				if(screenBPP==8)
				{
					HLine(0,(screenWidth-1),screenHeight-1-index,0,vbuffer,lpitch);
					HLine(0,(screenWidth-1),index,0,vbuffer,lpitch);
					HLine(0,(screenWidth-1),screenHeight-1-(index+1),0,vbuffer,lpitch);
					HLine(0,(screenWidth-1),index+1,0,vbuffer,lpitch);
				}
				else if(screenBPP==16)
				{
					HLine16(0,(screenWidth-1),screenHeight-1-index,0,vbuffer,lpitch);
					HLine16(0,(screenWidth-1),index,0,vbuffer,lpitch);
					HLine16(0,(screenWidth-1),screenHeight-1-(index+1),0,vbuffer,lpitch);
					HLine16(0,(screenWidth-1),index+1,0,vbuffer,lpitch);
				}
			}
			return TRUE;
		}
	default:
		return TRUE;
	}
	return TRUE;
}

void CRenderSystem::VLine(int y1, int y2, int x, int color, UCHAR* vbuffer, int lpitch)
{
	 UCHAR* startoffset;
	 if(x>g_rRectClip.right||x<g_rRectClip.left)
		 return ;
	 if(y1>y2)
		 swap(y1,y2);
	 if(y1>g_rRectClip.bottom||y2<g_rRectClip.top)
		 return ;
	 y1=(y1<g_rRectClip.top)?g_rRectClip.top:y1;
	 y2=(y2>g_rRectClip.bottom)?g_rRectClip.bottom:y2;
	 startoffset=vbuffer+y1*lpitch+x;
	 for(int index=0; index<y2-y1; index++)
	 {
		 *startoffset=(UCHAR)color;
		 startoffset+=lpitch;
	 }
}

void CRenderSystem::VLine16(int y1, int y2, int x, int color, UCHAR* vbuffer, int lpitch)
{
	 USHORT* startoffset;
	 lpitch>>=1;
	 if(x>g_rRectClip.right||x<g_rRectClip.left)
		 return ;
	 if(y1>y2)
		 swap(y1,y2);
	 if(y1>g_rRectClip.bottom||y2<g_rRectClip.top)
		 return ;
	 y1=(y1<g_rRectClip.top)?g_rRectClip.top:y1;
	 y2=(y2>g_rRectClip.bottom)?g_rRectClip.bottom:y2;
	 startoffset=(USHORT*)vbuffer+y1*lpitch+x;
	 for(int index=0; index<y2-y1; index++)
	 {
		 *startoffset=(USHORT)color;
		 startoffset+=lpitch;
	 }
}

void CRenderSystem::HLine(int x1, int x2, int y, int color, UCHAR* vbuffer, int lpitch)
{
	if(y<g_rRectClip.top||y>g_rRectClip.bottom)
		return ;
	if(x1>x2)
		swap(x1,x2);
	if(x1>g_rRectClip.right||x2<g_rRectClip.left)
		return ;
	x1=(x1<g_rRectClip.left)?g_rRectClip.left:x1;
	x2=(x2>g_rRectClip.right)?g_rRectClip.right:x2;
	memset((UCHAR*)(vbuffer+lpitch*y+x1),(UCHAR)color,x2-x1+1);
}
void CRenderSystem::HLine16(int x1, int x2, int y, int color, UCHAR* vbuffer, int lpitch)
{
	lpitch>>=1;
	if(y<g_rRectClip.top||y>g_rRectClip.bottom)
		return ;
	if(x1>x2)
		swap(x1,x2);
	if(x1>g_rRectClip.right||x2<g_rRectClip.left)
		return ;
	x1=(x1<g_rRectClip.left)?g_rRectClip.left:x1;
	x2=(x2>g_rRectClip.right)?g_rRectClip.right:x2;
	memset((USHORT*)(vbuffer+lpitch*y+x1),(USHORT)color,x2-x1+1);
}



int CRenderSystem::RenderRectangle(int sourceX1, int sourceY1, int sourceX2, int sourceY2, 
								   int destX1, int destY1, int destX2, int destY2, LPDIRECTDRAWSURFACE7 sourceSurface,
								   LPDIRECTDRAWSURFACE7 destSurface)
{
	RECT sourRect;
	RECT destRect;

	sourRect.left=sourceX1;
	sourRect.top=sourceY1;
	sourRect.right=sourceX2;
	sourRect.bottom=sourceY2;

	destRect.left=destX1;
	destRect.top=destY1;
	destRect.right=destX2;
	destRect.bottom=destY2;
	
	if(FAILED(destSurface->Blt(&destRect,sourceSurface,&sourRect,(DDBLT_WAIT | DDBLT_KEYSRC),NULL)))
	{
		MessageBox(hWnd,"RenderRectangle调用失败,blt失败!","error",MB_OK);
		return FALSE;
	}
	return TRUE;
}

void CRenderSystem::Release(void)
{
	if(lpddclipperwin!=NULL)
	{
		lpddclipperwin->Release();
		lpddclipperwin=NULL;
	}
	if(lpddclipper!=NULL)
	{
		lpddclipper->Release();
		lpddclipper=NULL;
	}
	if(lpddpal!=NULL)
	{
		lpddpal->Release();
		lpddpal=NULL;
	}
	if(lpddsback!=NULL)
	{
		lpddsback->Release();
		lpddsback=NULL;
	}
	if(lpddsprimary!=NULL)
	{
		lpddsprimary->Release();
		lpddsprimary=NULL;
	}
	if(lpdd!=NULL)
	{
		lpdd->Release();
		lpdd=NULL;
	}
}

void CRenderSystem::DrawPixel(int x,int y,int color)
{
	DDrawLockBackSurface();
	if (screenBPP==8)
	{
		*(backBuffer+y*backlpitch+x)=(UCHAR)color;
	}
	else if (screenBPP==16)
	{
		*((USHORT*)backBuffer+y*(backlpitch>>1)+x)=color;
	}
	DDrawUnLockBackSurface();
}
