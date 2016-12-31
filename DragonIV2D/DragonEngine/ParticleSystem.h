#pragma once
#include "datatype.h"
#include "RenderSystem.h"

typedef struct PARTICLE_TYP
{
	int state;          //particle's state
	int type;           //particle's effect type
	int x,y;            //particle's world position
	int xv,yv;          //particle's velocity
	int currColor;      //current rendering color
	int startColor;     //start rendering color
	int endColor;       //end rendering color
	int counter;        //type transition counter
	int maxCount;       //most counter
}PARTICLE,*LPPARTICLE;

class CParticleSystem
{
public:
	CParticleSystem();
	~CParticleSystem(void);
public:
	void StartParticle(int type,int color,int count,int x,
		int y,int xv,int yv);
	void ExplosionParticle(int type,int color,int count,
		int x,int y,float xv,float yv,int numParticles);
	void ResetParticle();
	void DrawParticle(CRenderSystem* render);
	void MoveParticle();
public:
	PARTICLE particles[MAX_PARTICLES];
};
