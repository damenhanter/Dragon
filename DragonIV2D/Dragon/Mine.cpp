#include "Mine.h"
#include "Mine.h"
#include "burst.h"
#include "dh.h"

extern CRenderSystem render;
extern CParticleSystem ps;
extern CBurst burst;
extern CDH dh;
extern CDSound dsound;



CMine::CMine(void)
:trackingRate(2.0)
,powerupId(-1)
{
}

CMine::~CMine(void)
{

}


void CMine::InitMine()
{
	mines[0].CreateAnimation(&render,0,0,100,100,16,ANIM_ATTR_VISIBLE|ANIM_ATTR_MULTI_FRAME,
		DDSCAPS_VIDEOMEMORY,0,SCREEN_BPP);
	CBitmapFile bitmap16bit;
	char buf[80];
	for (int i=0;i<16;i++)
	{
		PadName("../res/picture/mine","bmp",buf,i);
		bitmap16bit.LoadBitmapFromFile(buf);
		mines[0].LoadAnimFrame16(&bitmap16bit,i,0,0,BITMAP_EXTRACT_MODE_ABS);
		bitmap16bit.UnLoadBitmapFile();
	}

	mines[0].state=MINE_STATE_DEAD;
	//设置AI状态到睡眠
	mines[0].varsI[INDEX_MINE_AI_STATE]=MINE_STATE_AI_SLEEP;
	//设置损坏0
	mines[0].varsI[INDEX_MINE_DAMAGE]=0;
	//设置追踪为0
	mines[0].varsI[INDEX_MINE_CONTRACT_COUNT]=0;
	//设置动画频率
	mines[0].SetAnimSpeed(3);
	
	for (int i=1;i<MAX_MINES;i++)
	{
		mines[i].CloneAnim(&mines[0]);
	}
}

void CMine::ResetMine()
{
	for (int i=0;i<MAX_MINES;i++)
	{
		//设置状态关闭
		mines[i].state=MINE_STATE_DEAD;
		//设置AI状态到睡眠
		mines[i].varsI[INDEX_MINE_AI_STATE]=MINE_STATE_AI_SLEEP;
		//设置损坏0
		mines[i].varsI[INDEX_MINE_DAMAGE]=0;
		//设置追踪为0
		mines[i].varsI[INDEX_MINE_CONTRACT_COUNT]=0;
	}
}

//如果override=1函数使用发送的数据，否则使用随机值
void CMine::StartMine(int override,int x,int y,int AIState)
{
	for (int i=0; i<MAX_MINES;i++)
	{
		if (mines[i].state==MINE_STATE_DEAD)
		{
			if (!override)
			{
				int x=RAND_RANGE((UNIVERSE_MIN_X+256),(UNIVERSE_MAX_X-256));
				int y=RAND_RANGE((UNIVERSE_MIN_Y+256),(UNIVERSE_MAX_Y-256));
				//设置世界坐标
				mines[i].varsI[INDEX_WORLD_X]=x;
				mines[i].varsI[INDEX_WORLD_Y]=y;
				mines[i].varsI[INDEX_MINE_DIR]=0;
				//设置速度
				mines[i].xv=(float)RAND_RANGE(-8,8);
				mines[i].yv=(float)RAND_RANGE(-8,8);

				mines[i].state=MINE_STATE_ALIVE;

				mines[i].varsI[INDEX_MINE_DAMAGE]=0;
				mines[i].varsI[INDEX_MINE_AI_STATE]=MINE_STATE_AI_SLEEP;
				mines[i].varsI[INDEX_MINE_CONTRACT_COUNT]=0;
				return ;
			}
			else
			{
				//设置世界坐标
				mines[i].varsI[INDEX_WORLD_X]=x;
				mines[i].varsI[INDEX_WORLD_Y]=y;
				//设置速度
				mines[i].xv=0;
				mines[i].yv=0;

				mines[i].state=MINE_STATE_ALIVE;

				mines[i].varsI[INDEX_MINE_DAMAGE]=0;
				mines[i].varsI[INDEX_MINE_AI_STATE]=AIState;
				mines[i].varsI[INDEX_MINE_CONTRACT_COUNT]=0;
				return ;
			}
		}
	}
}

