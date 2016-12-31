#include "dragon.h"
#include "Burst.h"
#include "Rock.h"
#include "DH.h"
#include "Hud.h"
#include "Battleship.h"
#include "Mine.h"
#include "Plasma.h"
#include "Star.h"
#include "Station.h"
#include "Powerup.h"

#pragma warning(disable:4244)
#pragma warning(disable:4996)

extern HWND hWnd;
extern HINSTANCE hInstance;
extern float cosTable[16];
extern float sinTable[16];

#define WINDOW_APP   0


CDSound dsound;
CRenderSystem render;
CInputSystem* input8=NULL;
CGUISystem gui;

CDH dh;
CBurst burst;
CParticleSystem ps;
CMine mine;
CBattleship battleship;
CStation station;
CRock rock;
CPlasma plasma;
CPowerup powerup;
CHud hud;
CStar star;
//CErrorLog errorlog;

int mainbgkId=-1;
int gameoverId=-1;
int mainstartInfo=-1;
int engineId=-1;
int GameState=GAME_STATE_INIT;
int deadId=-1;
int windowClose=0;
int readyCounter = 0;   
int readyState   = 0;


BOOL bRunGame;

DWORD startClockCount;
int windowMode;

char buffer[256];

int WINAPI WinMain (HINSTANCE hinstance, 
					HINSTANCE hPrevInstance,
					PSTR szCmdLine, 
					int iCmdShow)
{
	HWND        hwnd ;
	MSG         msg ;
	WNDCLASSEX  wndclass ;

	wndclass.cbSize        = sizeof (wndclass) ;
	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = hinstance ;
	wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = ClassName ;
	wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;

	RegisterClassEx (&wndclass) ;

	hwnd = CreateWindowA(ClassName, "SCOTT",
		WINDOW_APP?(WS_OVERLAPPED):(WS_POPUP|WS_VISIBLE),
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hinstance, NULL) ;
	hWnd=hwnd;
	hInstance=hinstance;

	if(WINDOW_APP)
	{
		RECT windowRect={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
		AdjustWindowRectEx(&windowRect,GetWindowStyle(hWnd),
			GetMenu(hWnd)!=NULL,GetWindowExStyle(hWnd));

		MoveWindow(hWnd,200,100,
			windowRect.right-windowRect.left,windowRect.bottom-windowRect.top,TRUE);

	}

	ShowWindow (hwnd, iCmdShow) ;
	UpdateWindow (hwnd) ;

	GameInit();

	while (1) 
	{ 
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message==WM_QUIT)
				break;
			TranslateMessage (&msg) ;
			DispatchMessage (&msg) ; 
		}
		else
		{
			GameMain();
		}
	}

	GameShutdown();
	UnregisterClassA(ClassName,hinstance);
	return msg.wParam ;
} 

LRESULT APIENTRY WndProc(HWND hWnd, UINT message,WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ACTIVATE:
		{
			if (LOWORD(wParam)==WA_INACTIVE)
			{
				bRunGame=FALSE;
			}
			else
			{
				bRunGame=TRUE;
			}
		}
		return 0;
	case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;
			hdc=::BeginPaint(hWnd,&ps);
			::EndPaint(hWnd,&ps);
			return 0;
		}
	case WM_KEYUP:
		if(wParam==VK_ESCAPE)
		{
			::PostQuitMessage(0);
		}
		return 0;
	case WM_DESTROY:
		{
			::PostQuitMessage(0);
			windowClose=1;
		}
		return 0;
	}
	return DefWindowProc (hWnd, message, wParam, lParam) ;
}

