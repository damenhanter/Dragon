// OUTPOST.CPP - OutPost Game Demo

// INCLUDES ///////////////////////////////////////////////

#define INITGUID

#define WIN32_LEAN_AND_MEAN  

#include <windows.h>   // include important windows stuff
#include <windowsx.h> 
#include <mmsystem.h>
#include <iostream> // include important C/C++ stuff
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h> 
#include <math.h>
#include <io.h>
#include <fcntl.h>

#include <ddraw.h>  // directX includes
#include <dsound.h>
#include <dmksctrl.h>
#include <dmusici.h>
#include <dmusicc.h>
#include <dmusicf.h>
#include <dinput.h>
#include "T3DLIB1.h" // game library includes
#include "T3DLIB2.h"
#include "T3DLIB3.h"


//#include "mono.h"

// DEFINES ////////////////////////////////////////////////
#pragma comment (lib,"ddraw.lib")
#pragma comment (lib,"dinput8.lib")
#pragma comment (lib,"dsound.lib")
#pragma comment (lib,"winmm.lib")
// mathematical defines
CONST float FRICTION = 0.1;

// defines for windows 
#define WINDOW_CLASS_NAME "WINCLASS"  // class name

#define WINDOW_WIDTH    640            // size of window viewport
#define WINDOW_HEIGHT   480

// size of universe
#define UNIVERSE_MIN_X   (-8000)
#define UNIVERSE_MAX_X   (8000)
#define UNIVERSE_MIN_Y   (-8000)
#define UNIVERSE_MAX_Y   (8000)

// weapons defines
#define MAX_PLASMA         32   // max number of plasma pulses
#define PLASMA_SPEED       16   // pixels/sec of plasma pulse
#define PLASMA_SPEED_SLOW  12   // slow version for gunships

#define PLASMA_STATE_OFF   0   // this plasma is dead or off
#define PLASMA_STATE_ON    1   // this one is alive and in flight
#define PLASMA_RANGE_1     30  // type 1 plasma disruptor

#define PLASMA_ANIM_PLAYER     0  // a plasma torpedo from player
#define PLASMA_ANIM_ENEMY      1  // a plasma torpedo from any enemy

// asteroid field defines
#define MAX_ROCKS             300
#define ROCK_STATE_OFF        0   // this rock is dead or off
#define ROCK_STATE_ON         1   // this one is alive and in flight

#define ROCK_LARGE            0   // sizes of rock
#define ROCK_MEDIUM           1
#define ROCK_SMALL            2

// explosion defines 
#define MAX_BURSTS            8
#define BURST_STATE_OFF       0   // this burst is dead or off
#define BURST_STATE_ON        1   // this one is alive

// defines for player states
#define PLAYER_STATE_DEAD           0
#define PLAYER_STATE_DYING          1
#define PLAYER_STATE_INVINCIBLE     2
#define PLAYER_STATE_ALIVE          3
#define PLAYER_REGEN_COUNT          100 // click before regenerating

#define WRAITH_INDEX_DIR            3 // index of direction var

#define MAX_PLAYER_SPEED            16

// sound id's
#define MAX_FIRE_SOUNDS       8
#define MAX_EXPL_SOUNDS       8

// defines for gunships
#define MAX_GUNSHIPS          8

#define GUNSHIP_RANGE_RING       1000 // distance from player gunship will be created at

#define GUNSHIP_RIGHT            0
#define GUNSHIP_LEFT             1

#define MAX_GUNSHIP_DAMAGE       150
#define INDEX_GUNSHIP_DAMAGE     2  // damage level of gunship
#define INDEX_GUNSHIP_DIR        3  // direction of ship 
#define INDEX_GUNSHIP_TURRET     4  // direction of turret

// life state for gunships
#define GUNSHIP_STATE_DEAD        0
#define GUNSHIP_STATE_ALIVE       1
#define GUNSHIP_STATE_DAMAGED     2  
#define GUNSHIP_STATE_DYING       3
#define GUNSHIP_MIN_ATTACK_RANGE  500

// defines for powerups
#define POWERUP_TYPE_AMMO       0
#define POWERUP_TYPE_SHLD       1

#define POWERUP_STATE_DEAD      0
#define POWERUP_STATE_ALIVE     1
#define MAX_POWERUPS            16
#define INDEX_POWERUP_TYPE      0


