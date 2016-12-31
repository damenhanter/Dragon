#pragma once
#include "dragon.h"
#include "../DragonEngine/engine.h"
#pragma comment(lib,"../lib/DragonEngine.lib")

typedef struct  
{
	int x;
	int y;               //Î»ÖÃ
	USHORT color;         //ÐÇÐÇÑÕÉ«
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