void LoadSoundMusic()
{
	int iret=-1;
	if ((mainbgkId=dsound.DSoundLoadWave("../res/sound/STARSNG.WAV"))==-1)
	{
		MessageBox(hWnd,"加载声音文件失败1！","",MB_OK);
		return ;
	}

	if ((mainstartInfo=dsound.DSoundLoadWave("../res/sound/INTRO.wav"))==-1)
	{
		MessageBox(hWnd,"加载声音文件失败2！","",MB_OK);
		return ;
	}

	if ((deadId=dsound.DSoundLoadWave("../res/sound/BREAKUP.wav"))==-1)
	{
		MessageBox(hWnd,"加载声音文件失败3！","",MB_OK);
		return ;
	}

	if ((engineId=dsound.DSoundLoadWave("../res/sound/ENGINES.wav"))==-1)
	{
		MessageBox(hWnd,"加载声音文件失败4！","",MB_OK);
		return ;
	}

	if ((gameoverId=dsound.DSoundLoadWave("../res/sound/GAMEOVER.wav"))==-1)
	{
		MessageBox(hWnd,"加载声音文件失败5！","",MB_OK);
		return ;
	}

	if ((iret=dsound.DSoundLoadWave("../res/sound/MINEPOWER1.wav"))==-1)
	{
		MessageBox(hWnd,"加载声音文件失败6！","",MB_OK);
		return ;
	}
	mine.SetPowerupId(iret);

	if ((iret=dsound.DSoundLoadWave("../res/sound/STATIONTHROB2.wav"))==-1)
	{
		MessageBox(hWnd,"加载声音文件失败7！","",MB_OK);
		return ;
	}
	station.SetId(iret);

	if ((iret=dsound.DSoundLoadWave("../res/sound/AMMOPU.wav"))==-1)
	{
		MessageBox(hWnd,"加载声音文件失败8！","",MB_OK);
		return;
	}
	powerup.SetAmmoId(iret);


	if ((iret=dsound.DSoundLoadWave("../res/sound/SHLDPU.wav"))==-1)
	{
		MessageBox(hWnd,"加载声音文件失败9！","",MB_OK);
		return;
	}
	powerup.SetShldId(iret);

	if((iret=dsound.DSoundLoadWave("../res/sound/EXPL1.wav"))==-1)
	{
		MessageBox(hWnd,"加载声音文件失败10！","",MB_OK);
		return ;
	}
	burst.SetExpId(0,iret);

	if((iret=dsound.DSoundLoadWave("../res/sound/EXPL2.wav"))==-1)
	{
		MessageBox(hWnd,"加载声音文件失败11！","",MB_OK);
		return ;
	}
	burst.SetExpId(1,iret);

	for (int i=2;i<MAX_EXPL_SOUND;i++)
	{
		burst.SetExpId(i,dsound.ReplicateSound(burst.GetExpId(rand()%2)));
	}

	if ((iret=dsound.DSoundLoadWave("../res/sound/PULSE.wav"))==-1)
	{
		MessageBox(hWnd,"加载声音文件失败12！","",MB_OK);
		return;
	}
	plasma.SetFireId(0,iret);

	for (int i=1;i<MAX_FIRE_SOUND;i++)
	{
		plasma.SetFireId(i,dsound.ReplicateSound(plasma.GetFireId(0)));
	}
}
void CopyScreen(UCHAR* sourBitmap,UCHAR* destBuffer,int lpitch,int transpare)
{
	UCHAR* destAddr,*sourAddr;
	UCHAR pixel;

	destAddr=destBuffer;
	sourAddr=sourBitmap;

	if (transpare)
	{
		for (int i=0;i<SCREEN_HEIGHT;i++)
		{
			for (int x=0;x<SCREEN_WIDTH;x++)
			{
				if((pixel=sourAddr[x])!=0)
				{
					destAddr[x]=pixel;
				}
			}
			destAddr+=lpitch;
			sourAddr+=SCREEN_WIDTH;
		}
	}
	else
	{
		for (int i=0;i<SCREEN_HEIGHT;i++)
		{
			memcpy(destAddr,sourAddr,SCREEN_WIDTH);
			destAddr+=lpitch;
			sourAddr+=SCREEN_WIDTH;
		}
	}
}

