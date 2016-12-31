#include "Powerup.h"
#include "dh.h"
extern CRenderSystem render;
extern CDH dh;
extern CDSound dsound;

CPowerup::CPowerup(void)
{
	ammoId=0;
	shldId=0;
}

CPowerup::~CPowerup(void)
{

}

void CPowerup::SetAmmoId(int id)
{
	ammoId=id;
}

int& CPowerup::GetAmmoId()
{
	return ammoId;
}

void CPowerup::SetShldId(int id)
{
	shldId=id;
}

int& CPowerup::GetShldId()
{
	return shldId;
}

void CPowerup::InitPowerup()
{
	powerups[0].CreateAnimation(&render,0,0,50,50,3,ANIM_ATTR_VISIBLE|ANIM_ATTR_MULTI_FRAME,
		DDSCAPS_VIDEOMEMORY,0,SCREEN_BPP);
	char buf[80];
	PadName("../res/picture/ammo","bmp",buf,0);
	CBitmapFile bitmap16bit;
	bitmap16bit.LoadBitmapFromFile(buf);
	powerups[0].LoadAnimFrame16(&bitmap16bit,0,0,0,BITMAP_EXTRACT_MODE_ABS);
	bitmap16bit.UnLoadBitmapFile();
	
	PadName("../res/picture/shield","bmp",buf,0);
	bitmap16bit.LoadBitmapFromFile(buf);
	powerups[0].LoadAnimFrame16(&bitmap16bit,1,0,0,BITMAP_EXTRACT_MODE_ABS);
	bitmap16bit.UnLoadBitmapFile();
	
	PadName("../res/picture/source","bmp",buf,0);
	bitmap16bit.LoadBitmapFromFile(buf);
	powerups[0].LoadAnimFrame16(&bitmap16bit,2,0,0,BITMAP_EXTRACT_MODE_ABS);
	bitmap16bit.UnLoadBitmapFile();

	powerups[0].state=POWERUP_STATE_DEAD;
	powerups[0].varsI[INDEX_POWERUPS_TYPE]=POWERUP_TYPE_AMMO;

	for (int i=1;i<MAX_POWERUPS;i++)
	{
		powerups[i].CloneAnim(&powerups[0]);
	}
}

void CPowerup::ResetPowerup()
{
	for (int i=0;i<MAX_POWERUPS;i++)
	{
		powerups[i].state=POWERUP_STATE_DEAD;
		powerups[i].varsI[INDEX_POWERUPS_TYPE]=POWERUP_TYPE_AMMO;
	}
}

void CPowerup::StartPowerup(int type, int x, int y)
{
	for (int i=0;i<MAX_POWERUPS;i++)
	{
		if (powerups[i].state==POWERUP_STATE_DEAD)
		{
			powerups[i].varsI[INDEX_WORLD_X] = x;
			powerups[i].varsI[INDEX_WORLD_Y] = y;

			powerups[i].xv = RAND_RANGE(-2,2);
			powerups[i].yv = RAND_RANGE(-2,2);

			powerups[i].state = POWERUP_STATE_ALIVING;
			powerups[i].varsI[INDEX_POWERUPS_TYPE] = type;

			// 设置生命周期
			powerups[i].counter1 = RAND_RANGE(200,300);

			return ;
		}
	}
}

void CPowerup::MovePowerup()
{
	for (int i=0;i<MAX_POWERUPS;i++)
	{
		if (powerups[i].state==POWERUP_STATE_ALIVING)
		{
			if (ColliCheck(dh.GetX()-(dh.meteor.width>>1),dh.GetY()-(dh.meteor.height>>1),
				dh.meteor.width,dh.meteor.height,
				powerups[i].varsI[INDEX_WORLD_X]-(powerups[i].width>>1),powerups[i].varsI[INDEX_WORLD_Y]-(powerups[i].height>>1),
				powerups[i].width,powerups[i].height))
			{
				if (powerups[i].varsI[INDEX_POWERUPS_TYPE]==POWERUP_TYPE_AMMO)
				{
					dh.SetAmmo(dh.GetAmmo()+1000);
					dsound.play(ammoId,0);
				}
				else if (powerups[i].varsI[INDEX_POWERUPS_TYPE]==POWERUP_TYPE_SHLD)
				{
					dh.SetDamage(dh.GetDamage()-50);
					if (dh.GetDamage()<0)
					{
						dh.SetDamage(0);
					}
					dsound.play(shldId,0);
				}
				else if (powerups[i].varsI[INDEX_POWERUPS_TYPE]==POWERUP_TYPE_SOURCE)
				{
					dh.SetState(PLAYER_STATE_WIN);
				}

				powerups[i].state=POWERUP_STATE_DEAD;
			}

			powerups[i].varsI[INDEX_WORLD_X]+=powerups[i].xv;
			powerups[i].varsI[INDEX_WORLD_Y]+=powerups[i].yv;

			if (--powerups[i].counter1<0)
			{	
				powerups[i].state=POWERUP_STATE_DEAD;
			}

			if (powerups[i].varsI[INDEX_WORLD_X]>UNIVERSE_MAX_X)
			{
				powerups[i].varsI[INDEX_WORLD_X]=UNIVERSE_MIN_X;
			}
			else if (powerups[i].varsI[INDEX_WORLD_X]<UNIVERSE_MIN_X)
			{
				powerups[i].varsI[INDEX_WORLD_X]=UNIVERSE_MAX_X;
			}
			if (powerups[i].varsI[INDEX_WORLD_Y]>UNIVERSE_MAX_Y)
			{
				powerups[i].varsI[INDEX_WORLD_Y]=UNIVERSE_MIN_Y;
			}
			else if (powerups[i].varsI[INDEX_WORLD_Y]<UNIVERSE_MIN_Y)
			{
				powerups[i].varsI[INDEX_WORLD_Y]=UNIVERSE_MAX_Y;
			}
		}
	}
}

void CPowerup::DrawPowerup()
{
	for (int i=0;i<MAX_POWERUPS;i++)
	{
		if (powerups[i].state==POWERUP_STATE_ALIVING)
		{
			powerups[i].x=powerups[i].varsI[INDEX_WORLD_X]-dh.GetX()-(powerups[i].width>>1)+(SCREEN_WIDTH>>1);
			powerups[i].y=powerups[i].varsI[INDEX_WORLD_Y]-dh.GetY()-(powerups[i].height>>1)+(SCREEN_HEIGHT>>1);

			powerups[i].currFrame=powerups[i].varsI[INDEX_POWERUPS_TYPE];

			powerups[i].RenderAnimation(&render,render.lpddsback);
		}
	}
}