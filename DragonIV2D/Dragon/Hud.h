#pragma once
#include "dragon.h"
#include "../DragonEngine/engine.h"
#pragma comment(lib,"../lib/DragonEngine.lib")

class CHud
{
public:
	CHud(void);
	~CHud(void);

	void InitHud();
public:
	CAnimation huds;
};
