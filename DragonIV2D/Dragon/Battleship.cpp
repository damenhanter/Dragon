#include "Burst.h"
#include "dh.h"
#include "plasma.h"
#include "Battleship.h"



extern CRenderSystem render;
extern CParticleSystem ps;
extern CBurst burst;
extern CDH dh;
extern CPlasma plasma;
extern float cosTable[16];
extern float sinTable[16];
extern CErrorLog errorlog;

CBattleship::CBattleship(void)
{
}

CBattleship::~CBattleship(void)
{

}


void CBattleship::InitBattleShip()
{
	battleships[0].CreateAnimation(&render,0,0,200,200,16,ANIM_ATTR_VISIBLE|ANIM_ATTR_MULTI_FRAME,
		DDSCAPS_VIDEOMEMORY,0,SCREEN_BPP);
	char buf[80];
	CBitmapFile bitmap16bit;
	for (int i=0;i<16;i++)
	{
		PadName("../res/picture/battleship","bmp",buf,i);
		bitmap16bit.LoadBitmapFromFile(buf);
		battleships[0].LoadAnimFrame16(&bitmap16bit,i,0,0,BITMAP_EXTRACT_MODE_ABS);
		bitmap16bit.UnLoadBitmapFile();
	}
	
	battleships[0].state=BATTLESHIP_STATE_DEAD;
	battleships[0].varsI[INDEX_BATTLESHIP_TURRET]=0;
	battleships[0].varsI[INDEX_BATTLESHIP_DAMAGE]=0;
	battleships[0].varsI[INDEX_BATTLESHIP_DIR]=0;

	for (int i=1;i<MAX_BATTLESHIPS;i++)
	{
		battleships[i].CloneAnim(&battleships[0]);
	}
}

void CBattleship::ResetBattleShip()
{
	for (int i=0;i<MAX_BATTLESHIPS;i++)
	{
		battleships[0].state=BATTLESHIP_STATE_DEAD;
		battleships[0].varsI[INDEX_BATTLESHIP_TURRET]=0;
		battleships[0].varsI[INDEX_BATTLESHIP_DAMAGE]=0;
		battleships[0].varsI[INDEX_BATTLESHIP_DIR]=0;
	}
}

void CBattleship::StartBattleShip()
{
	for (int i=0;i<MAX_BATTLESHIPS;i++)
	{
		if (battleships[i].state==BATTLESHIP_STATE_DEAD)
		{
			//战列舰的位置在以玩家为中心的环形圈附近
			int angle=rand()%16;
			float xpos=BATTLESHIP_RANGE_RING*cosTable[angle];
			float ypos=BATTLESHIP_RANGE_RING*sinTable[angle];

			battleships[i].varsI[INDEX_WORLD_X]=int(dh.GetX()+xpos);
			battleships[i].varsI[INDEX_WORLD_Y]=int(dh.GetY()+ypos);
	
			battleships[i].varsI[INDEX_BATTLESHIP_DIR]=RAND_RANGE(0,15);
			battleships[i].xv=cosTable[battleships[i].varsI[INDEX_BATTLESHIP_DIR]]*BATTLESHIP_SPEED;
			battleships[i].yv=sinTable[battleships[i].varsI[INDEX_BATTLESHIP_DIR]]*BATTLESHIP_SPEED;

			battleships[i].varsI[INDEX_BATTLESHIP_TURRET]=0;
			battleships[i].varsI[INDEX_BATTLESHIP_DAMAGE]=0;
			battleships[i].state=BATTLESHIP_STATE_ALIVE;
			
			return;
		}
	}
}

