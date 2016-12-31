#include "StdAfx.h"
#include "ParticleSystem.h"
#include "../Dragon/DH.h"

extern RECT g_rRectClip;
extern CDH dh;
extern float cosTable[16];
extern float sinTable[16];

CParticleSystem::CParticleSystem()
{
}

CParticleSystem::~CParticleSystem(void)
{
}


void CParticleSystem::ResetParticle()
{
	for (int i=0;i<MAX_PARTICLES;i++)
	{
		particles[i].x=0;
		particles[i].y=0;
		particles[i].xv=0;
		particles[i].yv=0;
		particles[i].counter=0;
		particles[i].maxCount=0;
		particles[i].startColor=0;
		particles[i].currColor=0;
		particles[i].endColor=0;
		particles[i].state=PARTICLE_STATE_DEAD;
		particles[i].type=PARTICLE_TYPE_FADE;

	}
}

void CParticleSystem::StartParticle(int type,int color,int count,int x,
									 int y,int xv,int yv)
{
	int index=-1;
	for (int i=0;i<MAX_PARTICLES;i++)
	{
		if (particles[i].state==PARTICLE_STATE_DEAD)
		{
			index=i;
			break;
		}
	}
	if (index==-1)
	{
		return;
	}
	
	particles[index].type=type;
	particles[index].state=PARTICLE_STATE_ALIVE;
	particles[index].counter=0;
	particles[index].maxCount=count;
	particles[index].x=x;particles[index].y=y;
	particles[index].xv=xv;particles[index].yv=yv;
	particles[index].counter=0;
	particles[index].maxCount=count;
	switch (color)
	{
	case PARTICLE_COLOR_RED:
		{
			particles[index].startColor=COLOR_RED_START;
			particles[index].endColor=COLOR_RED_END;
		}
		break;
	case PARTICLE_COLOR_BLUE:
		{
			particles[index].startColor=COLOR_BLUE_START;
			particles[index].endColor=COLOR_BLUE_END;
		}
		break;
	case PARTICLE_COLOR_GREEN:
		{
			particles[index].startColor=COLOR_GREEN_START;
			particles[index].endColor=COLOR_GREEN_END;
		}
		break;
	case PARTICLE_COLOR_WHITE:
		{
			particles[index].startColor=COLOR_WHITE_START;
			particles[index].endColor=COLOR_WHITE_END;
		}
		break;
	}
	if (type==PARTICLE_TYPE_FICKER)
	{
		particles[index].currColor=RAND_RANGE(particles[index].startColor,particles[index].endColor);
	}
	else
	{
		particles[index].currColor=particles[index].startColor;
	}
}

void CParticleSystem::ExplosionParticle(int type, int color, int count,
										int x, int y, float xv, float yv, int numParticles)
{
	while (--numParticles>0)
	{
		int angle=RAND_RANGE(0,16);
		float vel=float(2+RAND_RANGE(0,4));
		StartParticle(type,color,count,
			x+RAND_RANGE(-4,4),y+RAND_RANGE(-4,4),
			(int)(xv+cosTable[angle]*vel),(int)(yv+sinTable[angle]*16));
	}
}

void CParticleSystem::DrawParticle(CRenderSystem* render)
{
	for (int i=0;i<MAX_PARTICLES;i++)
	{
		if (particles[i].state==PARTICLE_STATE_ALIVE)
		{
			int x=particles[i].x-(int)dh.GetX()+SCREEN_WIDTH/2;
			int y=particles[i].y-(int)dh.GetY()+SCREEN_HEIGHT/2;
			if (x>=SCREEN_WIDTH-1||x<0||y>=SCREEN_HEIGHT-1||y<0)
			{
				continue;
			}
			render->DrawPixel(x,y,particles[i].currColor);
		}
	}
}

void CParticleSystem::MoveParticle()
{
	for (int index=0;index<MAX_PARTICLES;index++)
	{
		if (particles[index].state == PARTICLE_STATE_ALIVE)
		{
			particles[index].x+=particles[index].xv;
			particles[index].y+=particles[index].yv;
			if (particles[index].type==PARTICLE_TYPE_FICKER)
			{
				particles[index].currColor = RAND_RANGE(particles[index].startColor, particles[index].endColor);
				if (++particles[index].counter >= particles[index].maxCount)
				{
					particles[index].state = PARTICLE_STATE_DEAD;             
				} 
			} 
			else
			{
				if (++particles[index].counter >= particles[index].maxCount)
				{
					particles[index].counter = 0;
					if (++particles[index].currColor>particles[index].endColor)
					{
						particles[index].state = PARTICLE_STATE_DEAD;  
					}
				}
			} 
		} 
	} 
}