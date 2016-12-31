#pragma once
#include "dragon.h"
#include "../DragonEngine/engine.h"
#pragma comment(lib,"../lib/DragonEngine.lib")



class CStation
{
public:
	CStation(void);
	~CStation(void);
	
	void SetNumDamageStation(int num);
	int& GetNumDamageStation();
	void SetId(int id);
	int& GetId();
	void InitStation();
	void ResetStation();
	void StartStation(int override=0,int x=0,int y=0);
	void MoveStation();
	void DrawStation();
private:
	int id;
	int NumDamageStation;
public:
	CAnimation stations[MAX_STATIONS];
	CAnimation SmallStations;
};