// defines for stations
#define MAX_STATIONS           20

// life state for stations
#define STATION_STATE_DEAD       0
#define STATION_STATE_ALIVE      1
#define STATION_STATE_DAMAGED    2  
#define STATION_STATE_DYING      3

#define STATION_SHIELDS_ANIM_ON  0  // animations for shields
#define STATION_SHIELDS_ANIM_OFF 1 

#define STATION_RANGE_RING       300

#define INDEX_STATION_DAMAGE     2  // tracks the current damage of station
#define MAX_STATION_DAMAGE      100

#define STATION_MIN_UPLINK_DISTANCE  150


// defines for mines
#define MAX_MINES                16

// life state for stations
#define MINE_STATE_DEAD       0
#define MINE_STATE_ALIVE      1
#define MINE_STATE_DAMAGED    2  
#define MINE_STATE_DYING      3

#define MINE_STATE_AI_ACTIVATED   1
#define MINE_STATE_AI_SLEEP       0

#define INDEX_MINE_AI_STATE       1  // state of ai system
#define INDEX_MINE_DAMAGE         2  // tracks the current damage of MINE
#define INDEX_MINE_CONTACT_COUNT  3  // tracks how long mine has been in contact with player

#define MAX_MINE_DAMAGE            50 
#define MAX_MINE_CONTACT_COUNT     20

#define MAX_MINE_VELOCITY        16  
#define MIN_MINE_TRACKING_DIST   1000
#define MIN_MINE_ACTIVATION_DIST 250 

// defines for the star field

#define MAX_STARS                256   // number of stars in universe

#define STAR_PLANE_0             0    // far plane
#define STAR_PLANE_1             1    // near plane
#define STAR_PLANE_2             2

#define STAR_COLOR_0             8    // color of farthest star plane
#define STAR_COLOR_1             7
#define STAR_COLOR_2             15   // color of nearest star plane


// defines for particle system
#define PARTICLE_STATE_DEAD               0
#define PARTICLE_STATE_ALIVE              1

// types of particles
#define PARTICLE_TYPE_FLICKER             0
#define PARTICLE_TYPE_FADE                1 

// color of particle
#define PARTICLE_COLOR_RED                0
#define PARTICLE_COLOR_GREEN              1
#define PARTICLE_COLOR_BLUE               2
#define PARTICLE_COLOR_WHITE              3

#define MAX_PARTICLES                     128

// color ranges
#define COLOR_RED_START                   32
#define COLOR_RED_END                     47

#define COLOR_GREEN_START                 96
#define COLOR_GREEN_END                   111

#define COLOR_BLUE_START                  144
#define COLOR_BLUE_END                    159

#define COLOR_WHITE_START                 16
#define COLOR_WHITE_END                   31

// indices used to access data arrays in BOBs 
#define INDEX_WORLD_X                     8
#define INDEX_WORLD_Y                     9

// defines for the states of the main loop
#define GAME_STATE_INIT                  0
#define GAME_STATE_MENU                  1
#define GAME_STATE_RESTART               2
#define GAME_STATE_RUNNING               3
#define GAME_STATE_UPLINK                4
#define GAME_STATE_EXIT                  5
#define GAME_STATE_WAITING_FOR_EXIT      6
#define GAME_STATE_PAUSED                7

// the menu defines
#define MENU_STATE_MAIN                  0   // main menu state 
#define MENU_STATE_INST                  1   // instructions state
#define MENU_SEL_NEW_GAME                0
#define MENU_SEL_INSTRUCTIONS            1
#define MENU_SEL_EXIT                    2
#define MAX_INSTRUCTION_PAGES            6  

// defines for font
#define FONT_NUM_CHARS         96  // entire character set
#define FONT_WIDTH             12
#define FONT_HEIGHT            12
#define FONT_WIDTH_NEXT_NUM    8
#define FONT_WIDTH_NEXT_LOWER  7
#define FONT_WIDTH_NEXT_UPPER  8

// number of starting objects
#define NUM_ACTIVE_GUNSHIPS          8
#define NUM_ACTIVE_MINES             8
#define NUM_ACTIVE_STATIONS          8


// PROTOTYPES /////////////////////////////////////////////

// game console
int  Game_Init(void *parms=NULL);
int  Game_Shutdown(void *parms=NULL);
int  Game_Main(void *parms=NULL);
void Game_Reset(void);

