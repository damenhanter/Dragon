#include "Rock.h"
#include "Burst.h"
#include "dragon.h"
#include "DH.h"
extern CRenderSystem render;

int RockSize[3]={96,100,40};

extern CDH dh;
extern CBurst burst;
extern CBoundSphere bs;
extern CParticleSystem ps;

CRock::CRock(void)
{
}

CRock::~CRock(void)
{

}

//��ʼ���ͼ������е���ʯ
void CRock::InitRock()
{
	int frame;
	CBitmapFile bitmap16bit;
	//����������ʯ
	LargeRock.CreateAnimation(&render,0,0,300,300,16,ANIM_ATTR_MULTI_FRAME|ANIM_ATTR_VISIBLE,
		DDSCAPS_VIDEOMEMORY,0,SCREEN_BPP);
	//���ض���֡
	char buffer[80];
	for (frame=0;frame<16;frame++)
	{
		PadName("../res/picture/LargeRock" ,"bmp",buffer,frame);
		bitmap16bit.LoadBitmapFromFile(buffer);
		LargeRock.LoadAnimFrame16(&bitmap16bit,frame,0,0,BITMAP_EXTRACT_MODE_ABS);
		bitmap16bit.UnLoadBitmapFile();
	}
	LargeRock.SetAnimSpeed(1+rand()%5);
	LargeRock.SetVelocity( -4+rand()%8, 4+rand()%4);
	LargeRock.SetPos(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT);
	LargeRock.varsI[0]=ROCK_LARGE;
	LargeRock.varsI[1]=RockSize[LargeRock.varsI[0]];
	LargeRock.animState=ROCK_STATE_OFF;

	//������ʯ


	MediumRock.CreateAnimation(&render,0,0,100,100,16,ANIM_ATTR_MULTI_FRAME|ANIM_ATTR_VISIBLE,
		DDSCAPS_VIDEOMEMORY,0,SCREEN_BPP);
	//���ض���֡
	memset(buffer,0,80);
	for (frame=0;frame<16;frame++)
	{
		PadName("../res/picture/MediumRock" ,"bmp",buffer,frame);
		bitmap16bit.LoadBitmapFromFile(buffer);
		MediumRock.LoadAnimFrame16(&bitmap16bit,frame,0,0,BITMAP_EXTRACT_MODE_ABS);
		bitmap16bit.UnLoadBitmapFile();
	}
	MediumRock.SetAnimSpeed(1+rand()%5);
	MediumRock.SetVelocity( -4+rand()%8, 4+rand()%4);
	MediumRock.SetPos(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT);
	MediumRock.varsI[0]=ROCK_MEDIUM;
	MediumRock.varsI[1]=RockSize[MediumRock.varsI[0]];
	MediumRock.animState=ROCK_STATE_OFF;
	
	//С����ʯ


	SmallRock.CreateAnimation(&render,0,0,40,40,16,ANIM_ATTR_MULTI_FRAME|ANIM_ATTR_VISIBLE,
		DDSCAPS_VIDEOMEMORY,0,SCREEN_BPP);
	//���ض���֡
	for (frame=0;frame<16;frame++)
	{
		PadName("../res/picture/LittleRock" ,"bmp",buffer,frame);
		bitmap16bit.LoadBitmapFromFile(buffer);
		SmallRock.LoadAnimFrame16(&bitmap16bit,frame,0,0,BITMAP_EXTRACT_MODE_ABS);
		bitmap16bit.UnLoadBitmapFile();
	}

	SmallRock.SetAnimSpeed(1+rand()%5);
	SmallRock.SetVelocity( -4+rand()%8, 4+rand()%4);
	SmallRock.SetPos(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT);
	SmallRock.varsI[0]=ROCK_SMALL;
	SmallRock.varsI[1]=RockSize[SmallRock.varsI[0]];
	SmallRock.animState=ROCK_STATE_OFF;

	//����ʯ������
	for (int i=0;i<MAX_ROCKS;i++)
	{
		rocks[i].CloneAnim(&LargeRock);
		rocks[i].SetAnimSpeed(1+rand()%5);
		rocks[i].SetVelocity(-4+rand()%8,-4+rand()%8);
		rocks[i].SetPos(0,0);
		rocks[i].varsI[INDEX_WORLD_X]=RAND_RANGE(UNIVERSE_MIN_X,UNIVERSE_MAX_X);
		rocks[i].varsI[INDEX_WORLD_Y]=RAND_RANGE(UNIVERSE_MIN_Y,UNIVERSE_MAX_Y);
		//������ʯ�ߴ�
		rocks[i].varsI[0]=rand()%3;
		rocks[i].varsI[1]=RockSize[rocks[i].varsI[0]];
		//���ݳߴ磬����ͼ��
		switch (rocks[i].varsI[0])
		{
		case ROCK_LARGE:
			{
				memcpy(rocks[i].images,LargeRock.images,
					sizeof(LPDIRECTDRAWSURFACE7)*ANIM_MAX_FRAMES);
				rocks[i].width=LargeRock.width;
				rocks[i].height=LargeRock.height;
			}
			break;
		case ROCK_MEDIUM:
			{
				memcpy(rocks[i].images,MediumRock.images,
					sizeof(LPDIRECTDRAWSURFACE7)*ANIM_MAX_FRAMES);
				rocks[i].width=MediumRock.width;
				rocks[i].height=MediumRock.height;
			}
			break;
		case ROCK_SMALL:
			{
				memcpy(rocks[i].images,SmallRock.images,
					sizeof(LPDIRECTDRAWSURFACE7)*ANIM_MAX_FRAMES);
				rocks[i].width=SmallRock.width;
				rocks[i].height=SmallRock.height;
			}
			break;
		default:
			break;
		}
	}
}

