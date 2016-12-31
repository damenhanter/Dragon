#pragma once
#include "dragon.h"
#include "../DragonEngine/engine.h"
#pragma comment(lib,"../lib/DragonEngine.lib")

typedef struct  
{
	int x;
	int y;               //λ��
	USHORT color;         //������ɫ
	int plane;
}STAR,LPSTAR;
class CStar
{
public:
	CStar(void);
	~CStar(void);
	
	void InitStar();
	void MoveStar();
	void DrawStar();
public:
	STAR stars[MAX_STARS];
};