// helper functions for game logic
void Draw_Info(void);

void Draw_Powerups(void);
void Start_Powerups(int type, int x, int y);
void Move_Powerups(void);
void Delete_Powerups(void);
void Init_Powerups(void);
void Reset_Powerups(void);


void Init_Gunships(void);
void Move_Gunships(void);
void Draw_Gunships(void);
void Start_Gunship(int override, int x, int y, int dir);
void Reset_Gunships(void);

void Fire_Plasma(int x,int y, int xv, int yv, int source);
void Draw_Plasma(void);
void Move_Plasma(void);
void Delete_Plasma(void);
void Init_Plasma(void);
void Reset_Plasma(void);

void Init_Stations(void);
void Move_Stations(void);
void Draw_Stations(void);
void Start_Station(int override, int x, int y);
void Reset_Stations(void);

void Create_Tables(void);
float Fast_Distance_2D(float x, float y);
void Seed_Rocks(void);
int Load_Player(void);
void Draw_Scanner(void);
int Pad_Name(char *filename, char *extension, char *padstring, int num);

int Copy_Screen(UCHAR *source_bitmap,UCHAR *dest_buffer, int lpitch, int transparent);

void Load_Buttons(void);
void Load_HUD(void);


inline void Copy_Palette(LPPALETTEENTRY dest, LPPALETTEENTRY source)
{ memcpy(dest, source, 256*sizeof(PALETTEENTRY)); }


// TYPES //////////////////////////////////////////////////



// MACROS ////////////////////////////////////////////////

// used to compute the min and max of two expresions
#define MIN(a, b)  (((a) < (b)) ? (a) : (b)) 
#define MAX(a, b)  (((a) > (b)) ? (b) : (a)) 

// returns a rand in a range
#define RAND_RANGE(a,b) ((a)+(rand()%((b)-(a)+1)))

// GLOBALS ////////////////////////////////////////////////

HWND main_window_handle = NULL; // save the window handle
HINSTANCE main_instance = NULL; // save the instance
char buffer[80];                // used to print text

BITMAP_IMAGE background_bmp, menu_bmp; // used to hold backgrounds
BITMAP_IMAGE menu_sel_bmps[3];   // holds the menu selections
BITMAP_IMAGE andre;

BOB wraith;                   // the player 
BOB powerups[MAX_POWERUPS];   // the powerups
BOB hud;                      // the art for the scanner hud

// player state variables

int player_ships       = 3;   // ships left
int player_regen_counter = 0; // used to track when to Regen
int player_ammo        = 1000;// ammo of player
int player_counter     = 0;   // used for state transition tracking
int player_regen_count = 0;   // used to regenerate player
int ready_counter = 0,        // used to draw a little "get ready"
    ready_state   = 0;


int win_counter = 0,          // tracks if player won
    player_won = 0;   

int intro_done = 0;           // flags if intro has played already

int game_paused      = 0,
    pause_debounce   = 0,
    huds_on          = 1,
    scanner_on       = 1,
    huds_debounce    = 0,
    scanner_debounce = 0;

// color palettes, so we don't have to reload all the time
PALETTEENTRY         game_palette[256]; // holds the main game palette
PALETTEENTRY         menu_palette[256]; // gee what do you think
PALETTEENTRY         andre_palette[256]; // for me

// positional and state info for player
float player_x=0,
      player_y=0,
	  player_dx=0, 
      player_dy=0, 
	  player_xv=0, 
	  player_yv=0,
      vel = 0;

// these contain the virual cos, sin lookup tables for the 16 sector circle
float cos_look16[16],
      sin_look16[16];

// sound id's
int intro_music_id  = -1,
    main_music_id   = -1,
    ready_id        = -1,
    engines_id      = -1,
    scream_id       = -1,
    game_over_id    = -1,
    mine_powerup_id = -1,
    deactivate_id   = -1,
    main_menu_id    = -1,
    empty_id        = -1,
    beep0_id             = -1,
    beep1_id             = -1,
    station_blow_id      = -1,
    station_throb_id     = -1,
    ammopu_id            = -1,
    shldpu_id            = -1;

int expl_ids[MAX_EXPL_SOUNDS] = {-1,-1,-1,-1,-1,-1,-1,-1};
int fire_ids[MAX_FIRE_SOUNDS] = {-1,-1,-1,-1,-1,-1,-1,-1};
int game_state                = GAME_STATE_INIT;   // initial game state