void CopyScreen16(UCHAR* sourBitmap,UCHAR* destBuffer,int lpitch,int transpare)
{
	USHORT* destAddr,*sourAddr;
	USHORT pixel;
	int lpitch16=lpitch>>1;

	destAddr=(USHORT*)destBuffer;
	sourAddr=(USHORT*)sourBitmap;

	if (transpare)
	{
		for (int i=0;i<SCREEN_HEIGHT;i++)
		{
			for (int x=0;x<SCREEN_WIDTH;x++)
			{
				if((pixel=sourAddr[x])!=0)
				{
					destAddr[x]=pixel;
				}
			}
			destAddr+=lpitch16;
			sourAddr+=SCREEN_WIDTH;
		}
	}
	else
	{
		for (int i=0;i<SCREEN_HEIGHT;i++)
		{
			memcpy(destAddr,sourAddr,SCREEN_WIDTH);
			destAddr+=lpitch16;
			sourAddr+=SCREEN_WIDTH;
		}
	}
}

void LoadStartInfo()
{
	render.DDrawFillSurface(render.lpddsback,0);
	render.DDrawFillSurface(render.lpddsprimary,0);

	CBitmapFile bitmap16bit;
	bitmap16bit.LoadBitmapFromFile("../res/picture/mainmenu.bmp");
	
	render.DDrawLockBackSurface();
	CopyScreen16(bitmap16bit.buffer,render.backBuffer,render.backlpitch,1);
	Sleep(3000);
	render.DDrawUnLockBackSurface();
	render.FlipSurface();
	/*if (WINDOW_APP)
	{
		render.DDrawLockBackSurface();
		CopyScreen16(bitmap16bit.buffer,render.backBuffer,render.backlpitch,1);
		render.ScreenTransitions(SCREEN_SWIPE_X,render.backBuffer,render.backlpitch);
		render.DDrawUnLockBackSurface();
		render.FlipSurface();
	}
	else
	{
		render.DDrawLockPrimarySurface();
		CopyScreen16(bitmap16bit.buffer,render.primaryBuffer,render.primarylpitch,1);
		render.ScreenTransitions(SCREEN_SWIPE_X,render.primaryBuffer,render.primarylpitch);
		render.DDrawUnLockPrimarySurface();
	}*/

	bitmap16bit.UnLoadBitmapFile();
	Sleep(5000);

	

	render.DDrawFillSurface(render.lpddsback,0);
	render.DDrawFillSurface(render.lpddsprimary,0);
}

void DrawInfo()
{
	char score[16];
	char buf[80];
	static int damage=0;
	static int warningCount=0;

	wsprintf(score,"0000000%d",dh.GetScore());
	wsprintf(buf,"SCORE %s",&score[strlen(score)-8]);
	gui.DrawTextGDI(buf,10,10,RGB(0,255,0),render.lpddsback);

	wsprintf(buf,"SHIELDS %d",100-dh.GetDamage());
	if (dh.GetDamage() < 90)
		gui.DrawTextGDI(buf,350-8*strlen(buffer),10,RGB(0,255,0),render.lpddsback);
	else
		gui.DrawTextGDI(buf,350-8*strlen(buffer),10,RGB(damage,0,0),render.lpddsback);

	if ((damage+=15) > 255)
		damage = 0;
	wsprintf(buffer,"SHIPS %d",dh.GetNumShips());
	gui.DrawTextGDI(buffer,520,10,RGB(0,255,0),render.lpddsback);

	// 	wsprintf(buffer,"VEL %.2f Kps",vel*(100/MAX_PLAYER_SPEED));
	// 	gui.DrawTextGDI(buffer,10,460,RGB(0,255,0),render.lpddsback);

	wsprintf(buffer,"POS [%.2f,%.2f]",dh.GetX(), dh.GetY());
	gui.DrawTextGDI(buffer,480,460,RGB(0,255,0),render.lpddsback);

	wsprintf(buffer,"AMMO %d",dh.GetAmmo());
	gui.DrawTextGDI(buffer,280,460,RGB(0,255,0),render.lpddsback);
}

