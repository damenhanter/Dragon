#include "Burst.h"
#include "DH.h"

extern CRenderSystem render;
extern CDSound dsound;
extern CDH dh;



CBurst::CBurst(void)
{
	for (int i=0;i<MAX_EXPL_SOUND;i++)
	{
		ExplId[i]=-1;
	}
}

CBurst::~CBurst(void)
{

}

void CBurst::SetExpId(int index,int id)
{
	ExplId[index]=id;
}

int& CBurst::GetExpId(int index)
{
	return ExplId[index];
}
void CBurst::InitBurst()
{
	burst[0].CreateAnimation(&render,0,0,100,100,15,
		ANIM_ATTR_VISIBLE | ANIM_ATTR_MULTI_FRAME,DDSCAPS_VIDEOMEMORY,0,SCREEN_BPP);
	//加载动画帧
	CBitmapFile bitmap16bit;
	char buf[80];
	for (int frame=0; frame<15; frame++)
	{
		PadName("../res/picture/explode","bmp",buf,frame);
		bitmap16bit.LoadBitmapFromFile(buf);
		burst[0].LoadAnimFrame16(&bitmap16bit,frame,0,0,BITMAP_EXTRACT_MODE_ABS);
		bitmap16bit.UnLoadBitmapFile();
	}
	//设置动画速度
	burst[0].SetAnimSpeed(1);
	//设置爆炸的尺寸
	burst[0].varsI[0]=burst[0].width;
	burst[0].varsI[1]=burst[0].height;
	//设置状态-静止
	burst[0].state=BURST_STATE_OFF;

	for (int i=1; i<MAX_BURSTS; i++)
	{
		burst[i].CloneAnim(&burst[0]);
	}
}

void CBurst::ResetBurst()
{
	for (int i=0;i<MAX_BURSTS;i++)
	{
		burst[i].state=BURST_STATE_OFF;
	}
}


void CBurst::MoveBurst()
{
	for (int i=0; i<MAX_BURSTS; i++)
	{
		if (burst[i].state==BURST_STATE_ON)
		{
			//移动炸弹
			burst[i].varsI[INDEX_WORLD_X]+=(int)burst[i].xv;
			burst[i].varsI[INDEX_WORLD_Y]+=(int)burst[i].yv;

			if (burst[i].currFrame>=burst[i].numFrames-1)
			{
				burst[i].state=BURST_STATE_OFF;
			}
		}
	}
}
void CBurst::DrawBurst()
{
	for (int i=0; i<MAX_BURSTS; i++)
	{
		if (burst[i].state==BURST_STATE_ON)
		{
			//转变炸弹位置到屏幕坐标系
			burst[i].x=burst[i].varsI[INDEX_WORLD_X]-(burst[i].width>>1)-dh.GetX()+(SCREEN_WIDTH>>1);
			burst[i].y=burst[i].varsI[INDEX_WORLD_Y]-(burst[i].height>>1)-dh.GetY()+(SCREEN_HEIGHT>>1);

			//是否需要缩放
 			//if (burst[i].varsI[0]!=burst[i].width||
 			//	burst[i].varsI[1]!=burst[i].height)
 			//{
 			//	//burst[i].RenderScaleAnim(&render,burst[i].varsI[0],burst[i].varsI[1],render.lpddsback);
 			//}
 			//else
 			//{
 			//	
 			//}
			burst[i].RenderAnimation(&render,render.lpddsback);
			burst[i].DoAnimation();
		}
	}
}

void CBurst::StartBurst(CParticleSystem* ps,int x,int y,int width,int height,float xv,float yv)
{
	for (int i=0; i<MAX_BURSTS; i++)
	{
		if (burst[i].state==BURST_STATE_OFF)
		{
			//设置动画频率
			burst[i].SetAnimSpeed(1);
			burst[i].currFrame=0;
			//设置爆炸速度
			burst[i].SetVelocity(xv,yv);
			//设置爆炸位置
			burst[i].SetPos(0,0);
			burst[i].varsI[INDEX_WORLD_X]=x;
			burst[i].varsI[INDEX_WORLD_Y]=y;
			//设置爆炸的尺寸
			burst[i].varsI[0]=width;
			burst[i].varsI[1]=height;
			//设置爆炸状态开
			burst[i].state=BURST_STATE_ON;

			ps->ExplosionParticle(PARTICLE_TYPE_FICKER,RAND_RANGE(PARTICLE_COLOR_RED,PARTICLE_COLOR_GREEN),
				20+rand()%40,burst[i].varsI[INDEX_WORLD_X],burst[i].varsI[INDEX_WORLD_Y],burst[i].xv,
				burst[i].yv,8+rand()%8);

			for (int soundIndex=0; soundIndex<=MAX_EXPL_SOUND;soundIndex++)
			{
				if (dsound.GetStatus(ExplId[soundIndex])==0)
				{
					dsound.play(ExplId[soundIndex],0);
					break;
				}
			}
			return ;
		}
	}
}

