#pragma once
#include "dragon.h"
#include "../DragonEngine/engine.h"
#pragma comment(lib,"../lib/DragonEngine.lib")

class CBattleship
{
public:
	CBattleship(void);
	~CBattleship(void);

	void InitBattleShip();
	void ResetBattleShip();
	void StartBattleShip();
	void MoveBattleShip();
	void DrawBattleShip();

public:
	CAnimation battleships[MAX_BATTLESHIPS];
};
