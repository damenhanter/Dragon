#pragma once
#ifndef _DATATYPE_
#define _DATATYPE_

#define INITGUID
#define DIRECTINPUT_VERSION  0x0800

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>

#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <direct.h>
#include <wchar.h>
#include <math.h>
#include <SYS\TIMEB.H>
#include <time.h>
#include <io.h>
#include <fcntl.h>
#include <objbase.h>
#include <dinput.h>
#include <dsound.h>
#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>
#include <ddraw.h>

#pragma warning (disable:4996) 
#pragma warning (disable:4244) 

extern float cosTable[16];
extern float sinTable[16];

#define ClassName  "SCOTT" 

#define SCREEN_WIDTH       640
#define SCREEN_HEIGHT      480
#define SCREEN_BPP         16

#define BITMAP_ID           0x4D42         //位图统一id

#define PALETTE_DEFAULT  "..\\res\\PALDATA1.PAL"

enum BitmapState
{
	BITMAP_STATE_DEAD,
	BITMAP_STATE_ALIVE,
	BITMAP_STATE_DYING,
	BITMAP_EXTRACT_MODE_ABS,
	BITMAP_EXTRACT_MODE_CELL,
	BITMAP_ATTR_LOADED=128
};

enum DDPixelFormat
{
	DD_PIXEL_FORMAT8=8,
	DD_PIXEL_FORMAT555=15,
	DD_PIXEL_FORMAT565=16,
	DD_PIXEL_FORMAT888=24,
	DD_PIXEL_FORMATA888=32
};	

enum AnimState
{
	ANIM_STATE_DEAD,
	ANIM_STATE_ANIM_DONE,
	ANIM_STATE_ALIVE=1,
	ANIM_STATE_DYING,
	ANIM_MAX_ANIMATIONS=16
};

#define ANIM_MAX_FRAMES   64
#define ANIM_MAX_ANIMATIONS   32
//DRAGON 属性
enum AnimAddr
{
	ANIM_ATTR_SINGLE_FRAME=1,
	ANIM_ATTR_MULTI_FRAME=2,
	ANIM_ATTR_MULTI_ANIM=4,
	ANIM_ATTR_ANIM_ONE_SHOT=8,
	ANIM_ATTR_VISIBLE=16,
	ANIM_ATTR_BOUNCE=32,
	ANIM_ATTR_WRAPAROUND=64,
	ANIM_ATTR_LOADED=128,
	ANIM_ATTR_CLONE=256
};
enum AnimDirection
{
	ANIM_EAST,
	ANIM_NEAST,
	ANIM_NORTH,
	ANIM_NWEST,
	ANIM_WEST,
	ANIM_SWEST,
	ANIM_SOUTH,
	ANIM_SEAST
};
//屏幕移动
enum ScreenAttr
{
	SCREEN_DARKNESS,
	SCREEN_WHITENESS,
	SCREEN_SWIPE_X,
	SCREEN_SWIPE_Y,
	SCREEN_DISOLVE,
	SCREEN_SCRUNCH,
	SCREEN_BLUENESS,
	SCREEN_REDNESS,
	SCREEN_GREENNESS
};

enum Blinker
{
	BLINKER_ADD,
	BLINKER_DELETE,
	BLINKER_UPDATE,
	BLINKER_RUN
};

enum MIDIState
{
	MIDI_NULL,
	MIDI_LOADED,
	MIDI_PLAYING,
	MIDI_STOPPED
};

enum SoundState
{
	SOUND_NULL,
	SOUND_LOADED,
	SOUND_PLAYING,
	SOUND_STOPPED
};

#define PARTICLE_STATE_DEAD            0
#define PARTICLE_STATE_ALIVE           1

#define PARTICLE_TYPE_FICKER           0
#define PARTICLE_TYPE_FADE             1

#define PARTICLE_COLOR_RED             0
#define PARTICLE_COLOR_GREEN           1
#define PARTICLE_COLOR_BLUE            2
#define PARTICLE_COLOR_WHITE           3

#define MAX_PARTICLES                  128

#define INDEX_WORLD_X                  8
#define INDEX_WORLD_Y                  9

#define COLOR_RED_START                32
#define COLOR_RED_END                  47
#define COLOR_GREEN_START              96
#define COLOR_GREEN_END                111
#define COLOR_BLUE_START               144
#define COLOR_BLUE_END                 159
#define COLOR_WHITE_START              16
#define COLOR_WHITE_END                31

#define MAX_SOUNDS                256
#define DM_NUM_SEGMENTS           64
#define DSVOLTODB(vol)           ((DWORD)(-30*(100-vol)))

typedef struct PCM_SOUND
{
	LPDIRECTSOUNDBUFFER dsbuffer;
	int state;
	int rate;
	int size;
	int id;
}PCMSound,*LPPCMSound;

typedef struct DMUSIC_MIDI_TYP
{
	IDirectMusicSegment* dm_segment;
	IDirectMusicSegmentState* dm_segstate;
	int id;
	int state;
}MIDISegment,*LPMIDISegment;

