#include "Hud.h"
extern CRenderSystem render;

CHud::CHud(void)
{
}

CHud::~CHud(void)
{
}

void CHud::InitHud()
{
	huds.CreateAnimation(&render,0,0,40,40,4,ANIM_ATTR_VISIBLE|ANIM_ATTR_MULTI_FRAME,
		DDSCAPS_VIDEOMEMORY,0,SCREEN_BPP);
	CBitmapFile bitmap16bit;
	char buf[80];
	for (int i=0;i<4;i++)
	{
		PadName("../res/picture/hud","bmp",buf,i);
		bitmap16bit.LoadBitmapFromFile(buf);
		huds.LoadAnimFrame16(&bitmap16bit,i,0,0,BITMAP_EXTRACT_MODE_ABS);
		bitmap16bit.UnLoadBitmapFile();
	}

}