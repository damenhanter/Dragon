#include "Star.h"
#include "DH.h"
extern CDH dh;
extern CRenderSystem render;

CStar::CStar(void)
{
}

CStar::~CStar(void)
{
}

void CStar::InitStar()
{
	for (int i=0;i<MAX_STARS;i++)
	{
		switch (rand()%3)
		{
		case STAR_PLANE_0:
			stars[i].color=_RGB16BIT565(255,255,255);
			stars[i].plane=STAR_PLANE_0;
			break;
		case STAR_PLANE_1:
			stars[i].color=_RGB16BIT565(255,255,255);
			stars[i].plane=STAR_PLANE_1;
			break;
		case STAR_PLANE_2:
			stars[i].color=_RGB16BIT565(255,255,255);
			stars[i].plane=STAR_PLANE_2;
			break;
		}
		stars[i].x=rand()%(SCREEN_WIDTH);
		stars[i].y=rand()%(SCREEN_HEIGHT);
	}
}

//star总是用屏幕坐标，我们只在屏幕范围内画star，这样就不用在整个星空里画star
void CStar::MoveStar()
{
	int starX,starY,plane_0_dx,plane_0_dy,
		plane_1_dx,plane_1_dy,plane_2_dx,plane_2_dy;
	plane_0_dx=-((int)dh.GetXV()>>2);
	plane_0_dy=-((int)dh.GetYV()>>2);
	plane_1_dx=-((int)dh.GetXV()>>1);
	plane_1_dy=-((int)dh.GetYV()>>1);
	plane_2_dx=-dh.GetXV();
	plane_2_dy=-dh.GetYV();
	//根据玩家的运动移动所有的星星
	for(int i=0;i<MAX_STARS;i++)
	{
		starX=stars[i].x;
		starY=stars[i].y;
		switch (stars[i].plane)
		{
		case STAR_PLANE_0:
			starX+=plane_0_dx;
			starY+=plane_0_dy;
			break;
		case STAR_PLANE_1:
			starX+=plane_1_dx;
			starY+=plane_1_dy;
			break;
		case STAR_PLANE_2:
			starX+=plane_2_dx;
			starY+=plane_2_dy;
			break;
		}
		if (starX>=SCREEN_WIDTH-1)
		{
			starX-=SCREEN_WIDTH-1;
		}
		else if (starX<0)
		{
			starX+=SCREEN_WIDTH-1;
		}
		if (starY>=SCREEN_HEIGHT-1)
		{
			starY-=SCREEN_HEIGHT-1;
		}
		else if (starY<0)
		{
			starY+=SCREEN_HEIGHT-1;
		}
		stars[i].x=starX;
		stars[i].y=starY;
	}

}

void CStar::DrawStar()
{
	for (int i=0;i<MAX_STARS;i++)
	{
		render.DrawPixel(stars[i].x,stars[i].y,stars[i].color);
	}
}
