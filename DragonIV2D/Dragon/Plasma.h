#pragma once
#include "dragon.h"
#include "../DragonEngine/engine.h"
#pragma comment(lib,"../lib/DragonEngine.lib")

class CPlasma
{
public:
	CPlasma(void);
	~CPlasma(void);

	void SetFireId(int index,int id);
	int& GetFireId(int index);
	void InitPlasma();
	void ResetPlasma();
	void MovePlasma();
	void DrawPlasma();
	void FirePlasma(int x,int y,int xv,int yv,int dir,int source=PLASMA_ANIM_PLAYER);
private:
	int fireId[MAX_FIRE_SOUND];
public:
	CAnimation plasmas[MAX_PLASMA];
};