#define PI                       ((float)3.141592654f)
#define	PI2                      ((float)6.283185307f)
#define PI_DIV_2                 ((float)1.570796327f)
#define PI_DIV_4                 ((float)0.785398163f)
#define PI_INV                   ((float)0.318309886f)

#define FIXP16_SHIFT              16
#define FIXP16_MAG                65535
#define FIXP16_DP_MASK            0x0000ffff
#define FIXP16_WP_MASK            0xffff0000
#define FIXP16_ROUND_UP           0x00008000

#define KEYDOWN(vk_code)  ((GetAsyncKeyState(vk_code) & 0x8000)? 1:0)
#define KEYUP(vk_code)    ((GetAsyncKeyState(vk_code) & 0x8000) ? 0:1)

#define _RGB16BIT555(r,g,b)        ((b & 31)+((g & 31)<<5)+((r & 31)<<10))
#define _RGB16BIT565(r,g,b)        ((b & 31)+((g & 63)<<5)+((r & 31)<<11))
#define _RGB24BIT(a,r,g,b)         ((b)+(g<<8)+(r<<16))
#define _RGB32BIT(a,r,g,b)         (b+(g<<8)+(r<<16)+(a<<24))

#define SET_BIT(word,bit_flag)     ((word)=( (word) | (bit_flag)))
#define RESET_BIT(word,bit_flag)   ((word)=( (word) & (~bit_flag)))

#define DDRAW_INIT_STRUCT(ddstruct)  {memset(&ddstruct,0,sizeof(ddstruct));ddstruct.dwSize=sizeof(ddstruct);}

#ifndef DSBCAPS_CTRLDEFAULT
#define DSBCAPS_CTRLDEFAULT (DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME )
#endif

#define ANG_TO_RAD(angle)        ((angle)*PI/180.0)
#define RAD_TO_ANG(radian)       ((radian)*180.0/PI)

#define RAND_RANGE(x,y)          ( (x) +(rand() % ((y)-(x)+1)) )


//灯光闪烁
typedef struct BLINKER_TYP
{
	int color_index;
	PALETTEENTRY on_color;
	PALETTEENTRY off_color;
	int on_time;          //保持开的帧数
	int off_time;         //保持关的帧数
	int counter;         //用于状态转变的计数
	int state;           //-1 off,1 on,0 dead
}BLINKER,*LPBLINKER;

typedef struct VERTEX2D_TYP
{
	float x;
	float y;
}VERTEX2D,*LPVERTEX2D;

inline void swap(int& a,int& b) {int temp;temp=a;a=b;b=temp;}
inline void swap(float& a,float& b) {float temp;temp=a;a=b;b=temp;}
inline int fabs(int& x){if(x<0)x=-x;return x;}
inline void MemSetWORD(void* dest, USHORT data, int count)
{
	_asm
	{
		mov edi,dest;
		mov ecx,count;
		mov ax,data;
		rep stosw;
	}
}
inline void MemSetDWORD(void* dest, DWORD data, int count)
{
	_asm
	{
		mov edi,dest;
		mov ecx,count;
		mov Eax,data;
		rep stosd;
	}
}


inline void PadName(char* filename,char* extension,char* padstring,int num)
{
	char buf[80];
	wsprintf(padstring,"%s0000.%s",filename,extension);
	int digestIndex=strlen(filename)+4;
	_itoa_s(num,buf,10);
	memcpy(padstring+digestIndex-strlen(buf),buf,strlen(buf));
}

inline float Distance2D(float x,float y)
{
	x=fabs(x);
	y=fabs(y);
	int mn=(int)min(x,y);
	return (x+y -(mn>>1)-(mn>>2)+(mn>>4));
}

inline float Distance3D(float x, float y, float z)
{
	int _x = (int)fabs(x) * 1024;
	int _y = (int)fabs(y) * 1024;
	int _z = (int)fabs(z) * 1024;
	if (_y < x) swap(_x,_y);
	if (_z < y) swap(_y,_z);
	if (_y < x) swap(_x,_y);
	int dist = (_z + 11*(_y >> 5) + (_x >> 2) );
	return((float)(dist >> 10));
}



inline void CreateTable()
{
	for (int i=0;i<16;i++)
	{
		float angle=float(PI*i*22.5/180);
		cosTable[i]=-cos(angle+PI/2);
		sinTable[i]=-sin(angle+PI/2);
	}
}
inline BOOL ColliCheck(int x1, int y1, int w1, int h1, 
					   int x2, int y2, int w2, int h2)
{
	int width1  = (w1>>1) - (w1>>3);
	int height1 = (h1>>1) - (h1>>3);

	int width2  = (w2>>1) - (w2>>3);
	int height2 = (h2>>1) - (h2>>3);

	int cx1 = x1 + width1; 
	int cy1 = y1 + height1;

	int cx2 = x2 + width2;
	int cy2 = y2 + height2;

	int dx = abs(cx2 - cx1);
	int dy = abs(cy2 - cy1);

	if (dx < (width1+width2) && dy < (height1+height2))
		return TRUE;
	else

		return FALSE;
}

#endif