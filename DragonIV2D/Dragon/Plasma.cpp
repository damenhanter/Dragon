#include "Plasma.h"
#include "DH.h"
#include "mine.h"
#include "Burst.h"
#include "Battleship.h"
#include "Station.h"
#include "rock.h"
#include "Powerup.h"

extern CDH dh;
extern CRenderSystem render;
extern CParticleSystem ps;
extern CMine mine;
extern CBurst burst;
extern CBattleship battleship;
extern CStation station;
extern CRock rock;
extern CDSound dsound;
extern HWND hWnd;
extern CPowerup powerup;

CPlasma::CPlasma(void)
{
	for (int i=0;i<MAX_FIRE_SOUND;i++)
	{
		fireId[i] =-1;
	}
}

CPlasma::~CPlasma(void)
{

}

void CPlasma::SetFireId(int index, int id)
{
	fireId[index]=id;
}

int& CPlasma::GetFireId(int index)
{
	return fireId[index];
}

void CPlasma::InitPlasma()
{
	//激光炮弹动画
	static int plasma_anim_player[16][3] = {{0,0,0},{1,1,1},{2,2,2},{3,3,3},{4,4,4},
	{5,5,5},{6,6,6},{7,7,7},{8,8,8},{9,9,9},{10,10,10},{11,11,11},{12,12,12},{13,13,13},
	{14,14,14},{15,15,15}};
	static int plasma_anim_enemy[] = {16,16,16};
	plasmas[0].CreateAnimation(&render,0,0,40,40,17,ANIM_ATTR_MULTI_ANIM|ANIM_ATTR_VISIBLE,
		DDSCAPS_VIDEOMEMORY,0,SCREEN_BPP);
	CBitmapFile bitmap16bit;
	char buf[80];
	for (int i=0;i<16;i++)
	{
		PadName("../res/picture/bulletP","bmp",buf,i);
		bitmap16bit.LoadBitmapFromFile(buf);
		plasmas[0].LoadAnimFrame16(&bitmap16bit,i,0,0,BITMAP_EXTRACT_MODE_ABS);
		bitmap16bit.UnLoadBitmapFile();

		plasmas[0].LoadAnimation(i,3,plasma_anim_player[i]);
	}
	PadName("../res/picture/bulletA","bmp",buf,0);
	bitmap16bit.LoadBitmapFromFile(buf);
	plasmas[0].LoadAnimFrame16(&bitmap16bit,16,0,0,BITMAP_EXTRACT_MODE_ABS);
	bitmap16bit.UnLoadBitmapFile();

	plasmas[0].LoadAnimation(16,3,plasma_anim_enemy);


	plasmas[0].SetAnimSpeed(1);
	plasmas[0].state=PLASMA_STATE_OFF;

	for(int i=1;i<MAX_PLASMA;i++)
	{
		plasmas[i].CloneAnim(&plasmas[0]);
	}
	
}

void CPlasma::ResetPlasma()
{
	for (int i=0;i<MAX_PLASMA;i++)
	{
		plasmas[i].state=PLASMA_STATE_OFF;
	}
}