void CRock::ResetRock()
{
	for (int i=0;i<MAX_ROCKS;i++)
	{
		rocks[i].animState=ROCK_STATE_OFF;
	}
}

void CRock::StartRock(int x,int y,int size,int xv,int yv)
{
	for (int i=0;i<MAX_ROCKS; i++)
	{
		if (rocks[i].animState==ROCK_STATE_OFF)
		{
			rocks[i].SetAnimSpeed(1+rand()%5);
			rocks[i].SetVelocity(xv,yv);
			rocks[i].SetPos(0,0);
			rocks[i].varsI[INDEX_WORLD_X]=x;
			rocks[i].varsI[INDEX_WORLD_Y]=y;
			//������ʯ�ߴ�
			rocks[i].varsI[0]=size;
			rocks[i].varsI[1]=RockSize[rocks[i].varsI[0]];
			//���ݳߴ磬����ͼ��
			switch (rocks[i].varsI[0])
			{
			case ROCK_LARGE:
				{
					//������ʯ���
					rocks[i].varsI[2]=100+rand()%100;
					memcpy(rocks[i].images,LargeRock.images,
						sizeof(LPDIRECTDRAWSURFACE7)*ANIM_MAX_FRAMES);
					rocks[i].width=LargeRock.width;
					rocks[i].height=LargeRock.height;
				}
				break;
			case ROCK_MEDIUM:
				{
					//������ʯ���
					rocks[i].varsI[2]=40+rand()%30;
					memcpy(rocks[i].images,MediumRock.images,
						sizeof(LPDIRECTDRAWSURFACE7)*ANIM_MAX_FRAMES);
					rocks[i].width=MediumRock.width;
					rocks[i].height=MediumRock.height;
				}
				break;
			case ROCK_SMALL:
				{
					//������ʯ���
					rocks[i].varsI[2]=10;
					memcpy(rocks[i].images,SmallRock.images,
						sizeof(LPDIRECTDRAWSURFACE7)*ANIM_MAX_FRAMES);
					rocks[i].width=SmallRock.width;
					rocks[i].height=SmallRock.height;
				}
				break;
			default:
				break;
			}
		}
		rocks[i].animState=ROCK_STATE_ON;
		return ;
	}
}
void CRock::SeekRock()
{
	for (int i=0;i<MAX_ROCKS*0.75; i++)
	{
		if (rocks[i].animState==ROCK_STATE_OFF)
		{
			rocks[i].SetAnimSpeed(1+rand()%5);
			rocks[i].SetVelocity( -6+rand()%12, -6+rand()%12);
			rocks[i].SetPos(0,0);
			rocks[i].varsI[INDEX_WORLD_X]=RAND_RANGE(UNIVERSE_MIN_X, UNIVERSE_MAX_X);
			rocks[i].varsI[INDEX_WORLD_Y]=RAND_RANGE(UNIVERSE_MIN_Y, UNIVERSE_MAX_Y);
			//������ʯ�ߴ�
			rocks[i].varsI[0]=rand()%3;
			rocks[i].varsI[1]=RockSize[rocks[i].varsI[0]];
			//���ݳߴ磬����ͼ��
			switch (rocks[i].varsI[0])
			{
			case ROCK_LARGE:
				{
					//������ʯ���
					rocks[i].varsI[2]=100+rand()%100;
					memcpy(rocks[i].images,LargeRock.images,
						sizeof(LPDIRECTDRAWSURFACE7)*ANIM_MAX_FRAMES);
					rocks[i].width=LargeRock.width;
					rocks[i].height=LargeRock.height;
				}
				break;
			case ROCK_MEDIUM:
				{
					//������ʯ���
					rocks[i].varsI[2]=40+rand()%30;
					memcpy(rocks[i].images,MediumRock.images,
						sizeof(LPDIRECTDRAWSURFACE7)*ANIM_MAX_FRAMES);
					rocks[i].width=MediumRock.width;
					rocks[i].height=MediumRock.height;
				}
				break;
			case ROCK_SMALL:
				{
					//������ʯ���
					rocks[i].varsI[2]=10;
					memcpy(rocks[i].images,SmallRock.images,
						sizeof(LPDIRECTDRAWSURFACE7)*ANIM_MAX_FRAMES);
					rocks[i].width=SmallRock.width;
					rocks[i].height=SmallRock.height;
				}
				break;
			default:
				break;
			}
		}
		rocks[i].animState=ROCK_STATE_ON;
	}
}
void CRock::MoveRock()
{
	for (int i=0;i<MAX_ROCKS;i++)
	{
		if (rocks[i].state==ROCK_STATE_ON)
		{
			rocks[i].varsI[INDEX_WORLD_X]+=rocks[i].xv;
			rocks[i].varsI[INDEX_WORLD_Y]+=rocks[i].yv;

			if (rocks[i].varsI[INDEX_WORLD_X]>=UNIVERSE_MAX_X)
			{
				rocks[i].varsI[INDEX_WORLD_X]=UNIVERSE_MIN_X;
			}
			else if (rocks[i].varsI[INDEX_WORLD_X]<=UNIVERSE_MIN_X)
			{
				rocks[i].varsI[INDEX_WORLD_X]=UNIVERSE_MAX_X;
			}
			else if (rocks[i].varsI[INDEX_WORLD_Y]>=UNIVERSE_MAX_Y)
			{
				rocks[i].varsI[INDEX_WORLD_Y]=UNIVERSE_MIN_Y;
			}
			else if (rocks[i].varsI[INDEX_WORLD_Y]<=UNIVERSE_MIN_Y)
			{
				rocks[i].varsI[INDEX_WORLD_Y]=UNIVERSE_MAX_Y;
			}

			//�����ʯ�����ײ���
		
			if (dh.GetState()==PLAYER_STATE_ALIVE &&
				ColliCheck(rocks[i].varsI[INDEX_WORLD_X]-(rocks[i].width>>1),rocks[i].varsI[INDEX_WORLD_Y]-(rocks[i].height>>1),
				rocks[i].width,rocks[i].height,
				dh.GetX()-(dh.meteor.width>>1),dh.GetY()-(dh.meteor.height>>1),
				dh.meteor.width,dh.meteor.height))
			{
				//����ײ�������ִ�С����ʯ
				switch (rocks[i].varsI[0])
				{
				case ROCK_LARGE:
					//��ʼ��ը
					burst.StartBurst(&ps,rocks[i].varsI[INDEX_WORLD_X],rocks[i].varsI[INDEX_WORLD_Y],
						 68+rand()%12,54+rand()%10,(int)rocks[i].xv>>1,(int)rocks[i].yv>>1);
					//ˢ������˺�
					dh.SetDamage(dh.GetDamage()+35);
					//ˢ����ײ����ٶ�
					dh.SetXV(dh.GetXV()+rocks[i].xv);
					dh.SetYV(dh.GetYV()+rocks[i].yv);
					
					//����һ����ҵ��ٶȷ���
					dh.meteor.varsI[INDEX_METEOR_DIR]+=RAND_RANGE(-4,4);
					if (dh.meteor.varsI[INDEX_METEOR_DIR]>15)
					{
						dh.meteor.varsI[INDEX_METEOR_DIR]-=16;
					}
					else if (dh.meteor.varsI[INDEX_METEOR_DIR]<0)
					{
						dh.meteor.varsI[INDEX_METEOR_DIR]+=16;
					}
					break;
				case ROCK_MEDIUM:
					{
						//��ʼ��ը
						burst.StartBurst(&ps,rocks[i].varsI[INDEX_WORLD_X],rocks[i].varsI[INDEX_WORLD_Y],
							52+rand()%10,44+rand()%8,(int)rocks[i].xv>>1,(int)rocks[i].yv>>1);
						//ˢ������˺�
						dh.SetDamage(dh.GetDamage()+15);
						//ˢ����ײ����ٶ�
						dh.SetXV(dh.GetXV()+((int)rocks[i].xv>>1));
						dh.SetYV(dh.GetYV()+((int)rocks[i].yv>>1));

						//����һ����ҵ��ٶȷ���
						dh.meteor.varsI[INDEX_METEOR_DIR]+=RAND_RANGE(-4,4);
						if (dh.meteor.varsI[INDEX_METEOR_DIR]>15)
						{
							dh.meteor.varsI[INDEX_METEOR_DIR]-=16;
						}
						else if (dh.meteor.varsI[INDEX_METEOR_DIR]<0)
						{
							dh.meteor.varsI[INDEX_METEOR_DIR]+=16;
						}
					}
					break;
				case ROCK_SMALL:
					{
						//��ʼ��ը
						burst.StartBurst(&ps,rocks[i].varsI[INDEX_WORLD_X],rocks[i].varsI[INDEX_WORLD_Y],
							30+rand()%8,27+rand()%6,(int)rocks[i].xv>>1,(int)rocks[i].yv>>1);
						//ˢ������˺�
						dh.SetDamage(dh.GetDamage()+5);
						//ˢ����ײ����ٶ�
						dh.SetXV(dh.GetXV()+(((int)rocks[i].xv/4)>>1));
						dh.SetYV(dh.GetYV()+(((int)rocks[i].yv/4)>>1));

						//����һ����ҵ��ٶȷ���
						dh.meteor.varsI[INDEX_METEOR_DIR]+=RAND_RANGE(-4,4);
						if (dh.meteor.varsI[INDEX_METEOR_DIR]>15)
						{
							dh.meteor.varsI[INDEX_METEOR_DIR]-=16;
						}
						else if (dh.meteor.varsI[INDEX_METEOR_DIR]<0)
						{
							dh.meteor.varsI[INDEX_METEOR_DIR]+=16;
						}
					}
					break;
				}
				dh.SetScore(dh.GetScore()+rocks[i].varsI[2]);
				dh.SetShield(3);
				rocks[i].state=ROCK_STATE_OFF;
			}
		}
	}
	//�����Ƿ�Ӧ��������ʯͷ������ϵͳ
	if ((rand()%100)==50)
	{
		for (int i=0; i<MAX_ROCKS;i++)
		{
			if (rocks[i].state==ROCK_STATE_OFF)
			{
				rocks[i].SetAnimSpeed(1+rand()%5);
				rocks[i].SetVelocity(-6+rand()%12,-6+rand()%12);
				rocks[i].SetPos(0,0);
				rocks[i].varsI[INDEX_WORLD_X]=RAND_RANGE(UNIVERSE_MIN_X,UNIVERSE_MAX_X);
				rocks[i].varsI[INDEX_WORLD_Y]=RAND_RANGE(UNIVERSE_MIN_Y,UNIVERSE_MAX_Y);
				//������ʯ�ߴ�
				rocks[i].varsI[0]=rand()%3;
				rocks[i].varsI[1]=RockSize[rocks[i].varsI[0]];
				//���ݳߴ磬����ͼ��
				switch (rocks[i].varsI[0])
				{
				case ROCK_LARGE:
					{
						//������ʯ���
						rocks[i].varsI[2]=100+rand()%100;
						memcpy(rocks[i].images,LargeRock.images,
							sizeof(LPDIRECTDRAWSURFACE7)*ANIM_MAX_FRAMES);
						rocks[i].width=LargeRock.width;
						rocks[i].height=LargeRock.height;
					}
					break;
				case ROCK_MEDIUM:
					{
						//������ʯ���
						rocks[i].varsI[2]=40+rand()%30;
						memcpy(rocks[i].images,MediumRock.images,
							sizeof(LPDIRECTDRAWSURFACE7)*ANIM_MAX_FRAMES);
						rocks[i].width=MediumRock.width;
						rocks[i].height=MediumRock.height;
					}
					break;
				case ROCK_SMALL:
					{
						//������ʯ���
						rocks[i].varsI[2]=10;
						memcpy(rocks[i].images,SmallRock.images,
							sizeof(LPDIRECTDRAWSURFACE7)*ANIM_MAX_FRAMES);
						rocks[i].width=SmallRock.width;
						rocks[i].height=SmallRock.height;
					}
					break;
				default:
					break;
				}
				rocks[i].state=ROCK_STATE_ON;
				return ;
			}
		}
	}
}

void CRock::DrawRock()
{
	for(int i=0; i<MAX_ROCKS;i++)
	{
		if (rocks[i].state==ROCK_STATE_ON)
		{
			//ת�䵽��Ļ����
			rocks[i].x=rocks[i].varsI[INDEX_WORLD_X]-dh.GetX()-(rocks[i].width>>1)+(SCREEN_WIDTH/2);
			rocks[i].y=rocks[i].varsI[INDEX_WORLD_Y]-dh.GetY()-(rocks[i].height>>1)+(SCREEN_HEIGHT/2);
			rocks[i].RenderAnimation(&render,render.lpddsback);
			rocks[i].DoAnimation();
		}
	}
}