void DrawScanner()
{
	int sx,sy;
	//画所有的岩石
	for (int index=0;index<MAX_ROCKS;index++)
	{
		if (rock.rocks[index].state==ROCK_STATE_ON)
		{
			//缩小为原来的1/128
			sx=((rock.rocks[index].varsI[INDEX_WORLD_X]-UNIVERSE_MIN_X)>>7)
				+(SCREEN_WIDTH/2)- ((UNIVERSE_MAX_X - UNIVERSE_MIN_X) >> 8);
			sy=((rock.rocks[index].varsI[INDEX_WORLD_Y]-UNIVERSE_MIN_Y)>>7)+32;
			render.DrawPixel(sx-236-10,sy+10+304,_RGB16BIT565(255,255,255));
		}
	}

	for (int i=0;i<MAX_BATTLESHIPS;i++)
	{
		if (battleship.battleships[i].state==BATTLESHIP_STATE_ALIVE)
		{
			//缩小为原来的1/128
			sx=((battleship.battleships[i].varsI[INDEX_WORLD_X]-UNIVERSE_MIN_X)>>7)
				+(SCREEN_WIDTH/2)- ((UNIVERSE_MAX_X - UNIVERSE_MIN_X) >> 8);
			sy=((battleship.battleships[i].varsI[INDEX_WORLD_Y]-UNIVERSE_MIN_Y)>>7)+32;
			render.DrawPixel(sx-10-236,sy+10+304,_RGB16BIT565(254,186,194));
			render.DrawPixel(sx-10+1-236,sy+10+304,_RGB16BIT565(254,186,194));
		}
	}

	for (int i=0;i<MAX_MINES;i++)
	{
		if (mine.mines[i].state==MINE_STATE_ALIVE)
		{
			//缩小为原来的1/128
			sx=((mine.mines[i].varsI[INDEX_WORLD_X]-UNIVERSE_MIN_X)>>7)
				+(SCREEN_WIDTH/2)- ((UNIVERSE_MAX_X - UNIVERSE_MIN_X) >> 8);
			sy=((mine.mines[i].varsI[INDEX_WORLD_Y]-UNIVERSE_MIN_Y)>>7)+32;
			render.DrawPixel(sx-10-236,sy+10+304,_RGB16BIT565(214,216,226));
			render.DrawPixel(sx-10-236,sy+10+304+1,_RGB16BIT565(214,216,226));
		}
	}

	for (int index=0; index < MAX_STATIONS; index++)
	{
		if (station.stations[index].state==STATION_STATE_ALIVE)
		{
			sx = ((station.stations[index].varsI[INDEX_WORLD_X] - UNIVERSE_MIN_X) >> 7) + (SCREEN_WIDTH/2) - ((UNIVERSE_MAX_X - UNIVERSE_MIN_X) >> 8);
			sy = ((station.stations[index].varsI[INDEX_WORLD_Y] - UNIVERSE_MIN_Y) >> 7) + 32;
			
			if (station.stations[index].state == STATION_SHIELDS_ANIM_ON)
			{
				station.SmallStations.currFrame= 0;
				station.SmallStations.x = sx-10-236 -3;
				station.SmallStations.y = sy+304+10 -3; 
				station.SmallStations.RenderAnimation(&render,render.lpddsback);           
			} 
			else
			{
				station.SmallStations.currFrame = 1;
				station.SmallStations.x = sx-10-236 - 3;
				station.SmallStations.y = sy+304+10 - 3; 
				station.SmallStations.RenderAnimation(&render,render.lpddsback); 
			} 
		} 
	}
	//画玩家，作为白色
	sx = (((int)dh.GetX()- UNIVERSE_MIN_X) >> 7) + (SCREEN_WIDTH/2) - ((UNIVERSE_MAX_X - UNIVERSE_MIN_X) >> 8);
	sy = (((int)dh.GetY()- UNIVERSE_MIN_Y) >> 7) + 32;

	int col=_RGB16BIT565(rand()%256,rand()%256,rand()%256);
	render.DrawPixel(sx-10-236,sy+10+304,col);
	render.DrawPixel(sx-10-236+1,sy+10+304,col);
	render.DrawPixel(sx-10-236,sy+10+304+1,col);
	render.DrawPixel(sx-10-236+1,sy+10+304+1,col);

	//画边框
	hud.huds.x          = 0;
	hud.huds.y          = 332;
	hud.huds.currFrame  = 0;
	hud.huds.RenderAnimation(&render,render.lpddsback);

	hud.huds.x          = 113;
	hud.huds.y          = 332;
	hud.huds.currFrame  = 1;
	hud.huds.RenderAnimation(&render,render.lpddsback);

	hud.huds.x          = 0;
	hud.huds.y          = 440;
	hud.huds.currFrame  = 2;
	hud.huds.RenderAnimation(&render,render.lpddsback);


	hud.huds.x          = 113;
	hud.huds.y          =440;
	hud.huds.currFrame  = 3;
	hud.huds.RenderAnimation(&render,render.lpddsback);

}
void ResetGame()
{
	dh.SetState(PLAYER_STATE_ALIVE);
	dh.SetScore(0);   // 玩家的分
	dh.SetNumShips(3);   // 玩家飞船数
	dh.SetDamage(0);   // 玩家伤害
	dh.SetCounter(0);   // 状态转变记数
	dh.SetAmmo(1000); // 玩家炮弹
	dh.SetShield(0);  //玩家防护罩
	station.SetNumDamageStation(0) ; // 摧毁的航空站

	dh.ResetPos();
	dh.SetVel(0);
	dh.meteor.varsI[INDEX_METEOR_DIR]= 0;
	station.SetId(-1);

	readyCounter = 0;    
	readyState   = 1;

	ps.ResetParticle();
	battleship.ResetBattleShip();
	mine.ResetMine();
	rock.ResetRock();
	plasma.ResetPlasma();
	burst.ResetBurst();
	station.ResetStation();
	powerup.ResetPowerup();

	rock.SeekRock();

	for (int index=0; index < NUM_ACTIVE_BATTLESHIPS; index++)
		battleship.StartBattleShip();

	for (int index=0; index < NUM_ACTIVE_MINES; index++)
		mine.StartMine();

	for (int index=0; index < NUM_ACTIVE_STATIONS; index++)
		station.StartStation();
}
int GameInit(void* parms)
{
	srand(render.StartClock());
	
	CreateTable();
	
	//errorlog.CreateErrorFile();

	render.CreateDDraw(hWnd,SCREEN_WIDTH,SCREEN_HEIGHT,SCREEN_BPP,WINDOW_APP);

	dsound.DSoundInit();

	LoadSoundMusic();

	input8=new CInputSystem(FALSE);

	plasma.InitPlasma();

	star.InitStar();

	rock.InitRock();
	rock.SeekRock();

	burst.InitBurst();

	mine.InitMine();

	battleship.InitBattleShip();

	station.InitStation();

	powerup.InitPowerup();
	//
	dsound.play(mainstartInfo,0);

	dh.InitDH();

	hud.InitHud();

	LoadStartInfo();

	ShowCursor(FALSE);

	return true;
}
int GameShutdown(void* parms)
{
	dsound.ShutDown();
	if(input8!=NULL)
	{
		delete input8;
		input8=NULL;
	}
	render.Release();
	return true;
}
int GameMain(void* parms)
{
	float xv=0.0,yv=0.0;
	static BOOL ToggleEngine=FALSE;
	static BOOL ToggleHud=TRUE;
	static BOOL ToggleInfo=TRUE;
	static BOOL TogglePause=FALSE;
	
	input8->UpateDevice();

	if (GameState==GAME_STATE_INIT)
	{
		GameState=GAME_STATE_RESTART;
	}
	else if (GameState==GAME_STATE_MENU)
	{
		GameState=GAME_STATE_RESTART;
	}
	else if (GameState==GAME_STATE_RESTART)
	{
		dsound.play(mainbgkId,DSBPLAY_LOOPING);
		ResetGame();
		GameState=GAME_STATE_RUNNING;
	}
	else if (GameState==GAME_STATE_RUNNING)
	{
		render.StartClock();
		//清除后备缓冲
		render.DDrawFillSurface(render.lpddsback,0);

		if (dh.GetState()==PLAYER_STATE_ALIVE)
		{
			if (input8->KeyDown(DIK_EQUALS))
			{
				mine.SetTrackingRate(mine.GetTrackingRate()+0.1);
				if (mine.GetTrackingRate()>4)
				{
					mine.SetTrackingRate(4);
				}
			}
			else if (input8->KeyDown(DIK_MINUS))
			{
				mine.SetTrackingRate(mine.GetTrackingRate()-0.1);
				if (mine.GetTrackingRate()<0)
				{
					mine.SetTrackingRate(0);
				}
			}

			if (input8->KeyDown(DIK_RIGHT))
			{
				if(++dh.meteor.varsI[INDEX_METEOR_DIR]>15)
				{
					dh.meteor.varsI[INDEX_METEOR_DIR]=0;
				}
			}
			else if (input8->KeyDown(DIK_LEFT))
			{
				if (--dh.meteor.varsI[INDEX_METEOR_DIR]<0)
				{
					dh.meteor.varsI[INDEX_METEOR_DIR]=15;
				}
			}

			if (input8->KeyDown(DIK_UP))
			{
				xv = cosTable[dh.meteor.varsI[INDEX_METEOR_DIR]];
				yv = sinTable[dh.meteor.varsI[INDEX_METEOR_DIR]];

				if (!ToggleEngine)
				{
					dsound.play(engineId,DSBPLAY_LOOPING);
					ToggleEngine^=1;
				}

				ps.StartParticle(PARTICLE_TYPE_FADE,PARTICLE_COLOR_GREEN,3,
					dh.GetX()+RAND_RANGE(-2,2),dh.GetY()+RAND_RANGE(-2,2),
					-int(dh.GetXV())>>3,-int(dh.GetYV())>>3);
			}
			else if (ToggleEngine)
			{
				ToggleEngine^=1;
				dsound.play(engineId,0);
			}
			//显示/关闭 外框
			if (input8->KeyDown(DIK_S))
			{
				ToggleHud^=1;
			}
			if (input8->KeyDown(DIK_H))
			{
				ToggleInfo^=1;
			}
			
			if (input8->KeyDown(DIK_P))
			{
				/*TogglePause^=1;*/
				GameState=GAME_STATE_PAUSED;
			}

			dh.SetXV(dh.GetXV()+xv);
			dh.SetYV(dh.GetYV()+yv);
			dh.SetVel(Distance2D(dh.GetXV(),dh.GetYV()));

			if (dh.GetVel()>=MAX_PLAYER_SPEED)
			{
				dh.SetXV(MAX_PLAYER_SPEED*(dh.GetXV()/dh.GetVel()));
				dh.SetYV(MAX_PLAYER_SPEED*(dh.GetYV()/dh.GetVel()));
			}

			dh.SetX(dh.GetX()+dh.GetXV());
			dh.SetY(dh.GetY()+dh.GetYV());

			if (dh.GetX()>UNIVERSE_MAX_X)
			{
				dh.SetX(UNIVERSE_MIN_X);
			}
			else if (dh.GetX()<UNIVERSE_MIN_X)
			{
				dh.SetX(UNIVERSE_MAX_X);
			}
			if (dh.GetY()>UNIVERSE_MAX_Y)
			{
				dh.SetY(UNIVERSE_MIN_Y);
			}
			else if (dh.GetY()<UNIVERSE_MIN_Y)
			{
				dh.SetY(UNIVERSE_MAX_X);
			}

			//玩家发射炮弹
			if (input8->KeyDown(DIK_LCONTROL)||input8->KeyDown(DIK_SPACE))
			{
				if (dh.GetAmmo()>0)
				{
					float plasma_xv = cosTable[dh.meteor.varsI[INDEX_METEOR_DIR]]*PLASMA_SPEED;
					float plasma_yv = sinTable[dh.meteor.varsI[INDEX_METEOR_DIR]]*PLASMA_SPEED;
					//炮弹方向
					int dir=dh.meteor.varsI[INDEX_METEOR_DIR];

					
					plasma.FirePlasma(dh.GetX(),dh.GetY(),
						dh.GetXV()+plasma_xv,
						dh.GetYV()+plasma_yv,dir, PLASMA_ANIM_PLAYER);

					if (dh.GetAmmo() > 0)
						dh.SetAmmo(--dh.GetAmmo());
				}
			}

			//检查状态转换
			if (dh.GetDamage() >= 100 && dh.GetState() == PLAYER_STATE_ALIVE)
			{
				dh.SetDamage(100);
				dh.SetState(PLAYER_STATE_DYING);
				dh.SetNumShips(--dh.GetNumShips());
				dh.SetCounter(0);
				ToggleEngine = FALSE;
				dsound.play(deadId,0);
			} 

		} 
		else if (dh.GetState() == PLAYER_STATE_DYING)
		{ 
			// 玩家将要玩完了

			// 开始随机的爆炸
			int bwidth  = 16 + rand()%64;

			if ((rand()%4)==1)
				burst.StartBurst(&ps,dh.GetX()+rand()%40, dh.GetY()+rand()%8,
				bwidth, (bwidth >> 2) + (bwidth >> 1),
				-4+rand()%8,2+rand()%4);    

			dh.meteor.varsI[INDEX_METEOR_DIR] += (RAND_RANGE(-1,1));

			if (dh.meteor.varsI[INDEX_METEOR_DIR] > 15)
				dh.meteor.varsI[INDEX_METEOR_DIR] = 0;
			else if (dh.meteor.varsI[INDEX_METEOR_DIR] < 0)
				dh.meteor.varsI[INDEX_METEOR_DIR] = 15;

			if (++dh.GetCounter() > 150)
			{  
				dh.SetCounter(0);
				dh.SetState(PLAYER_STATE_INVINCIBLE);
				dh.SetDamage(0);
			} 
		} 
		else if (dh.GetState() == PLAYER_STATE_INVINCIBLE)
		{
			//玩家是无敌的
			if (++dh.GetCounter() > 70)
			{
				dh.SetState(PLAYER_STATE_ALIVE);

				dh.SetCounter(0);

				//重设玩家的位置
				dh.ResetPos();

				if (dh.GetNumShips() == 0)
				{
					//重设所有变量
					dh.SetState(PLAYER_STATE_DEAD);
					dh.SetCounter(0);
					readyCounter = 0;
					readyState   = 0;
					//关闭引擎
					dsound.play(engineId,0);
					//玩完
					dsound.play(gameoverId,0);
				} 
			}
			// 开始准备
			if ((dh.GetCounter() == 20) && (dh.GetNumShips() > 0))
			{
				// 重设到准备状态
				readyCounter = 0; 
				readyState = 1;
			}
		} 

		powerup.MovePowerup();

		station.MoveStation();

		battleship.MoveBattleShip();

		mine.MoveMine();

		rock.MoveRock();

		star.MoveStar();

		ps.MoveParticle();

		burst.MoveBurst();

		star.DrawStar();

		powerup.DrawPowerup();

		station.DrawStation();

		battleship.DrawBattleShip();

		rock.DrawRock();

		mine.DrawMine();

		ps.DrawParticle(&render);

		plasma.DrawPlasma();

		plasma.MovePlasma();

		if (dh.GetState() == PLAYER_STATE_ALIVE || dh.GetState() == PLAYER_STATE_DYING)
		{	
			if (ToggleEngine)
				dh.meteor.currFrame = dh.meteor.varsI[INDEX_METEOR_DIR] + 16*(rand()%2);
			else
				dh.meteor.currFrame= dh.meteor.varsI[INDEX_METEOR_DIR];

			dh.meteor.RenderAnimation(&render,render.lpddsback);

			if (dh.GetShield()>0)
			{
				dh.SetShield(--dh.GetShield());
				dh.meteor.currFrame = 34-dh.GetShield();    
				dh.meteor.RenderAnimation(&render,render.lpddsback);
			}

		} 

		burst.DrawBurst();

		if (ToggleInfo)
			DrawInfo();

		if (ToggleHud)
			DrawScanner();

		if (dh.GetState()== PLAYER_STATE_DEAD)
		{
			gui.DrawTextGDI("G A M E   O V E R",(SCREEN_WIDTH/2) - 8*(strlen("G A M E   O V E R")>>1),
				SCREEN_HEIGHT/2,RGB(0,255,0),render.lpddsback);
		} 
		
		if (dh.GetState()== PLAYER_STATE_WIN)
		{
			gui.DrawTextGDI("SUCCESSFULL!",(SCREEN_WIDTH/2) - 8*(strlen("SUCCESSFULL!")>>1),
				SCREEN_HEIGHT/2,RGB(0,255,0),render.lpddsback);
		}

		if (readyState)
		{
			gui.DrawTextGDI("A R E  Y O U  R E A D Y!",320-8*strlen("A R E  Y O U  R E A D Y!")/2,
				200,RGB(0,rand()%255,0),render.lpddsback);
			if (++readyCounter > 60)
			{
				readyState = 0;
				readyCounter = 0;
			} 

		} 

		render.FlipSurface();
		render.WaitClock(30);

		if (++dh.GetCounter() > 10)
		{
			if (input8->KeyDown(DIK_ESCAPE))
			{
				GameState = GAME_STATE_EXIT;
				dsound.StopSound(engineId);
			} 
		}
	}	
	else if (GameState == GAME_STATE_PAUSED)
	{
		if (input8->KeyUp(DIK_P))
		{
			GameState=GAME_STATE_RUNNING;
		}
		
		/*if (TogglePause)
		{
			TogglePause^=1;
			
		}*/
		gui.DrawTextGDI("G A M E  P A U S E D  -- P R E S S  <P>",320-8*strlen("G A M E  P A U S E D  -- P R E S S  <P>")/2, 
			200, RGB(0,255,0),render.lpddsback);
		/*else
			GameState= GAME_STATE_RUNNING;*/

		
	} 
	else if (GameState == GAME_STATE_EXIT)
	{
		PostMessage(hWnd, WM_DESTROY,0,0);
		//errorlog.CloseFile();
		GameState = GAME_STATE_WAITING_FOR_EXIT;
		
		render.DDrawLockPrimarySurface();
		render.ScreenTransitions(SCREEN_SWIPE_X, render.primaryBuffer, render.primarylpitch);
		render.DDrawUnLockPrimarySurface();

		render.DDrawFillSurface(render.lpddsback, 0);
		render.DDrawFillSurface(render.lpddsprimary, 0);
	}
	else if	(GameState == GAME_STATE_WAITING_FOR_EXIT)
	{
		

	} 

	return TRUE;

}