void CPlasma::MovePlasma()
{
	//移动所有的激光炮弹并检查碰撞检测
	for (int i=0;i<MAX_PLASMA;i++)
	{
		//检查炮弹是否在飞行
		if (plasmas[i].state==PLASMA_STATE_ON)
		{
			plasmas[i].varsI[INDEX_WORLD_X]+=plasmas[i].xv;
			plasmas[i].varsI[INDEX_WORLD_Y]+=plasmas[i].yv;
			
			if ((++plasmas[i].counter1>PLASMA_RANGE_1)||
				(plasmas[i].varsI[INDEX_WORLD_X]>UNIVERSE_MAX_X)||
				(plasmas[i].varsI[INDEX_WORLD_X]<UNIVERSE_MIN_X)||
				(plasmas[i].varsI[INDEX_WORLD_Y]>UNIVERSE_MAX_Y)||
				(plasmas[i].varsI[INDEX_WORLD_Y]<UNIVERSE_MIN_Y))
			{
				//报废掉该离子
				plasmas[i].state=PLASMA_STATE_OFF;
				plasmas[i].counter1=0;
				continue;
			}

			//检查离子是否击中雷
			for (int k=0;k<MAX_MINES;k++)
			{
				if (mine.mines[k].state==MINE_STATE_ALIVE &&
					plasmas[i].animState==PLASMA_ANIM_PLAYER)
				{
					if (ColliCheck(plasmas[i].varsI[INDEX_WORLD_X]-(plasmas[i].width>>1),
						plasmas[i].varsI[INDEX_WORLD_Y]-(plasmas[i].height>>1),
						plasmas[i].width,plasmas[i].height,mine.mines[k].varsI[INDEX_WORLD_X]-(mine.mines[k].width>>1),
						mine.mines[k].varsI[INDEX_WORLD_Y]-(mine.mines[k].height>>1),mine.mines[k].width,mine.mines[k].height))
					{
						//首先报废掉离子
						plasmas[i].state=PLASMA_STATE_OFF;
						plasmas[i].counter1=0;
						//增加雷的伤害
						mine.mines[k].varsI[INDEX_MINE_DAMAGE]+=(1+rand()%3);
						if (mine.mines[k].varsI[INDEX_MINE_DAMAGE]>MAX_MINE_DAMAGE)
						{
							mine.mines[k].state=MINE_STATE_DEAD;
							mine.StartMine();
							dh.SetScore(dh.GetScore()+250);
						}
						int width=30+rand()%40;
						burst.StartBurst(&ps,plasmas[i].varsI[INDEX_WORLD_X],plasmas[i].varsI[INDEX_WORLD_Y],
							width,(width>>1)+width*.25,((int)plasmas[i].xv>>1),(int)plasmas[i].yv>>1);
						
						break;
					}
				}
			}

			//检测炮弹离子是否击中战列舰
			for (int k=0;k<MAX_BATTLESHIPS;k++)
			{
				if (battleship.battleships[k].state==BATTLESHIP_STATE_ALIVE &&
					plasmas[i].animState==PLASMA_ANIM_PLAYER)
				{
					if (ColliCheck(plasmas[i].varsI[INDEX_WORLD_X]-(plasmas[i].width>>1),
						plasmas[i].varsI[INDEX_WORLD_Y]-(plasmas[i].height>>1),plasmas[i].width,plasmas[i].height,
						battleship.battleships[k].varsI[INDEX_WORLD_X]-(battleship.battleships[k].width>>1),
						battleship.battleships[k].varsI[INDEX_WORLD_Y]-(battleship.battleships[k].height>>1),
						battleship.battleships[k].width,battleship.battleships[k].height))
					{
						//首先报废掉离子
						plasmas[i].state=PLASMA_STATE_OFF;
						plasmas[i].counter1=0;
						//增加战列舰的伤害
						battleship.battleships[k].varsI[INDEX_BATTLESHIP_DAMAGE]+=(1+rand()%3);
						if (battleship.battleships[k].varsI[INDEX_BATTLESHIP_DAMAGE]>MAX_BATTLESHIP_DAMAGE)
						{
							battleship.battleships[k].state=BATTLESHIP_STATE_DEAD;
							battleship.StartBattleShip();
							dh.SetScore(dh.GetScore()+1000);
						}
						
						burst.StartBurst(&ps,plasmas[i].varsI[INDEX_WORLD_X],plasmas[i].varsI[INDEX_WORLD_Y],
							40+rand()%20,30+rand()%16,(int)plasmas[i].xv>>1,(int)plasmas[i].yv>>1);

						break;
					}
				}
			}

			//
			//检测炮弹离子是否击中敌空间站
			for (int k=0;k<MAX_STATIONS;k++)
			{
				if (station.stations[k].state==STATION_STATE_ALIVE &&
					plasmas[i].animState==PLASMA_ANIM_PLAYER)
				{
					if (ColliCheck(plasmas[i].varsI[INDEX_WORLD_X]-(plasmas[i].width>>1),
						plasmas[i].varsI[INDEX_WORLD_Y]-(plasmas[i].height>>1),plasmas[i].width,plasmas[i].height,
						station.stations[k].varsI[INDEX_WORLD_X]-(station.stations[k].width>>1),
						station.stations[k].varsI[INDEX_WORLD_Y]-(station.stations[k].height>>1),station.stations[k].width,
						station.stations[k].height))
					{
						//首先报废掉离子
						plasmas[i].state=PLASMA_STATE_OFF;
						plasmas[i].counter1=0;
						//增加敌空间站的伤害
						station.stations[k].varsI[INDEX_STATION_DAMAGE]+=(1+rand()%3);
						if (station.stations[k].varsI[INDEX_STATION_DAMAGE]>MAX_STATION_DAMAGE)
						{
							station.stations[k].state=STATION_STATE_DEAD;
							station.StartStation();
							dh.SetScore(dh.GetScore()+10000);
						
							if(++station.GetNumDamageStation()>=NUM_ACTIVE_STATIONS)
							{
								dh.SetState(PLAYER_STATE_WIN);
							}
						}
						burst.StartBurst(&ps,plasmas[i].varsI[INDEX_WORLD_X],plasmas[i].varsI[INDEX_WORLD_Y],
							40+rand()%20,30+rand()%16,(int)station.stations[k].xv>>1,(int)station.stations[i].yv>>1);

						break;
					}
				}
			}
			
			//玩家是否被敌舰击中
			if (dh.GetState()==PLAYER_STATE_ALIVE &&
				plasmas[i].animState==PLASMA_ANIM_ENEMY)
			{
				if (ColliCheck(plasmas[i].varsI[INDEX_WORLD_X]-(plasmas[i].width>>1),
					plasmas[i].varsI[INDEX_WORLD_Y]-(plasmas[i].height>>1),
					plasmas[i].width,plasmas[i].height,dh.GetX()-(dh.meteor.width>>1),
					dh.GetY()-(dh.meteor.height>>1),dh.meteor.width,dh.meteor.height))
				{
					burst.StartBurst(&ps,plasmas[i].varsI[INDEX_WORLD_X],plasmas[i].varsI[INDEX_WORLD_Y],
						40+rand()%20,30+rand()%16,(int)dh.GetX()>>1,(int)dh.GetY()>>1);
					
					dh.SetDamage(dh.GetDamage()+2);
					dh.SetShield(3);
					//首先报废掉离子
					plasmas[i].state=PLASMA_STATE_OFF;
					plasmas[i].counter1=0;
					continue;
					
				}
			}

			//检查是否击中石头
			for (int k=0;k<MAX_ROCKS;k++)
			{
				if (rock.rocks[k].state==ROCK_STATE_ON )
				{
					if (ColliCheck(plasmas[i].varsI[INDEX_WORLD_X]-(plasmas[i].width>>1),
						plasmas[i].varsI[INDEX_WORLD_Y]-(plasmas[i].height>>1),
						plasmas[i].width,plasmas[i].height,rock.rocks[k].varsI[INDEX_WORLD_X]-(rock.rocks[k].width>>1),
						rock.rocks[k].varsI[INDEX_WORLD_Y]-(rock.rocks[k].height>>1),rock.rocks[k].width,rock.rocks[k].height))
					{
						//首先报废掉离子
						plasmas[i].state=PLASMA_STATE_OFF;
						plasmas[i].counter1=0;
						
						if (RAND_RANGE(0,10)==5)
						{
							//Start a powerup
							powerup.StartPowerup(RAND_RANGE(POWERUP_TYPE_AMMO,POWERUP_TYPE_SHLD),
								rock.rocks[k].varsI[INDEX_WORLD_X],rock.rocks[k].varsI[INDEX_WORLD_Y]);
						}
						if (RAND_RANGE(0,500)==499)
						{
							powerup.StartPowerup(POWERUP_TYPE_SOURCE,
								rock.rocks[k].varsI[INDEX_WORLD_X],rock.rocks[k].varsI[INDEX_WORLD_Y]);
						}
						switch (rock.rocks[k].varsI[0])
						{
						case ROCK_LARGE:
							burst.StartBurst(&ps,plasmas[i].varsI[INDEX_WORLD_X],plasmas[i].varsI[INDEX_WORLD_Y],
								68+rand()%12,54+rand()%10,(int)rock.rocks[i].xv>>1,(int)rock.rocks[i].yv>>1);
							break;
						case ROCK_MEDIUM:
							burst.StartBurst(&ps,plasmas[i].varsI[INDEX_WORLD_X],plasmas[i].varsI[INDEX_WORLD_Y],
								54+rand()%10,44+rand()%8,(int)rock.rocks[i].xv>>1,(int)rock.rocks[i].yv>>1);
							break;
						case ROCK_SMALL:
							burst.StartBurst(&ps,plasmas[i].varsI[INDEX_WORLD_X],plasmas[i].varsI[INDEX_WORLD_Y],
								34-4+rand()%8,30-3+rand()%6,(int)rock.rocks[i].xv>>1,(int)rock.rocks[i].yv>>1);
							break;
						}
						dh.SetScore(dh.GetScore()+rock.rocks[k].varsI[2]);
						//减少岩石厚度
						rock.rocks[k].varsI[2]=rock.rocks[k].varsI[2]-50;

						if (rock.rocks[k].varsI[2]>0 && rock.rocks[k].varsI[2]<50)
						{
							switch (rock.rocks[k].varsI[0])
							{
							case ROCK_LARGE:
								//分裂成两个中型的
								rock.StartRock(rock.rocks[k].varsI[INDEX_WORLD_X]+rand()%16,
									rock.rocks[k].varsI[INDEX_WORLD_Y]+rand()%16,ROCK_MEDIUM,
									rock.rocks[k].xv-2+rand()%4,rock.rocks[k].yv-2+rand()%4);
								rock.StartRock(rock.rocks[k].varsI[INDEX_WORLD_X]+rand()%16,
									rock.rocks[k].varsI[INDEX_WORLD_Y]+rand()%16,ROCK_MEDIUM,
									rock.rocks[k].xv-2+rand()%4,rock.rocks[k].yv-2+rand()%4);

								if ((rand()%3)==1)
								{
									rock.StartRock(rock.rocks[k].varsI[INDEX_WORLD_X]+rand()%16,
										rock.rocks[k].varsI[INDEX_WORLD_Y]+rand()%16,ROCK_SMALL,
										rock.rocks[k].xv-2+rand()%4,rock.rocks[k].yv-2+rand()%4);
								}
								//报废掉以前的岩石
								rock.rocks[k].state=ROCK_STATE_OFF;
								break;
							case ROCK_MEDIUM:
								//分裂成1-3个小的
								for (int numRocks=1+rand()%3;numRocks>0;numRocks--)
								{
									rock.StartRock(rock.rocks[k].varsI[INDEX_WORLD_X]+rand()%8,
										rock.rocks[k].varsI[INDEX_WORLD_Y]+rand()%8,ROCK_SMALL,
										rock.rocks[k].xv-2+rand()%4,rock.rocks[k].yv-2+rand()%4);
								}
								//报废掉以前的岩石
								rock.rocks[k].state=ROCK_STATE_OFF;
								break;
							case ROCK_SMALL:
								rock.rocks[k].state=ROCK_STATE_OFF;
								break;
							}	
						}
						else if (rock.rocks[k].varsI[2]<=0)
						{
							rock.rocks[k].state=ROCK_STATE_OFF;
						}
						break;
					}
				}
			}

		}
	}
}