// FUNCTIONS //////////////////////////////////////////////

LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
// this is the main message handler of the system
PAINTSTRUCT	ps;		   // used in WM_PAINT
HDC			hdc;	   // handle to a device context

// what is the message 
switch(msg)
	{	
	case WM_CREATE: 
        {
		// do initialization stuff here


		return(0);
		} break;

    case WM_PAINT:
         {
         // start painting
         hdc = BeginPaint(hwnd,&ps);

         // end painting
         EndPaint(hwnd,&ps);
         return(0);
        } break;


	case WM_DESTROY: 
		{

		// kill the application			
		PostQuitMessage(0);
		return(0);
		} break;

	default:break;

    } // end switch

// process any messages that we didn't take care of 
return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc

// WINMAIN ////////////////////////////////////////////////

int WINAPI WinMain(	HINSTANCE hinstance,
					HINSTANCE hprevinstance,
					LPSTR lpcmdline,
					int ncmdshow)
{
// this is the winmain function

WNDCLASS winclass;	// this will hold the class we create
HWND	 hwnd;		// generic window handle
MSG		 msg;		// generic message
HDC      hdc;       // generic dc
PAINTSTRUCT ps;     // generic paintstruct

// first fill in the window class stucture
winclass.style			= CS_DBLCLKS | CS_OWNDC | 
                          CS_HREDRAW | CS_VREDRAW;
winclass.lpfnWndProc	= WindowProc;
winclass.cbClsExtra		= 0;
winclass.cbWndExtra		= 0;
winclass.hInstance		= hinstance;
winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
winclass.hCursor		= LoadCursor(NULL, IDC_ARROW);
winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
winclass.lpszMenuName	= NULL; 
winclass.lpszClassName	= WINDOW_CLASS_NAME;

// register the window class
if (!RegisterClass(&winclass))
	return(0);

// create the window, note the use of WS_POPUP
if (!(hwnd = CreateWindow(WINDOW_CLASS_NAME, // class
						  "Vector tracking demo",	 // title
						  WS_POPUP | WS_VISIBLE,
					 	  0,0,	   // x,y
						  WINDOW_WIDTH,  // width
                          WINDOW_HEIGHT, // height
						  NULL,	   // handle to parent 
						  NULL,	   // handle to menu
						  hinstance,// instance
						  NULL)))	// creation parms
return(0);

// save the window handle and instance in a global
main_window_handle = hwnd;
main_instance      = hinstance;

// perform all game console specific initialization
Game_Init();

// enter main event loop
while(1)
	{
	if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{ 
		// test if this is a quit
        if (msg.message == WM_QUIT)
           break;
	
		// translate any accelerator keys
		TranslateMessage(&msg);

		// send the message to the window proc
		DispatchMessage(&msg);
		} // end if
    
    // main game processing goes here
    Game_Main();

	} // end while


// shutdown game and release all resources
Game_Shutdown();

// return to Windows like this
return(msg.wParam);

} // end WinMain

////////////////////////////////////////////////////////////////////////////

void Load_Andre(void)
{
// this function loads all the buttons for the interface

// load image
Load_Bitmap_File(&bitmap8bit, "OUTART/ENEMYAI.DAT");

// save the palette
Copy_Palette(andre_palette, bitmap8bit.palette);

Create_Bitmap(&andre,320-216/2,240-166/2,216,166);
Load_Image_Bitmap(&andre,&bitmap8bit,0,0,BITMAP_EXTRACT_MODE_ABS);

// unload object bitmap
Unload_Bitmap_File(&bitmap8bit);

} // end Load_Andre