void CBattleship::MoveBattleShip()
{
	for (int i=0;i<MAX_BATTLESHIPS;i++)
	{
		if (battleships[i].state==BATTLESHIP_STATE_ALIVE)
		{
			battleships[i].varsI[INDEX_WORLD_X]+=int(battleships[i].xv);
			battleships[i].varsI[INDEX_WORLD_Y]+=int(battleships[i].yv);

			if (battleships[i].varsI[INDEX_WORLD_X]>=UNIVERSE_MAX_X)
			{
				battleships[i].varsI[INDEX_WORLD_X]=UNIVERSE_MIN_X;
			}
			else if (battleships[i].varsI[INDEX_WORLD_X]<=UNIVERSE_MIN_X)
			{
				battleships[i].varsI[INDEX_WORLD_X]=UNIVERSE_MAX_X;
			}
			if (battleships[i].varsI[INDEX_WORLD_Y]>=UNIVERSE_MAX_Y)
			{
				battleships[i].varsI[INDEX_WORLD_Y]=UNIVERSE_MIN_Y;
			}
			else if (battleships[i].varsI[INDEX_WORLD_Y]<=UNIVERSE_MIN_Y)
			{
				battleships[i].varsI[INDEX_WORLD_Y]=UNIVERSE_MAX_Y;
			}
			//战舰受伤害程度
			if (battleships[i].varsI[INDEX_BATTLESHIP_DAMAGE]>(MAX_BATTLESHIP_DAMAGE>>2)&&
				rand()%3==1)
			{
				ps.StartParticle(PARTICLE_TYPE_FICKER,PARTICLE_COLOR_WHITE,30+rand()%25,
					battleships[i].varsI[INDEX_WORLD_X]+RAND_RANGE(-4,4),battleships[i].varsI[INDEX_WORLD_Y]+RAND_RANGE(-4,4),
					(int)battleships[i].xv>>3,(int)battleships[i].yv>>3);

				ps.StartParticle(PARTICLE_TYPE_FADE,PARTICLE_COLOR_RED,5,
					battleships[i].varsI[INDEX_WORLD_X]+RAND_RANGE(-4,4),battleships[i].varsI[INDEX_WORLD_Y]+RAND_RANGE(-4,4),
					(int)battleships[i].xv>>3,(int)battleships[i].yv>>3);
			}
			//做AI
			if (Distance2D(dh.GetX()-battleships[i].varsI[INDEX_WORLD_X],
				dh.GetY()-battleships[i].varsI[INDEX_WORLD_Y])<BATTLESHIP_MIN_ATTACK_RANGE)
			{
				//移动战列舰
				if (dh.GetX()>battleships[i].varsI[INDEX_WORLD_X])
				{
					battleships[i].varsI[INDEX_WORLD_X]++;
				}
				else if (dh.GetX()<battleships[i].varsI[INDEX_WORLD_X])
				{
					battleships[i].varsI[INDEX_WORLD_X]--;
				}
				if (dh.GetY()>battleships[i].varsI[INDEX_WORLD_Y])
				{
					battleships[i].varsI[INDEX_WORLD_Y]++;
				}
				else if (dh.GetY()<battleships[i].varsI[INDEX_WORLD_Y])
				{
					battleships[i].varsI[INDEX_WORLD_Y]--;
				}

				int tdir1=battleships[i].varsI[INDEX_BATTLESHIP_TURRET];
				float d1x=battleships[i].varsI[INDEX_WORLD_X]+cosTable[tdir1]*32;
				float d1y=battleships[i].varsI[INDEX_WORLD_Y]+sinTable[tdir1]*32;

				int tdir2=battleships[i].varsI[INDEX_BATTLESHIP_TURRET]+1;
				if (tdir2>15)
				{
					tdir2=0;
				}
				float d2x=battleships[i].varsI[INDEX_WORLD_X]+cosTable[tdir2]*32;
				float d2y=battleships[i].varsI[INDEX_WORLD_Y]+sinTable[tdir2]*32;

				int tdir0=battleships[i].varsI[INDEX_BATTLESHIP_TURRET]-1;
				if (tdir0<0)
				{
					tdir0=15;
				}
				float d0x=battleships[i].varsI[INDEX_WORLD_X]+cosTable[tdir0]*32;
				float d0y=battleships[i].varsI[INDEX_WORLD_Y]+sinTable[tdir0]*32;

				//寻找最小距离
				float dist0=Distance2D(dh.GetX()-d0x,dh.GetY()-d0y);
				float dist1=Distance2D(dh.GetX()-d1x,dh.GetY()-d1y);
				float dist2=Distance2D(dh.GetX()-d2x,dh.GetY()-d2y);

				if (dist0<dist2 && dist0<dist1)
				{
					battleships[i].varsI[INDEX_BATTLESHIP_TURRET]=tdir0;
				}
				else if (dist2<dist0 && dist2<dist1)
				{
					battleships[i].varsI[INDEX_BATTLESHIP_TURRET]=tdir2;
				}
				if ((rand()%5)==1)
				{
						int plasma_x=battleships[i].varsI[INDEX_WORLD_X];
						int plasma_y=battleships[i].varsI[INDEX_WORLD_Y];

						float plasma_xv=battleships[i].xv+cosTable[battleships[i].varsI[INDEX_BATTLESHIP_TURRET]]*PLASMA_SPEED_SLOW;
						float plasma_yv=battleships[i].yv+cosTable[battleships[i].varsI[INDEX_BATTLESHIP_TURRET]]*PLASMA_SPEED_SLOW;
						
						//炮弹方向
						int dir=battleships[i].varsI[INDEX_BATTLESHIP_TURRET];
						plasma.FirePlasma(plasma_x,plasma_y,(int)plasma_xv,(int)plasma_yv,dir,PLASMA_ANIM_ENEMY);
				
				}
			}
		}
	}
}

void CBattleship::DrawBattleShip()
{
	//static ;
	for (int i=0;i<MAX_BATTLESHIPS;i++)
	{
		if (battleships[i].state==BATTLESHIP_STATE_ALIVE)
		{
			battleships[i].x=battleships[i].varsI[INDEX_WORLD_X]-(battleships[i].width>>1)+(SCREEN_WIDTH>>1)-dh.GetX();
			battleships[i].y=battleships[i].varsI[INDEX_WORLD_Y]-(battleships[i].height>>1)+(SCREEN_HEIGHT>>1)-dh.GetY();

			battleships[i].currFrame=battleships[i].varsI[INDEX_BATTLESHIP_DIR];
			
			/*errorlog.WriteError("battleship[%d].x=%d,battleship[%d].y=%d,currFrame=%d\n",
				i,battleships[i].x,i,battleships[i].y,battleships[i].currFrame);*/
			
			battleships[i].RenderAnimation(&render,render.lpddsback);
		}
	}
}