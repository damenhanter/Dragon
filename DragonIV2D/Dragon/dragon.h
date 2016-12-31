#pragma once

#include "../DragonEngine/engine.h"
#pragma comment(lib,"../lib/DragonEngine.lib")



#define UNIVERSE_MIN_X                 (-8000)  //游戏地图大小
#define UNIVERSE_MAX_X                 (8000)
#define UNIVERSE_MIN_Y                 (-8000)
#define UNIVERSE_MAX_Y                 (8000)

#define MAX_PLASMA                     20
#define PLASMA_SPEED                   16
#define PLASMA_SPEED_SLOW              12
#define PLASMA_STATE_OFF               0
#define PLASMA_STATE_ON                1
#define PLASMA_RANGE_1                 30
#define PLASMA_ANIM_PLAYER             0
#define PLASMA_ANIM_ENEMY              1
#define INDEX_PLASMA_DIR               0
#define INDEX_PLASMA_TYPE              1    


#define MAX_ROCKS                      300
#define ROCK_STATE_OFF                 0
#define ROCK_STATE_ON                  1
#define ROCK_LARGE                     0
#define ROCK_MEDIUM                    1
#define ROCK_SMALL                     2

#define MAX_BURSTS                     8
#define BURST_STATE_OFF                0
#define BURST_STATE_ON                 1

#define PLAYER_STATE_DEAD              0
#define PLAYER_STATE_DYING             1
#define PLAYER_STATE_INVINCIBLE        2
#define PLAYER_STATE_ALIVE             3
#define PLAYER_STATE_WIN               4
#define PLAYER_REGEON_COUNT            100

#define INDEX_METEOR_DIR               3

#define MAX_PLAYER_SPEED               16

#define MAX_FIRE_SOUND                 8
#define MAX_EXPL_SOUND                 8

#define MAX_BATTLESHIPS                   8
#define BATTLESHIP_RIGHT                  0
#define BATTLESHIP_LEFT                   1
#define BATTLESHIP_RANGE_RING             2000

#define MAX_BATTLESHIP_DAMAGE             150
#define INDEX_BATTLESHIP_DIR              3
#define INDEX_BATTLESHIP_DAMAGE           2
#define INDEX_BATTLESHIP_TURRET           4

#define BATTLESHIP_STATE_DEAD              0
#define BATTLESHIP_STATE_DYING             3
#define BATTLESHIP_STATE_DAMAGED           2
#define BATTLESHIP_STATE_ALIVE             1
#define BATTLESHIP_MIN_ATTACK_RANGE        500
#define BATTLESHIP_SPEED                   4
#define POWERUP_TYPE_AMMO               0
#define POWERUP_TYPE_SHLD               1
#define POWERUP_TYPE_SOURCE				2

#define POWERUP_STATE_DEAD              0
#define POWERUP_STATE_ALIVING           1

#define MAX_POWERUPS                    16
#define INDEX_POWERUPS_TYPE             0

#define MAX_STATIONS                    20

#define STATION_STATE_DEAD              0
#define STATION_STATE_DYING             3
#define STATION_STATE_DAMAGED           2
#define STATION_STATE_ALIVE             1

#define STATION_RANGE_RING              300
#define INDEX_STATION_DAMAGE            2
#define MAX_STATION_DAMAGE              100

#define STATION_SHIELDS_ANIM_ON			0  
#define STATION_SHIELDS_ANIM_OFF		1 

#define STATION_MIN_UPLINK_DISTANCE     150

#define MAX_MINES                       16

#define MINE_STATE_DEAD                0
#define MINE_STATE_DYING               3
#define MINE_STATE_DAMAGED             2
#define MINE_STATE_ALIVE               1

#define MINE_STATE_AI_ACTIVATED        1
#define MINE_STATE_AI_SLEEP            0

#define INDEX_MINE_AI_STATE            1
#define INDEX_MINE_DAMAGE              2
#define INDEX_MINE_CONTRACT_COUNT      3
#define INDEX_MINE_DIR                 4

#define MAX_MINE_DAMAGE                50
#define MAX_MINE_CONTRACT_COUNT        20

#define MAX_MINE_VELOCITY              16
#define MIN_MINE_TRACKING_DIST         1000
#define MIN_MINE_ACTIVATION_DIST       250

#define MAX_STARS                      256
#define STAR_PLANE_0                   0
#define STAR_PLANE_1                   1
#define STAR_PLANE_2                   2

#define STAR_COLOR_0                   255
#define STAR_COLOR_1                   254
#define STAR_COLOR_2                   253

#define GAME_STATE_INIT                0
#define GAME_STATE_MENU                1
#define GAME_STATE_RESTART             2
#define GAME_STATE_RUNNING             3
#define GAME_STATE_UPLINK              4
#define GAME_STATE_EXIT                5
#define GAME_STATE_WAITING_FOR_EXIT    6
#define GAME_STATE_PAUSED              7

#define MENU_STATE_MAIN                0
#define MENU_STATE_INST                1
#define MENU_SEL_NEW_GAME              0
#define MENU_SEL_INSTRUCTIONS          1
#define MENU_SEL_EXIT                  2
#define MAX_INSTRUCTION_PAGES          6

#define FONT_NUM_CHARS                 96
#define FONT_WIDTH                     12
#define FONT_HEIGHT                    12
#define FONT_WIDTH_NEXT_NUM            8
#define FONT_WIDTH_NEXT_LOWER          7
#define FONT_WIDTH_NEXT_UPPER          8

#define NUM_ACTIVE_BATTLESHIPS			   8
#define NUM_ACTIVE_MINES               8
#define NUM_ACTIVE_STATIONS            8






int GameInit(void* parms=NULL);
int GameShutdown(void* parms=NULL);
int GameMain(void* params=NULL);

LRESULT APIENTRY WndProc (HWND, UINT, WPARAM, LPARAM);

