#pragma once

#include "dragon.h"
#include "../DragonEngine/engine.h"
#pragma comment(lib,"../lib/DragonEngine.lib")


class CPowerup
{
public:
	CPowerup(void);
	~CPowerup(void);

	void SetAmmoId(int id);
	int& GetAmmoId();
	void SetShldId(int id);
	int& GetShldId();

	void InitPowerup();
	void ResetPowerup();
	void StartPowerup(int type,int x,int y);
	void MovePowerup();
	void DrawPowerup();
private:
	int ammoId;
	int shldId;
public:
	CAnimation powerups[MAX_POWERUPS];
};