void CPlasma::DrawPlasma()
{
	for (int i=0;i<MAX_PLASMA;i++)
	{
		if (plasmas[i].state==PLASMA_STATE_ON)
		{
			plasmas[i].x=plasmas[i].varsI[INDEX_WORLD_X]-dh.GetX()-(plasmas[i].width>>1)+(SCREEN_WIDTH>>1);
			plasmas[i].y=plasmas[i].varsI[INDEX_WORLD_Y]-dh.GetY()-(plasmas[i].height>>1)+(SCREEN_HEIGHT>>1);

			plasmas[i].RenderAnimation(&render,render.lpddsback);
			plasmas[i].DoAnimation();

		}
	}
}

//向一个给定的位置，已给定的速度开火
void CPlasma::FirePlasma(int x, int y, int xv, int yv,int dir,int source)
{	
	static int turretTrack[16][2]  = {{25,-30},{ 55,-20}, {70,0}, {70,20}, { 80,35}, {80,60},{75,85},
	{48,90},{25,100},{5,90},{-12,80},{-30,60},{-40,32},{-30,18},{-5,0},{0,-20}};

	for(int i=0;i<MAX_PLASMA;i++)
	{
		if (plasmas[i].state==PLASMA_STATE_OFF)
		{
			//设置子弹的位置
			plasmas[i].varsI[INDEX_WORLD_X]=x-(plasmas[0].width>>1)+turretTrack[dir][0];
			plasmas[i].varsI[INDEX_WORLD_Y]=y-(plasmas[0].height>>1)+turretTrack[dir][1];
			
			plasmas[i].xv=xv;
			plasmas[i].yv=yv;
			
			plasmas[i].counter1=0;
			plasmas[i].state=PLASMA_STATE_ON;

			plasmas[i].animState=source;
			int anim=0;
			if (source==PLASMA_ANIM_PLAYER)
			{
				anim=dir;
				plasmas[i].currFrame=dir;
			}
			else
			{
				anim=16;
				plasmas[i].currFrame=16;
			}
			
			plasmas[i].SetAnimation(anim);

			for (int i=0;i<MAX_EXPL_SOUND;i++)
			{
				if (dsound.GetStatus(fireId[i])==0)
				{
					dsound.play(fireId[i],0);
					break;
				}
			}
			return ;
		}
	}
}