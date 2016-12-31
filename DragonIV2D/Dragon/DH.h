#pragma once
#include "../DragonEngine/engine.h"
#pragma comment(lib,"../lib/DragonEngine.lib")

class CDH
{
public:
	CDH(void);
	~CDH(void);

	inline void SetX(float x)
	{
		this->x=x;
	}

	inline float& GetX()
	{
		return x;
	}

	inline void SetY(float y)
	{
		this->y=y;
	}

	inline float& GetY()
	{
		return y;
	}

	inline void SetXV(float xv)
	{
		this->xv=xv;
	}

	inline float& GetXV()
	{
		return xv;
	}

	inline void SetYV(float yv)
	{
		this->yv=yv;
	}

	float& GetYV()
	{
		return yv;
	}

	inline void SetDX(float dx)
	{
		this->dx=dx;
	}

	inline float& GetDX()
	{
		return dx;
	}

	inline void SetDY(float dy)
	{
		this->dy=dy;
	}

	inline float& GetDY()
	{
		return dy;
	}

	inline void SetVel(float vel)
	{
		this->Vel=vel;
	}

	inline float& GetVel()
	{
		return Vel;
	}

	inline void SetState(int state)
	{
		this->state=state;
	}

	inline int& GetState()
	{
		return state;
	}

	inline void SetDamage(int damage)
	{
		this->damage=damage;
	}

	inline int& GetDamage()
	{
		return damage;
	}

	inline void SetScore(int score)
	{
		this->score=score;
	}

	inline int& GetScore()
	{
		return score;
	}

	inline void SetShield(int shield)
	{
		this->shield=shield;
	}

	inline int& GetShield()
	{
		return shield;
	}

	inline void SetAmmo(int ammo)
	{
		this->ammo=ammo;
	}

	inline int& CDH::GetAmmo()
	{
		return ammo;
	}

	inline void SetNumShips(int num)
	{
		Numships=num;
	}

	inline int& GetNumShips()
	{
		return Numships;
	}
	
	inline void SetCounter(int cnt)
	{
		counter=cnt;
	}
	inline int& GetCounter()
	{
		return counter;
	}
	void InitDH();
	void ResetPos();
private:
	float x;
	float y;
	float xv;
	float yv;
	float dx;
	float dy;
	float Vel;
	int counter;  //×´Ì¬×ª»»¼ÆÊý
	int state;
	int damage;
	int score;
	int shield;
	int ammo;     //Íæ¼Òµ¯Ò©
	int Numships;
public:
	CAnimation meteor;//Á÷ÐÇºÅ·É´¬
};
