#include "DH.h"
#include "dragon.h"

extern CRenderSystem render;

CDH::CDH(void)
:x(0),y(0),xv(0),yv(0)
,state(3)
,damage(0)
,score(0)
,shield(0)
,dx(0)
,dy(0)
,ammo(0)
{
}

CDH::~CDH(void)
{
}

void CDH::ResetPos()
{
	x=y=0;
	xv=yv=0;
	dx=dy=0;
	meteor.varsI[3]=0;
}


void CDH::InitDH()
{
	meteor.CreateAnimation(&render,0,0,200,200,35,ANIM_ATTR_MULTI_FRAME|ANIM_ATTR_VISIBLE,
						DDSCAPS_VIDEOMEMORY,0,SCREEN_BPP);
	CBitmapFile bitmap16bit;
	char buf[80];
	for (int i=0;i<16;i++)
	{
		PadName("../res/picture/dh","bmp",buf,i);
		bitmap16bit.LoadBitmapFromFile(buf);
		meteor.LoadAnimFrame16(&bitmap16bit,i,0,0,BITMAP_EXTRACT_MODE_ABS);
		bitmap16bit.UnLoadBitmapFile();
	}

	for (int i=0;i<16;i++)
	{
		PadName("../res/picture/dhe","bmp",buf,i);
		bitmap16bit.LoadBitmapFromFile(buf);
		meteor.LoadAnimFrame16(&bitmap16bit,i+16,0,0,BITMAP_EXTRACT_MODE_ABS);
		bitmap16bit.UnLoadBitmapFile();
	}

	bitmap16bit.LoadBitmapFromFile("../res/picture/shield.bmp");
	for (int i=0;i<3;i++)
	{
		meteor.LoadAnimFrame16(&bitmap16bit,32+i,0,0,BITMAP_EXTRACT_MODE_ABS);
	}
	bitmap16bit.UnLoadBitmapFile();

	meteor.SetPos((float)(SCREEN_WIDTH>>1)-(meteor.width>>1),(float)(SCREEN_HEIGHT>>1)-(meteor.height>>1));
	meteor.varsI[INDEX_METEOR_DIR]=0;

}