void CMine::MoveMine()
{
	for (int i=0;i<MAX_MINES;i++)
	{
		if (mines[i].state==MINE_STATE_ALIVE)
		{
			if (mines[i].varsI[INDEX_MINE_AI_STATE]==MINE_STATE_AI_ACTIVATED)
			{
				//移动雷
				mines[i].varsI[INDEX_WORLD_X]+=(int)mines[i].xv;
				mines[i].varsI[INDEX_WORLD_Y]+=(int)mines[i].yv;
				//添加毁坏的痕迹
				if (mines[i].varsI[INDEX_MINE_DAMAGE]>(MAX_MINE_DAMAGE>>1) && (rand()%3)==1)
				{
					ps.StartParticle(PARTICLE_TYPE_FICKER,PARTICLE_COLOR_WHITE,30+rand()%25,
						mines[i].varsI[INDEX_WORLD_X]+RAND_RANGE(-4,4),
						mines[i].varsI[INDEX_WORLD_Y]+RAND_RANGE(-4,4),
						(int)(mines[i].xv/8),(int)(mines[i].yv/8));
					ps.StartParticle(PARTICLE_TYPE_FADE,PARTICLE_COLOR_RED,5,
						mines[i].varsI[INDEX_WORLD_X]+RAND_RANGE(-4,4),
						mines[i].varsI[INDEX_WORLD_Y]+RAND_RANGE(-4,4),
						int(mines[i].xv/8),int(mines[i].yv/8));
				}

				//追踪算法
				//计算追踪雷到玩家的方向向量
				float vx=float(dh.GetX()-mines[i].varsI[INDEX_WORLD_X]);
				float vy=float(dh.GetY()-mines[i].varsI[INDEX_WORLD_Y]);
				float length=Distance2D(vx,vy);
				//追踪雷在一定进的距离进行追踪
				if(length<MIN_MINE_TRACKING_DIST)
				{
					//执行追踪算法
					vx=trackingRate*vx/length;
					vy=trackingRate*vy/length;

					mines[i].xv+=vx;
					mines[i].yv+=vy;

					if (rand()%10==1)
					{
						vx=(float)RAND_RANGE(-1,1);
						vy=(float)RAND_RANGE(-1,1);
						mines[i].xv+=vx;
						mines[i].yv+=vy;
					}

					length=Distance2D(mines[i].xv,mines[i].yv);
					//检查是否超速
					if (length>MAX_MINE_VELOCITY)
					{
						mines[i].xv*=0.75;
						mines[i].yv*=0.75;
					}
					
					//寻找雷的追踪方向
					int tdir1=mines[i].varsI[INDEX_MINE_DIR];
					float d1x=mines[i].varsI[INDEX_WORLD_X]+cosTable[tdir1]*32;
					float d1y=mines[i].varsI[INDEX_WORLD_Y]+sinTable[tdir1]*32;

					int tdir2=mines[i].varsI[INDEX_MINE_DIR]+1;
					if (tdir2>15)
					{
						tdir2=0;
					}
					float d2x=mines[i].varsI[INDEX_WORLD_X]+cosTable[tdir2]*32;
					float d2y=mines[i].varsI[INDEX_WORLD_Y]+sinTable[tdir2]*32;

					int tdir0=mines[i].varsI[INDEX_MINE_DIR]-1;
					if (tdir0<0)
					{
						tdir0=15;
					}
					float d0x=mines[i].varsI[INDEX_WORLD_X]+cosTable[tdir0]*32;
					float d0y=mines[i].varsI[INDEX_WORLD_Y]+sinTable[tdir0]*32;

					//寻找最小距离
					float dist0=Distance2D(dh.GetX()-d0x,dh.GetY()-d0y);
					float dist1=Distance2D(dh.GetX()-d1x,dh.GetY()-d1y);
					float dist2=Distance2D(dh.GetX()-d2x,dh.GetY()-d2y);

					if (dist0<dist2 && dist0<dist1)
					{
						mines[i].varsI[INDEX_MINE_DIR]=tdir0;
					}
					else if (dist2<dist0 && dist2<dist1)
					{
						mines[i].varsI[INDEX_MINE_DIR]=tdir2;
					}
				}
				else
				{
					if (rand()%30==1)
					{
						//增加一个随机速度
						vx=(float)RAND_RANGE(-2,2);
						vy=(float)RAND_RANGE(-2,2);
						
						mines[i].xv+=vx;
						mines[i].yv+=vy;

						length=Distance2D(mines[i].xv,mines[i].yv);
						//检查是否超速
						if (length>MAX_MINE_VELOCITY)
						{
							mines[i].xv*=0.75;
							mines[i].yv*=0.75;
						}

					}
				}
			}	
			else
			{
				//计算雷到玩家的距离
				float vx=float(dh.GetX()-mines[i].varsI[INDEX_WORLD_X]);
				float vy=float(dh.GetY()-mines[i].varsI[INDEX_WORLD_Y]);
				float length=Distance2D(vx,vy);
				if (length<MIN_MINE_ACTIVATION_DIST)
				{
					mines[i].varsI[INDEX_MINE_AI_STATE]=MINE_STATE_AI_ACTIVATED;
					//dsound.SetFrequence(powerupId,22050);
					dsound.play(powerupId,0);
				}
			}

			if (mines[i].varsI[INDEX_WORLD_X] > UNIVERSE_MAX_X)
			{
				mines[i].varsI[INDEX_WORLD_X] = UNIVERSE_MIN_X;
			} 
			else if (mines[i].varsI[INDEX_WORLD_X] < UNIVERSE_MIN_X )
			{
				mines[i].varsI[INDEX_WORLD_X] = UNIVERSE_MAX_X;
			}  

			if (mines[i].varsI[INDEX_WORLD_Y] > UNIVERSE_MAX_Y)
			{
				mines[i].varsI[INDEX_WORLD_Y] = UNIVERSE_MIN_Y;
			}
			else if (mines[i].varsI[INDEX_WORLD_Y] < UNIVERSE_MIN_Y ) 
			{
				mines[i].varsI[INDEX_WORLD_Y] = UNIVERSE_MAX_Y;
			} 
			//检查和玩家的碰撞检测		
			
			if(dh.GetState()==PLAYER_STATE_ALIVE&&
				ColliCheck(mines[i].varsI[INDEX_WORLD_X]-(mines[i].width>>1),mines[i].varsI[INDEX_WORLD_Y]-(mines[i].height>>1),
				mines[i].width,mines[i].height,
				dh.GetX()-(dh.meteor.width>>1),dh.GetY()+(dh.meteor.height>>1),
				dh.meteor.width,dh.meteor.height))
			{
				if (++mines[i].varsI[INDEX_MINE_CONTRACT_COUNT]>MAX_MINE_CONTRACT_COUNT)
				{
					dh.SetScore(dh.GetScore()+250);
					dh.SetDamage(dh.GetDamage()+30);
					dh.SetShield(3);
					int width=30+rand()%40;
					burst.StartBurst(&ps,mines[i].varsI[INDEX_WORLD_X],
						mines[i].varsI[INDEX_WORLD_Y],
						width,width*0.75,(int)mines[i].xv>>1,(int)mines[i].yv>>1);
					mines[i].state=MINE_STATE_DEAD;
					StartMine();
					continue;
				}
			}
			else
			{
				mines[i].varsI[INDEX_MINE_CONTRACT_COUNT]=0;
			}
		}
	}
}

void CMine::DrawMine()
{
	for (int i=0;i<MAX_MINES;i++)
	{
		if (mines[i].state=MINE_STATE_ALIVE)
		{
			mines[i].x=mines[i].varsI[INDEX_WORLD_X]-dh.GetX()+(SCREEN_WIDTH>>1)-(mines[i].width>>1);
			mines[i].y=mines[i].varsI[INDEX_WORLD_Y]-dh.GetY()+(SCREEN_HEIGHT>>1)-(mines[i].height>>1);
			
			mines[i].currFrame=mines[i].varsI[INDEX_MINE_DIR];
			mines[i].RenderAnimation(&render,render.lpddsback);
		}
	}
}

