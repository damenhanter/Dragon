#pragma once
#include "dragon.h"
#include "../DragonEngine/engine.h"
#pragma comment(lib,"../lib/DragonEngine.lib")


class CMine
{
public:
	CMine(void);
	~CMine(void);

	inline void SetTrackingRate(float rate)
	{
		this->trackingRate=rate;
	}

	inline float GetTrackingRate()
	{
		return trackingRate;
	}

	inline void SetPowerupId(int id)
	{
		powerupId=id;
	}
	inline int& GetPowerupId()
	{
		return powerupId;
	}
	void InitMine();
	void ResetMine();
	void StartMine(int override=0,int x=0,int y=0,int AIState=MINE_STATE_AI_ACTIVATED);
	void MoveMine();
	void DrawMine();
private:
	int powerupId;
	float trackingRate;   //À××·×ÙÆµÂÊ
	CBoundSphere m_BoundSphere;
public:
	CAnimation mines[MAX_MINES];
	
};
