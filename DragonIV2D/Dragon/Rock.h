#pragma once
#include "dragon.h"
#include "../DragonEngine/engine.h"
#pragma comment(lib,"../lib/DragonEngine.lib")

class CRock
{
public:
	CRock(void);
	~CRock(void);

	void InitRock();
	void ResetRock();
	void SeekRock();
	void StartRock(int x,int y,int size,int xv,int yv);
	void MoveRock();
	void DrawRock();

private:
	CAnimation LargeRock;
	CAnimation SmallRock;
	CAnimation MediumRock;
public:
	CAnimation rocks[MAX_ROCKS];
};