void Game_Reset(void)
{
// this function resets all the game related systems and vars for another run

int index; // looping var

// player state variables
player_state       = PLAYER_STATE_ALIVE;
player_score       = 0;   // the score
player_ships       = 3;   // ships left
player_regen_counter = 0; // used to track when to regen
player_damage      = 0;   // damage of player
player_counter     = 0;   // used for state transition tracking
player_ammo        = 1000; // ammo
player_regen_count = 0;   // used to regenerate player
player_shield_count = 0;  // used to display shields
win_counter = 0;
player_won = 0;   
num_stations_destroyed = 0; // number of stations destroyed thus far

// menu_up = 0;             // flags if the main menu is up
// intro_done = 0;          // flags if intro has played already

// positional and state info for player
player_x=0;
player_y=0;
player_dx=0;
player_dy=0; 
player_xv=0; 
player_yv=0;
vel = 0;
wraith.varsI[WRAITH_INDEX_DIR] = 0;
game_state = GAME_STATE_RESTART;   // initial game state
station_id  = -1;

ready_counter = 0,        // used to draw a little "get ready"
ready_state   = 0;

// call all the reset funcs
Init_Reset_Particles();
Reset_Gunships();
Reset_Mines();
Reset_Rocks();
Reset_Plasma();
Reset_Bursts();
Reset_Stations();
Reset_Powerups();

// start the rocks
Seed_Rocks();


// start the gunships
for (index=0; index < NUM_ACTIVE_GUNSHIPS; index++)
    Start_Gunship();

// start the mines
for (index=0; index < NUM_ACTIVE_MINES; index++)
    Start_Mine();

// start the stations
for (index=0; index < NUM_ACTIVE_STATIONS; index++)
    Start_Station();
 
} // end Game_Reset

////////////////////////////////////////////////////////////

int Game_Init(void *parms)
{
// this function is where you do all the initialization 
// for your game

int index;         // looping var
char filename[80]; // used to build up files names

//mono.clear();
//mono.print("\ndebugger on-line\n");

// seed random number generate
srand(Start_Clock());

// create all lookup tables
Create_Tables();

// initialize direct draw
DDraw_Init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);

// initialize direct sound
DSound_Init();

// initialize Direct input
DInput_Init();

// initialize all input devices
DInput_Init_Keyboard();
 
// load all the sound and music
Load_Sound_Music();

// initialize the plasma pulses
Init_Plasma();

// initialize all the stars
Init_Stars();

// init rocks
Init_Rocks();
Seed_Rocks();

// init all the explosions
Init_Bursts();

// initialize all the Mines
Init_Mines();

// initialize all the gunships
Init_Gunships();

// initialize all the stations, must be after mines!
Init_Stations();

// initialize the powerups
Init_Powerups();

// initialize particle system
Init_Reset_Particles();

Load_Andre();

// set clipping rectangle to screen extents so objects don't
// mess up at edges
RECT screen_rect = {0,0,screen_width,screen_height};
lpddclipper = DDraw_Attach_Clipper(lpddsback,1,&screen_rect);

// hide the mouse
ShowCursor(FALSE);

// start the intro music
DSound_Play(intro_music_id);

// do the introduction
Do_Intro();

// load the player
Load_Player();

// load the hud art
Load_HUD();

// return success
return(1);

} // end Game_Init

///////////////////////////////////////////////////////////

int Game_Shutdown(void *parms)
{
// this function is where you shutdown your game and
// release all resources that you allocated

// delete all the explosions
Delete_Bursts();
    
// delete the player
Destroy_BOB(&wraith);

// delete all the rocks
Delete_Rocks();

// delete all the plasma pulses
Delete_Plasma();

// delete all the stations
Delete_Stations();

// delete the mines
Delete_Mines();

// shutdown directdraw
DDraw_Shutdown();

// unload sounds
DSound_Delete_All_Sounds();

// shutdown directsound
DSound_Shutdown();

// release all input devices
DInput_Release_Keyboard();

// return success
return(1);
} // end Game_Shutdown

///////////////////////////////////////////////////////////

