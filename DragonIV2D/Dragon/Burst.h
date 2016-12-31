#pragma once
#include "dragon.h"
#include "../DragonEngine/engine.h"
#pragma comment(lib,"../lib/DragonEngine.lib")

class CBurst
{
public:
	CBurst(void);
	~CBurst(void);
	void SetExpId(int index,int id);
	int& GetExpId(int index);
	void InitBurst();
	void ResetBurst();
	void MoveBurst();
	void DrawBurst();
	void StartBurst(CParticleSystem* ps,int x,int y,int width,int height,float xv,float yv);
private:
	int ExplId[MAX_EXPL_SOUND];
public:
	CAnimation burst[8];
};
