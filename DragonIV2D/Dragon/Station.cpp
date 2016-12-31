#include "Station.h"
#include "DH.h"
#include "Mine.h"
#include "Burst.h"

extern CDH dh;
extern CRenderSystem render;
extern CMine mine;
extern CBurst burst;
extern CParticleSystem ps;
extern float cosTable[16];
extern float sinTable[16];

CStation::CStation(void)
:id(-1)
,NumDamageStation(0)
{
}

CStation::~CStation(void)
{

}

void CStation::SetId(int id)
{
	this->id=id;
}

int& CStation::GetId()
{
	return id;
}

void CStation::SetNumDamageStation(int num)
{
	NumDamageStation=num;
}

int& CStation::GetNumDamageStation()
{
	return NumDamageStation;
}
void CStation::InitStation()
{
	static int shields_on_anim[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	static int shields_off_anim[1] = {16};
	stations[0].CreateAnimation(&render,0,0,300,300,17,ANIM_ATTR_VISIBLE|ANIM_ATTR_MULTI_ANIM,
		DDSCAPS_VIDEOMEMORY,0,SCREEN_BPP);
	CBitmapFile bitmap16bit;
	char buf[80];
	for (int i=0;i<17;i++)
	{
		PadName("../res/picture/station","bmp",buf,i);
		bitmap16bit.LoadBitmapFromFile(buf);
		stations[0].LoadAnimFrame16(&bitmap16bit,i,0,0,BITMAP_EXTRACT_MODE_ABS);
		bitmap16bit.UnLoadBitmapFile();
	}
	stations[0].state=STATION_STATE_ALIVE;
	stations[0].animState=STATION_SHIELDS_ANIM_ON;
	stations[0].varsI[INDEX_STATION_DAMAGE]=0;
	stations[0].SetAnimSpeed(15);
	stations[0].LoadAnimation(STATION_SHIELDS_ANIM_ON,16,shields_on_anim);
	stations[0].LoadAnimation(STATION_SHIELDS_ANIM_OFF,1,shields_off_anim);
	stations[0].SetAnimation(STATION_SHIELDS_ANIM_ON);
	for (int i=1;i<MAX_STATIONS;i++)
	{
		stations[i].CloneAnim(&stations[0]);
	}

	SmallStations.CreateAnimation(&render,0,0,10,10,2,ANIM_ATTR_VISIBLE|ANIM_ATTR_MULTI_FRAME,
		DDSCAPS_VIDEOMEMORY,0,SCREEN_BPP);

	bitmap16bit.LoadBitmapFromFile("../res/picture/SmallStation1.BMP");
	SmallStations.LoadAnimFrame16(&bitmap16bit,0,0,0,BITMAP_EXTRACT_MODE_ABS);
	bitmap16bit.UnLoadBitmapFile();


	bitmap16bit.LoadBitmapFromFile("../res/picture/SmallStation2.BMP");
	SmallStations.LoadAnimFrame16(&bitmap16bit,1,0,0,BITMAP_EXTRACT_MODE_ABS);
	bitmap16bit.UnLoadBitmapFile();
	

}

void CStation::ResetStation()
{
	for (int i=0;i<MAX_STATIONS;i++)
	{
		stations[i].state=STATION_STATE_DEAD;
		stations[i].animState=STATION_SHIELDS_ANIM_ON;
		stations[i].varsI[INDEX_STATION_DAMAGE]=0;
	}
}

void CStation::StartStation(int override,int x,int y)
{
	for (int i=0;i<MAX_STATIONS;i++)
	{
		if (stations[i].state==STATION_STATE_DEAD)
		{
			int xpos = RAND_RANGE((UNIVERSE_MIN_X+1500),(UNIVERSE_MAX_X-1500));
			int ypos = RAND_RANGE((UNIVERSE_MIN_Y+1500),(UNIVERSE_MAX_Y-1500));
			//设置位置
			stations[i].varsI[INDEX_WORLD_X]=xpos;
			stations[i].varsI[INDEX_WORLD_Y]=ypos;

			int angle=rand()%16;
			int mineX=xpos+STATION_RANGE_RING*cosTable[angle];
			int mineY=ypos+STATION_RANGE_RING*sinTable[angle];

			mine.StartMine(1,mineX,mineY,MINE_STATE_AI_SLEEP);
			stations[i].xv=0;
			stations[i].yv=0;

			stations[i].state=STATION_STATE_ALIVE;
			stations[i].SetAnimation(STATION_SHIELDS_ANIM_ON);
			stations[i].animState=STATION_SHIELDS_ANIM_ON;
			stations[i].varsI[INDEX_STATION_DAMAGE]=0;
			
			return ;
		}
	}
}

void CStation::MoveStation()
{
	for (int i=0;i<MAX_STATIONS;i++)
	{
		if (stations[i].state==STATION_STATE_ALIVE)
		{
			stations[i].varsI[INDEX_WORLD_X]+=stations[i].xv;
			stations[i].varsI[INDEX_WORLD_Y]+=stations[i].yv;

			if (stations[i].varsI[INDEX_WORLD_X] > UNIVERSE_MAX_X)
			{
				stations[i].varsI[INDEX_WORLD_X] = UNIVERSE_MIN_X;
			}
			else if (stations[i].varsI[INDEX_WORLD_X] < UNIVERSE_MIN_X )
			{
				stations[i].varsI[INDEX_WORLD_X] = UNIVERSE_MAX_X;
			}     

			if (stations[i].varsI[INDEX_WORLD_Y] > UNIVERSE_MAX_Y)
			{
				stations[i].varsI[INDEX_WORLD_Y] = UNIVERSE_MIN_Y;
			}
			else if (stations[i].varsI[INDEX_WORLD_Y] < UNIVERSE_MIN_Y ) 
			{
				stations[i].varsI[INDEX_WORLD_Y] = UNIVERSE_MAX_Y;
			}
			//测试伤害级别
			if ((stations[i].varsI[INDEX_STATION_DAMAGE]>MAX_STATION_DAMAGE>>2)&&
				 (rand()%(20 - (stations[i].varsI[INDEX_STATION_DAMAGE] >> 3))) == 1)
			{
				int width=20+rand()%60;
				burst.StartBurst(&ps,stations[i].varsI[INDEX_WORLD_X] - (stations[i].width*.5)+RAND_RANGE(0,stations[i].width),
					stations[i].varsI[INDEX_WORLD_Y] - (stations[i].height*.5)+RAND_RANGE(0,stations[i].height), 
					width,(width >> 2) + (width >> 1),
					int(stations[i].xv)>>1, int(stations[i].yv)>>1); 
			}
		}
	}
}

void CStation::DrawStation()
{
	for (int i=0;i<MAX_STATIONS;i++)
	{
		if (stations[i].state==STATION_STATE_ALIVE)
		{	
			stations[i].x = stations[i].varsI[INDEX_WORLD_X] - (stations[i].width >> 1) - dh.GetX() + (SCREEN_WIDTH/2);
			stations[i].y = stations[i].varsI[INDEX_WORLD_Y] - (stations[i].height >> 1) - dh.GetY() + (SCREEN_HEIGHT/2);

			stations[i].RenderAnimation(&render,render.lpddsback);
			stations[i].DoAnimation();

		}
	
	}
}