int Game_Main(void *parms)
{
// this is the workhorse of your game it will be called
// continuously in real-time this is like main() in C
// all the calls for you game go here!

int  index;                  // looping var
int  dx,dy;                  // general deltas used in collision detection
float xv=0, yv=0;            // used to compute velocity 
static int engines_on = 0;   // tracks state of engines each frame 
static int andre_up = 0;


// what state is the main loop in?
if (game_state == GAME_STATE_INIT)
{
// reset everything

// transition into menu state
// game_state = GAME_STATE_MENU;

// transition into run state
game_state = GAME_STATE_RESTART;
} // end game_state is game init
else
if (game_state == GAME_STATE_MENU)
{

game_state = GAME_STATE_RESTART;

} // end if in menu state
else 
if (game_state == GAME_STATE_RESTART)
{
// start the main music
DSound_Play(main_music_id, DSBPLAY_LOOPING);

// restart the game
Game_Reset();

// set to run state
game_state = GAME_STATE_RUNNING;


} // end if restart
else
if (game_state == GAME_STATE_RUNNING)
{

// start the timing clock
Start_Clock();

// clear the drawing surface
DDraw_Fill_Surface(lpddsback, 0);

// read all input devices
DInput_Read_Keyboard();

// only process player if alive
if (player_state == PLAYER_STATE_ALIVE)
{

// test for tracking rate change
if (keyboard_state[DIK_EQUALS])
   {
    if ((mine_tracking_rate+=0.1) > 4.0)
       mine_tracking_rate = 4.0;
   } // end if
else
if  (keyboard_state[DIK_MINUS])
    {
    if ((mine_tracking_rate-=0.1) < 0)
       mine_tracking_rate = 0;
    } // end if


// test if player is moving
if (keyboard_state[DIK_RIGHT])
    {
    // rotate player to right
    if (++wraith.varsI[WRAITH_INDEX_DIR] > 15)
		wraith.varsI[WRAITH_INDEX_DIR] = 0;
	
    } // end if
else
if (keyboard_state[DIK_LEFT])
    {
    // rotate player to left
    if (--wraith.varsI[WRAITH_INDEX_DIR] <  0)
	 	wraith.varsI[WRAITH_INDEX_DIR] = 15;

    } // end if

// vertical/speed motion
if (keyboard_state[DIK_UP])
    {
    // move player forward
    xv = cos_look16[wraith.varsI[WRAITH_INDEX_DIR]];
	yv = sin_look16[wraith.varsI[WRAITH_INDEX_DIR]];

    // test to turn on engines
    if (!engines_on)
       DSound_Play(engines_id,DSBPLAY_LOOPING);

	// set engines to on
	engines_on = 1;

    Start_Particle(PARTICLE_TYPE_FADE, PARTICLE_COLOR_GREEN, 3, 
                   player_x+RAND_RANGE(-2,2), player_y+RAND_RANGE(-2,2), (-int(player_xv)>>3), (-int(player_yv)>>3));
    
    } // end if
else
if (engines_on)
   {
   // reset the engine on flag and turn off sound
   engines_on = 0;

   // turn off the sound
 
   DSound_Stop_Sound(engines_id);
   } // end if



// toggle huds
if (keyboard_state[DIK_H] && !huds_debounce)
   {
   huds_debounce = 1;
   huds_on = (huds_on) ? 0 : 1;

   DSound_Play(beep1_id);

   } // end if

if (!keyboard_state[DIK_H])
   huds_debounce = 0;

// toggle scanner
if (keyboard_state[DIK_S] && !scanner_debounce)
   {
   scanner_debounce = 1;
   scanner_on = (scanner_on) ? 0 : 1;

   DSound_Play(beep1_id);

   } // end if

if (!keyboard_state[DIK_S])
   scanner_debounce = 0;



// add velocity change to player's velocity
player_xv+=xv;
player_yv+=yv;

// test for maximum velocity
vel = Fast_Distance_2D(player_xv, player_yv);

if (vel >= MAX_PLAYER_SPEED)
   {
   // re-compute velocity vector by normalizing then re-scaling
   player_xv = (MAX_PLAYER_SPEED-1)*player_xv/vel;
   player_yv = (MAX_PLAYER_SPEED-1)*player_yv/vel;
   } // end if

// add in frictional coefficient

// move player, note that these are in world coords
player_x+=player_xv;
player_y+=player_yv;

// wrap player to universe
if (player_x > UNIVERSE_MAX_X)
   {
   // reset pos
   player_x = UNIVERSE_MIN_X;

   // set warp flag
   } // end if
else
if (player_x < UNIVERSE_MIN_X)
   {
   // reset pos
   player_x = UNIVERSE_MAX_X;

   // set warp flag
   } // end if

// y coords
if (player_y > UNIVERSE_MAX_Y)
   {
   // reset pos
   player_y = UNIVERSE_MIN_Y;

   // set warp flag
   } // end if
else
if (player_y < UNIVERSE_MIN_Y)
   {
   // reset pos
   player_y = UNIVERSE_MAX_Y;

   // set warp flag
   } // end if


// test if player is firing

if ((keyboard_state[DIK_LCONTROL] || keyboard_state[DIK_SPACE]))
	{
    if (player_ammo > 0)
       {
    	// compute plasma velocity vector
        float plasma_xv = cos_look16[wraith.varsI[WRAITH_INDEX_DIR]]*PLASMA_SPEED;
	    float plasma_yv = sin_look16[wraith.varsI[WRAITH_INDEX_DIR]]*PLASMA_SPEED;
 
	    // fire the plasma taking into consideration ships's velocity
        Fire_Plasma(player_x,player_y,
		            player_xv+plasma_xv,
				    player_yv+plasma_yv, PLASMA_ANIM_PLAYER);

        // use up some ammo
        if (player_ammo > 0)
        player_ammo--;

	    } // end if
    else
        {
        DSound_Play(empty_id);
        } // end else

    } // end if

// do bounds check


// regenerate player
if (++player_regen_counter >= PLAYER_REGEN_COUNT)
    {
    // regenerate the player a bit
    if (player_damage >0 )
        player_damage--;

    // reset counter
    player_regen_counter = 0;

    } // end if


// test for dying state transition
if (player_damage >= 100 && player_state == PLAYER_STATE_ALIVE)
    {
    // set damage to 100
    player_damage = 100;

    // kill player
    player_state = PLAYER_STATE_DYING;
    player_ships--;

    // set counter to 0
    player_counter = 0;

    // turn engines off
	engines_on = 0;

    // start scream
    DSound_Play(scream_id);
    
    } // end if


} // end if player alive
else
if (player_state == PLAYER_STATE_DYING)
   { 
   // player is dying

   // start random bursts
   int bwidth  = 16 + rand()%64;
   
   if ((rand()%4)==1)
        Start_Burst(player_x+rand()%40, player_y+rand()%8,
                   bwidth, (bwidth >> 2) + (bwidth >> 1),
                   -4+rand()%8,2+rand()%4);    

   // get jiggie with it
   wraith.varsI[WRAITH_INDEX_DIR] += (RAND_RANGE(-1,1));

   if (wraith.varsI[WRAITH_INDEX_DIR] > 15)
       wraith.varsI[WRAITH_INDEX_DIR] = 0;
   else
   if (wraith.varsI[WRAITH_INDEX_DIR] < 0)
      wraith.varsI[WRAITH_INDEX_DIR] = 15;

   // update state counter
   if (++player_counter > 150)
      {
      // set state to invincible  
      player_state = PLAYER_STATE_INVINCIBLE;

      // reset counter
      player_counter = 0;

      // reset damage
      player_damage = 0;
      } // end if


   } // end if
else
if (player_state == PLAYER_STATE_INVINCIBLE)
   {
   // player is invincible
 
   // flicker shields and play energizing sound

   // update state counter
   if (++player_counter > 70)
      {
      // set state to invincible  
      player_state = PLAYER_STATE_ALIVE;

      // reset counter
      player_counter = 0;

      // reset player position
      player_x = 0;
      player_y = 0;
      player_dx=0;
      player_dy=0; 
      player_xv=0; 
      player_yv=0;
      wraith.varsI[WRAITH_INDEX_DIR] = 0;

      // decrease player ships
      if (player_ships == 0)
         {
         // change state to dead, reset all vars
         player_state   = PLAYER_STATE_DEAD;
         player_counter = 0;
         ready_state    = 1;
         ready_counter  = 0;

         // turn engines
         DSound_Stop_Sound(engines_id);

         // play game over sound
         DSound_Play(game_over_id);

         } // end if       

      } // end if

    // start ready again
    if ((player_counter == 20) && (player_ships > 0))
      {
      // reset to ready
      ready_counter = 0, 
      ready_state   = 0;
      } // end if
   
   } // end if

// end player sub states

// move the powerups 
Move_Powerups();

// move the stations
Move_Stations();

// move the gunships
Move_Gunships();

// move the mines
Move_Mines();

// move the asteroids
Move_Rocks();

// move the stars
Move_Stars();

// move the particles
Move_Particles();

// move the explosions
Move_Bursts();

// draw the stars
Draw_Stars();

// draw the powerups
Draw_Powerups();

// draw the stations
Draw_Stations();

// draw the gunships
Draw_Gunships();

// draw the rocks
Draw_Rocks();

// draw the mines
Draw_Mines();

// draw the particles
Draw_Particles();

// draw the plasma
Draw_Plasma();

// move the plasma
Move_Plasma();

// draw the player if alive
if (player_state == PLAYER_STATE_ALIVE || player_state == PLAYER_STATE_DYING)
	{	
	// set the current frame
    if (engines_on)
		wraith.curr_frame = wraith.varsI[WRAITH_INDEX_DIR] + 16*(rand()%2);
	else
		wraith.curr_frame = wraith.varsI[WRAITH_INDEX_DIR];

	// draw the bob
    Draw_BOB(&wraith,lpddsback);

    // draw the shields
    if (player_shield_count > 0)
       {
       player_shield_count--;

       // select shield frame
       wraith.curr_frame = 34-player_shield_count;    
	   
       // draw the bob
       Draw_BOB(&wraith,lpddsback);

       } // end if

	} // end if

// draw explosions last
Draw_Bursts();

// draw the score and ships left
if (huds_on)
    Draw_Info();

if (scanner_on)
    Draw_Scanner();

// last state here so it will overlay
if (player_state == PLAYER_STATE_DEAD)
   {
   // player is dead
   ready_state   = 1;
   ready_counter = 0;

   // player is done!
   Draw_Text_GDI("G A M E   O V E R",(SCREEN_WIDTH/2) - 8*(strlen("G A M E   O V E R")>>1),SCREEN_HEIGHT/2,RGB(0,255,0),lpddsback);

   } // end if

// draw get ready?
if (ready_state==0)
   {
   // test if counter is 10 for voice
   if (ready_counter==10)
       DSound_Play(ready_id);

   // draw text
   Draw_Text_GDI("E N T E R I N G   S E C T O R - ALPHA 11",320-8*strlen("E N T E R I N G   S E C T O R!")/2, 200,RGB(0,rand()%255,0),lpddsback);

   // increment counter
   if (++ready_counter > 60)
      {
      // set state to ready
      ready_state = 1;
      ready_counter = 0;

      } // end if

   } // end if

// test for me!
if (keyboard_state[DIK_LALT] && keyboard_state[DIK_RALT] && keyboard_state[DIK_A])
   {
   // lock primary
   DDraw_Lock_Back_Surface();   
   Draw_Bitmap(&andre,back_buffer, back_lpitch,0);        
   // release the primary buffer
   DDraw_Unlock_Back_Surface();   
   andre_up = 1;
   } // end if
else 
   {
   andre_up = 0;
   Set_Palette(game_palette);
   } // end else

if (andre_up == 1)
   {
   Set_Palette(andre_palette);
   andre_up = 2;
   } // end if


// flip the surfaces
DDraw_Flip();

// sync to 30ish fps
Wait_Clock(30);

// check of user is trying to exit
if (++player_counter > 10)
if (keyboard_state[DIK_ESCAPE])
   {
   // send game back to menu state
   game_state = GAME_STATE_EXIT;

   // stop all sounds
   DSound_Stop_Sound(engines_id);

   } // end if


} // end if game running
else
if (game_state == GAME_STATE_PAUSED)
   {
    // pause game
    if (keyboard_state[DIK_P] && !pause_debounce)
       {
       pause_debounce = 1;
       game_paused = (game_paused) ? 0 : 1;

       DSound_Play(beep1_id);

       } // end if

    if (!keyboard_state[DIK_P])
       game_paused = 0;

    if (game_paused)
       {
       // draw text
       Draw_Text_GDI("G A M E  P A U S E D  -- P R E S S  <P>",320-8*strlen("G A M E  P A U S E D  -- P R E S S  <P>")/2, 200, RGB(255,0,0),lpddsback);
       } // end if
    else
       game_state = GAME_STATE_RUNNING;


   } // end if
else
if (game_state == GAME_STATE_EXIT)
   {
   // this is the exit state, called just once
   PostMessage(main_window_handle, WM_DESTROY,0,0);
   game_state = GAME_STATE_WAITING_FOR_EXIT;

   // fade to black
   Screen_Transitions(SCREEN_DARKNESS, NULL, 0);

   // clear out buffers
   DDraw_Fill_Surface(lpddsback, 0);
   DDraw_Fill_Surface(lpddsprimary, 0);

   } // end if
else
if (game_state == GAME_STATE_WAITING_FOR_EXIT)
   {
   // wait here in safe state

   } // end wait

// return success
return(1);

} // end Game_Main

