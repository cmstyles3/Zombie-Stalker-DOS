// I N C L U D E S //////////////////////////////////////////////////////////
#include "pcx.h"
#include "keyboard.h"
#include "sound.h"
#include <fstream.h>
#include <dir.h>

// D E F I N E S ////////////////////////////////////////////////////////////
#define PLAYGAME       '1'
#define INSTRUCTIONS   '2'
#define SET_DIFFICULTY '3'
#define EXIT           '4'

#define MAX_BULLETS 6  //the max amount of bullets on the screen

#define MAX_ZOMBIES 20 //the max amount of zombies on the screen

#define CELL_ROWS 10 //number of cell rows on the screen
#define CELL_COLS 20 //number of cell columns on the screen

#define WORLD_ROWS 6 //number of screen rows in the world matrix
#define WORLD_COLS 4 //number of screen column in the world matrix

//states for the zombie
#define ZOMBIE_DEAD 0
#define ZOMBIE_STILL 1
#define ZOMBIE_CHASE 2
#define ZOMBIE_EVADE 3
#define ZOMBIE_DEFAULT_RANDOM 4
#define ZOMBIE_DYER_RANDOM 5
#define ZOMBIE_DYING  6

//defines for the game objects
#define GREY_WALL 0
#define BLUE_WALL 1
#define BUSH 2
#define TILE_FLOOR 3
#define TILE_HEALTH 4
#define TILE_AMMO 5
#define GRASS 6
#define GRASS_HEALTH 7
#define GRASS_AMMO 8
#define HEALTH_GENERATOR 9
#define TILE_KEY 10
#define GRASS_KEY 11
#define DOOR 12
#define END 15
#define START 16
#define FAKEWALL 17

//defines for the game sound effects
#define PLAYER_DEAD 0
#define SHOT 1
#define DOOR_OPEN 2
#define HEALTH 3
#define AMMO 4
#define ZOMBIE_HIT 5


// E N U M //////////////////////////////////////////////////////////////////
enum bool{false,true}; //boolean true,false

enum switch_on_off{off,on}; //used to turn on or off


// S T R U C T U R E S //////////////////////////////////////////////////////

//structure used get the pos of zombies
typedef struct zombie_pos_typ
{
	bool state; //state of zombie_pos true,false
	int x,y; //x,y of zombie_pos
}zombie_pos, *zombie_ptr;

//structure used to store data for each screen
typedef struct screen_typ
 {
	int cells[CELL_ROWS][CELL_COLS]; //cells for each screen
	int num_zombies; //number of zombies present on each screen
	switch_on_off snowing; //used to store snowing on or off for each screen
	zombie_pos positions[MAX_ZOMBIES]; //position for each zombie on the screen
}screen, *screen_ptr;

//structure for bullets data
struct bullet_typ
{
	int x,y; //x,y coorinate of bullets
	int xv,yv; //x,y velocity of bullets
	bool state; //state of bullets true or false
	sprite object; //sprite for each bullet
}bullets[MAX_BULLETS];


typedef struct explode_typ
{
	int x,y;
	int xv,yv;
	int color,back;
	bool state;
	int counter,threshold;
}explode;
//structure for the zombies
struct zombies_typ
{
	int x,y; //x,y coorinates of zombies
	int xv,yv; //x velocity and y velocity of zombies
	int state; //state DEAD,DYING,CHASING ......
	int lives;
	int direction; //direction of zombie
	int counter,threshold; //used to time when to change state of zombie
	explode zexplode[40];
	sprite object; //sprite for the zombie
}zombies[MAX_ZOMBIES],boss;

//structure for the snow
struct
{
	int x,y; //x,y coorinates of the snow
	int xv,yv; //x,y velocity of snow
	int color; //the color of snow;white,light grey
	int back; //used to store background color
	int stop; //when snow flake has hit the ground
	bool state;
}snow[40];

struct score_type
{
	unsigned long score;
	char name[10];
}scorelist[10];

// P R O T O T Y P E S /////////////////////////////////////////////////////
void Intro();

void load_screens(char *);

void draw_screen(int,int);

void load_sprites();

void load_sounds();

void instructions();

char menu();

void init_zombies();

void erase_zombies();

void under_zombies();

void draw_zombies();

int start_zombie(int,int);

void start_zombies(int,int);

void init_bullets();

void erase_bullets();

void under_bullets();

void draw_bullets();

void shoot_bullets(int,int,int,int,int);

void init_snow();

void erase_snow();

void under_snow();

void draw_snow();

void move_snow();

void show_game_status();

int playgame(char *,char *);

void init_explode(int);

void erase_explode();

void under_explode();

void draw_explode();

void animate_explode();

void init_boss(int,int);

void erase_boss();

void under_boss();

void move_boss();

int draw_boss();

void load_screens_boss(char *);

void load_sprites_boss();

int play_game_boss(char *,char *);

void loadscores();

void newscore(unsigned long);

void showscores();

// G L O B A L S ////////////////////////////////////////////////////////////
pcx_picture imagery,weapons,mmenu,background,intro,instruct;//GRAPHICS FOR GAME

sprite player,wall1,wall2,bush,floors,door,pause_pic; //SPRITES FOR THE GAME
sprite endtile,starttile;

int bullet_vel_x[4]={8,-8,0,0}; //array for the bullets x and y velocities
int bullet_vel_y[4]={0,0,-8,8};

unsigned long player_score;
int player_dir;
int ammo,keys,health;

screen world[WORLD_ROWS][WORLD_COLS]; //holds all the screens in the world

int screen_x,screen_y,old_screen_x,old_screen_y,screen_change; //used to keep track of the current screen

_sound soundfx[6];
music song;
int mload;
int sload[6];

int difficulty;
int currsnd=6;

char *scorefile = "scores.txt";

//=========================---> M A I N <---=================================
void main()
{
	Screen_Transition(SCREEN_DARKNESS);
	/*if (system("loadhigh soundrv.com")!=0)
	{
		system(searchpath("loadhigh soundrv.com"));
		system("loadhigh soundrv.com");
	}
	if (system("loadhigh midpak.com")!=0)
	{
		system(searchpath("loadhigh midpak.com"));
		system("loadhigh midpak.com");
	} */
	randomize();
	int playstate;
	difficulty=1;
	int index;
	int vol;
	char choice=0;
	Set_Graphics_Mode(GRAPHMODE); //set video mode to 320x200x256 (mode 13h || mode 19)
	PCX_Init((pcx_picture_ptr)&intro); //ALLOCATE MEMORY FOR PICTURE
	/*mload=Music_Load("starwars.xmi",(music_ptr)&song);
	if (mload) Music_Play((music_ptr)&song,0);*/
	Time_Delay(30);
	PCX_Load("INTRO.pcx",(pcx_picture_ptr)&intro,1); //Load the picture
	PCX_Show_Buffer((pcx_picture_ptr)&intro); //show the picture
	Time_Delay(100);
	Screen_Transition(SCREEN_DISOLVE); //dissolve screen
	Fill_Screen(0);
	PCX_Load("title.pcx",(pcx_picture_ptr)&intro,1);
	PCX_Show_Buffer((pcx_picture_ptr)&intro);
	Time_Delay(105);
	Screen_Transition(SCREEN_DARKNESS); //fade to dark
	PCX_Delete((pcx_picture_ptr)&intro); //DEALLOCATE MEMORY FOR PICTURE
	Fill_Screen(0); //clear screen with black
	do
	{
	//MAIN MENU USED FOR THE GAME
		choice=menu();
		switch(choice)
		{
			case PLAYGAME:
				if (mload)
				{
					Music_Stop();
					Music_Unload((music_ptr)&song);
				}
				playstate=2;
				health=100;ammo=300;
				player_score=0;
				PCX_Init((pcx_picture_ptr)&intro);
				PCX_Load("story.pcx",(pcx_picture_ptr)&intro,1);
				PCX_Show_Buffer((pcx_picture_ptr)&intro);
				PCX_Delete((pcx_picture_ptr)&intro);
				while (kbhit()) getch();
				getch();
				Screen_Transition(SCREEN_DARKNESS);
                    Fill_Screen(0);
				do
				{
					if (playstate==1) {health=100;ammo=100;player_score/=10;player_score*=5;}
					if (playstate) playstate=playgame("level1.txt","jf.xmi");
				}
				while (playstate==1);
				do
				{
					if (playstate==1) {health=100;ammo=100;player_score/=10;player_score*=5;}
					if (playstate) playstate=playgame("level2.txt","msi.xmi");
				}
				while (playstate==1);
				do
				{
					if (playstate==1) {health=100;ammo=100;player_score/=10;player_score*=5;}
					if (playstate) playstate=playgame("level3.txt","mib.xmi");
				}
				while (playstate==1);
				do
				{
					if (playstate==1) {health=100;ammo=100;player_score/=10;player_score*=5;}
					if (playstate) playstate=playgame("level4.txt","avt.xmi");
				}
				while (playstate==1);
				do
				{
					if (playstate==1) {health=100;ammo=100;player_score/=10;player_score*=5;}
					if (playstate) playstate=playgame("level5.txt","jb.xmi");
				}
				while (playstate==1);
				do
				{
					if (playstate==1) {health=100;ammo=100;player_score/=10;player_score*=5;}
					if (playstate) playstate=playgame("level6.txt","jnbs.xmi");
				}
				while (playstate==1);
				do
				{
					if (playstate==1) {health=100;ammo=100;player_score/=10;player_score*=5;}
					if (playstate) playstate=play_game_boss("levelB.txt","matt2.xmi");
				}
				while (playstate==1);
				mload=Music_Load("matt.xmi",(music_ptr)&song);
				if (mload)
				{
					Music_Stop();
					Music_Play((music_ptr)&song,0);
				}
				break;
			case INSTRUCTIONS:
				instructions();
				break;
			case SET_DIFFICULTY:
				Set_Graphics_Mode(TEXTMODE);
				textmode(C40);
				char *choice[4] = {"1    Mommy, this game looks fun. ",
								   "2    I think I have a shot.      ",
								   "3    Want a peace of me.         ",
								   "4    Don't worry, it's me.       "};
				int key=0;
				difficulty=1;
				clrscr();
				textcolor(0);
				textbackground(4);
				gotoxy(1,1);
				cprintf("%s",choice[0]);
				textcolor(4);
				textbackground(0);
				gotoxy(1,2);
				cprintf("%s",choice[1]);
				gotoxy(1,3);
				cprintf("%s",choice[2]);
				gotoxy(1,4);
				cprintf("%s",choice[3]);
				gotoxy(1,1);
				do
				{
					key=getch();
					switch (key)
					{
					 case 72:
						gotoxy(1,difficulty);
						textcolor(4);
						textbackground(0);
						cprintf("%s",choice[difficulty-1]);
						difficulty--;
						if (difficulty<1) difficulty=4;
						gotoxy(1,difficulty);
						textcolor(0);
						textbackground(4);
						cprintf("%s",choice[difficulty-1]);
						gotoxy(1,difficulty);
						break;
					 case 80:
						gotoxy(1,difficulty);
						textcolor(4);
						textbackground(0);
						cprintf("%s",choice[difficulty-1]);
						difficulty++;
						if (difficulty>4) difficulty=1;
						gotoxy(1,difficulty);
						textcolor(0);
						textbackground(4);
						cprintf("%s",choice[difficulty-1]);
						gotoxy(1,difficulty);
						break;
					 case '1':
						gotoxy(1,difficulty);
						textcolor(4);
						textbackground(0);
						cprintf("%s",choice[difficulty-1]);
						difficulty=1;
						gotoxy(1,difficulty);
						textcolor(0);
						textbackground(4);
						cprintf("%s",choice[0]);
						gotoxy(1,difficulty);
						break;
					 case '2':
						gotoxy(1,difficulty);
						textcolor(4);
						textbackground(0);
						cprintf("%s",choice[difficulty-1]);
						difficulty=2;
						gotoxy(1,difficulty);
						textcolor(0);
						textbackground(4);
						cprintf("%s",choice[1]);
						gotoxy(1,difficulty);
						break;
					 case '3':
						gotoxy(1,difficulty);
						textcolor(4);
						textbackground(0);
						cprintf("%s",choice[difficulty-1]);
						difficulty=3;
						gotoxy(1,difficulty);
						textcolor(0);
						textbackground(4);
						cprintf("%s",choice[2]);
						gotoxy(1,difficulty);
						break;
					 case '4':
						gotoxy(1,difficulty);
						textcolor(4);
						textbackground(0);
						cprintf("%s",choice[difficulty-1]);
						difficulty=4;
						gotoxy(1,difficulty);
						textcolor(0);
						textbackground(4);
						cprintf("%s",choice[3]);
						gotoxy(1,difficulty);
						break;
					}
				}
				while (key!=13);
				Set_Graphics_Mode(GRAPHMODE);
				break;
			case EXIT:
				if (mload) Music_Stop();
				break;
			default:
				break;
		}
		if(mload&&Music_Status()==2)
		{
			Music_Stop();
			Music_Play((music_ptr)&song,0);
		}
	}while(choice!=EXIT); //if char exit is selected then quit
	Set_Graphics_Mode(TEXTMODE); //set video mode to regular text mode (0x03)

	if (mload) Music_Unload((music_ptr)&song);
}//======================---E N D  O F  M A I N---===========================

void load_screens(char *filename)
{
	ifstream fin(filename);
	int x,y,index_x,index_y,index,temp,row,col;
	int zombie_x,zombie_y;
	for (row=0;row<6;row++)
	{
		for (col=0;col<4;col++)
		{
			world[index_y][index_x].num_zombies=0;
			world[index_y][index_x].positions[index].state=false;
		}
	}
	if(fin.good())
	{
		for (row=0;row<6;row++)
		{
			for (col=0;col<4;col++)
			{
				fin>>temp;
				if (temp==1) world[row][col].snowing=on;
				else world[row][col].snowing=off;
			}
		}
		while(!fin.eof())
		{
			for(index_y=0;index_y<WORLD_ROWS;index_y++)
			{
				for(index_x=0;index_x<WORLD_COLS;index_x++)
				{
					index=0;
					for(y=0;y<CELL_ROWS;y++)
					{
						for(x=0;x<CELL_COLS;x++)
						{
							fin>>world[index_y][index_x].cells[y][x];
							if(world[index_y][index_x].cells[y][x]==START)
							{
								screen_y=index_y;
								screen_x=index_x;
								player.x=x<<4;
								player.y=y<<4;
							}
							if(world[index_y][index_x].cells[y][x]==13)
							{
								zombie_x=x<<4;
								zombie_y=y<<4;
								world[index_y][index_x].positions[index].x=zombie_x;
								world[index_y][index_x].positions[index].y=zombie_y;
								world[index_y][index_x].positions[index].state=true;
								index++;
								world[index_y][index_x].cells[y][x]=TILE_FLOOR;
								world[index_y][index_x].num_zombies++;
							}
							else if(world[index_y][index_x].cells[y][x]==14)
							{
								zombie_x=x<<4;
								zombie_y=y<<4;
								world[index_y][index_x].positions[index].x=zombie_x;
								world[index_y][index_x].positions[index].y=zombie_y;
								world[index_y][index_x].positions[index].state=true;
								index++;
								world[index_y][index_x].cells[y][x]=GRASS;
								world[index_y][index_x].num_zombies++;
							}
						}
					}
				}
			}
		}
		fin.close();
	}
}


//draw the screen for what ever the screen is
void draw_screen(int wx,int wy)
{
	screen_ptr current_screen;
	int index_x,index_y,cell_id;
	SET_SPRITE_SIZE(16,16);
	current_screen=&world[wy][wx];
	for(index_y=0;index_y<CELL_ROWS;index_y++)
	{
		for(index_x=0;index_x<CELL_COLS;index_x++)
		{
			cell_id=current_screen->cells[index_y][index_x];
			if(cell_id==GREY_WALL)
			{
				wall1.x=index_x<<4;
				wall1.y=index_y<<4;
				Sprite_Draw((sprite_ptr)&wall1,double_buffer,1);
			}
			else if(cell_id==BLUE_WALL || cell_id==FAKEWALL)
			{
				wall2.x=index_x<<4;
				wall2.y=index_y<<4;
				Sprite_Draw((sprite_ptr)&wall2,double_buffer,1);
			}
			else if(cell_id==BUSH)
			{
				bush.x=index_x<<4;
				bush.y=index_y<<4;
				Sprite_Draw((sprite_ptr)&bush,double_buffer,1);
			}
			else if(cell_id==DOOR)
			{
				door.x=index_x<<4;
				door.y=index_y<<4;
				Sprite_Draw((sprite_ptr)&door,double_buffer,1);
			}
			else if(cell_id==END)
			{
				endtile.x=index_x<<4;
				endtile.y=index_y<<4;
				Sprite_Draw((sprite_ptr)&endtile,double_buffer,1);
			}
			else if(cell_id==START)
			{
				starttile.x=index_x<<4;
				starttile.y=index_y<<4;
				Sprite_Draw((sprite_ptr)&starttile,double_buffer,1);
			}
			else
			{
				floors.curr_frame=cell_id-3;
				floors.x=index_x<<4;
				floors.y=index_y<<4;
				Sprite_Draw((sprite_ptr)&floors,double_buffer,1);
			}
		}
	}
}
//loads all the sprites used in the game
void load_sprites()
{
	int index,index2;
	PCX_Init((pcx_picture_ptr)&imagery);
	PCX_Load("graphics.pcx",(pcx_picture_ptr)&imagery,1);
	Sprite_Init((sprite_ptr)&wall1,0,0,16,16,0,0,0,0,0,0); //initializes the x,y,height,width,.........
	Sprite_Init((sprite_ptr)&wall2,0,0,16,16,0,0,0,0,0,0);
	Sprite_Init((sprite_ptr)&bush,0,0,16,16,0,0,0,0,0,0);
	Sprite_Init((sprite_ptr)&door,0,0,16,16,0,0,0,0,0,0);
	Sprite_Init((sprite_ptr)&endtile,0,0,16,16,0,0,0,0,0,0);
	Sprite_Init((sprite_ptr)&starttile,0,0,16,16,0,0,0,0,0,0);
	Sprite_Init((sprite_ptr)&floors,0,0,16,16,0,0,0,0,0,0);
	Sprite_Init((sprite_ptr)&pause_pic,83,50,157,20,0,0,0,0,0,0);
	PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&wall1,0,0,0); //grabs sprite from the picture that was loaded
	PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&wall2,0,1,0);
	PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&bush,0,2,0);
	PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&door,0,3,0);
	PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&endtile,0,14,1);
	PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&starttile,0,13,1);
	PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&pause_pic,0,0,8);
	pause_pic.curr_frame=0;
	for(index=0;index<9;index++)
		PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&floors,index,index,1);
	Sprite_Init((sprite_ptr)&player,144,128,16,16,0,0,0,0,0,0);
	for(index=0;index<16;index++)
		PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&player,index,index,2);
	for(index=16;index<23;index++)
		PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&player,index,index-16,4);
	player.state=SPRITE_ALIVE;
	player.curr_frame=0;
	for (index=0;index<MAX_ZOMBIES;index++)
	{
		Sprite_Init((sprite_ptr)&zombies[index].object,0,0,16,16,0,0,0,0,0,0);
		for(index2=0;index2<16;index2++)
			PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&zombies[index].object,index2,index2,3);
		for(index2=16;index2<25;index2++)
			PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&zombies[index].object,index2,index2-16,5);
		zombies[index].state=0;
		zombies[index].object.curr_frame=0;
	}
	PCX_Delete((pcx_picture_ptr)&imagery);
	PCX_Init((pcx_picture_ptr)&weapons);
	PCX_Load("weapon.pcx",(pcx_picture_ptr)&weapons,1);
	for (index=0; index<MAX_BULLETS; index++)
	{
		 Sprite_Init((sprite_ptr)&bullets[index].object,0,0,4,4,0,0,0,0,0,0);
		 PCX_Get_Sprite((pcx_picture_ptr)&weapons,(sprite_ptr)&bullets[index].object,0,0,0);
		 PCX_Get_Sprite((pcx_picture_ptr)&weapons,(sprite_ptr)&bullets[index].object,1,1,0);
		 PCX_Get_Sprite((pcx_picture_ptr)&weapons,(sprite_ptr)&bullets[index].object,2,2,0);
		 PCX_Get_Sprite((pcx_picture_ptr)&weapons,(sprite_ptr)&bullets[index].object,3,3,0);
		 bullets[index].x                 = 0;
		 bullets[index].y                 = 0;
		 bullets[index].object.curr_frame = 0;
		 bullets[index].state             = false;
	}
	PCX_Delete((pcx_picture_ptr)&weapons);
}

void load_sounds()
{
	sload[SHOT]=Sound_Load("blzlas.voc",(sound_ptr)&soundfx[SHOT],1);
	sload[ZOMBIE_HIT]=Sound_Load("zombie.voc",(sound_ptr)&soundfx[ZOMBIE_HIT],1);
	sload[AMMO]=Sound_Load("ammo.voc",(sound_ptr)&soundfx[AMMO],1);
	sload[HEALTH]=Sound_Load("health.voc",(sound_ptr)&soundfx[HEALTH],1);
}

//displays the instructions for the game
void instructions()
{
	PCX_Init((pcx_picture_ptr)&intro);
	PCX_Load("instruct.pcx",(pcx_picture_ptr)&intro,1);
	PCX_Show_Buffer((pcx_picture_ptr)&intro);
	PCX_Delete((pcx_picture_ptr)&intro);
	while (kbhit()) getch();
	getch();
	Screen_Transition(SCREEN_SWIPE_Y);
}

//displays menu and gets char from keyboard
char menu()
{
	char choice;
	PCX_Init((pcx_picture_ptr)&mmenu);
	PCX_Load("menu.pcx",(pcx_picture_ptr)&mmenu,1);
	PCX_Show_Buffer((pcx_picture_ptr)&mmenu);
	PCX_Delete((pcx_picture_ptr)&mmenu);
	while (kbhit()) getch();
	choice=getch();
	return choice;
}

//initializes all bullets state as false
void init_bullets()
{
	int index;
	for(index=0;index<MAX_BULLETS;index++)
		bullets[index].state=false;
}

//shoots bullets if ammo is not zero
void shoot_bullets(int x,int y,int xv,int yv,int direction)
{
	int index;
	if(ammo!=0)
	{
		if(bullets[0].state==false)
		{
			bullets[0].state=true;
			bullets[0].object.x=x;
			bullets[0].object.y=y;
			bullets[0].xv=xv;
			bullets[0].yv=yv;
			bullets[0].object.curr_frame=direction;
			Sprite_Under((sprite_ptr)&bullets[0].object,double_buffer);
			ammo--;
		}
		else if(bullets[1].state==false)
		{
			bullets[1].state=true;
			bullets[1].object.x=x;
			bullets[1].object.y=y;
			bullets[1].xv=xv;
			bullets[1].yv=yv;
			bullets[1].object.curr_frame=direction;
			Sprite_Under((sprite_ptr)&bullets[1].object,double_buffer);
			ammo--;
		}

		else if(bullets[2].state==false)
		{
			bullets[2].state=true;
			bullets[2].object.x=x;
			bullets[2].object.y=y;
			bullets[2].xv=xv;
			bullets[2].yv=yv;
			bullets[2].object.curr_frame=direction;
			Sprite_Under((sprite_ptr)&bullets[2].object,double_buffer);
			ammo--;
		}
		else if(bullets[3].state==false)
		{
			bullets[3].state=true;
			bullets[3].object.x=x;
			bullets[3].object.y=y;
			bullets[3].xv=xv;
			bullets[3].yv=yv;
			bullets[3].object.curr_frame=direction;
			Sprite_Under((sprite_ptr)&bullets[3].object,double_buffer);
			ammo--;
		}

		else if(bullets[4].state==false)
		{
			bullets[4].state=true;
			bullets[4].object.x=x;
			bullets[4].object.y=y;
			bullets[4].xv=xv;
			bullets[4].yv=yv;
			bullets[4].object.curr_frame=direction;
			Sprite_Under((sprite_ptr)&bullets[4].object,double_buffer);
			ammo--;
		}

		else if(bullets[5].state==false)
		{
			bullets[5].state=true;
			bullets[5].object.x=x;
			bullets[5].object.y=y;
			bullets[5].xv=xv;
			bullets[5].yv=yv;
			bullets[5].object.curr_frame=direction;
			Sprite_Under((sprite_ptr)&bullets[5].object,double_buffer);
			ammo--;
		}
		if(ammo<=0) ammo=0;
	}
}

//erase the sprite for all bullets thats state is true
void erase_bullets()
{
	int index;
	for(index=0;index<MAX_BULLETS;index++)
		if(bullets[index].state==true)
			Sprite_Erase((sprite_ptr)&bullets[index].object,double_buffer);
}

//scan the background for bullets if state is true
void under_bullets()
{
	int index;
	for(index=0;index<MAX_BULLETS;index++)
		if(bullets[index].state==true)
			Sprite_Under((sprite_ptr)&bullets[index].object,double_buffer);
}
//draws the bullets if state is true
void draw_bullets()
{
	int index;
	for(index=0;index<MAX_BULLETS;index++)
		if(bullets[index].state==true)
			Sprite_Draw((sprite_ptr)&bullets[index].object,double_buffer,1);
}

//moves bullets if state is true
//does all collision detection for bullets
void move_bullets()
{
	int index,mindex,bullet_x,bullet_y,bullet_x_center,bullet_y_center,
	cell_x,cell_y,cell_id;
	for (index=0; index<MAX_BULLETS; index++)
	{
		if (bullets[index].state == true)
		{
			bullet_x = (bullets[index].object.x += bullets[index].xv);
			bullet_y = (bullets[index].object.y += bullets[index].yv);
			bullet_x_center = bullet_x+4;
			bullet_y_center = bullet_y+4;
			for (mindex=0; mindex<MAX_ZOMBIES; mindex++)
			{
				if (zombies[mindex].state != ZOMBIE_DEAD && zombies[mindex].state != ZOMBIE_DYING &&
					bullet_x_center > zombies[mindex].x && bullet_x_center < zombies[mindex].x+16 &&
					bullet_y_center > zombies[mindex].y && bullet_y_center < zombies[mindex].y+16)
				{
					if (sload[ZOMBIE_HIT] && (currsnd>=ZOMBIE_HIT || !Sound_Status()) )
					{
						Sound_Stop();
						Sound_Play((sound_ptr)&soundfx[ZOMBIE_HIT]);
						currsnd=ZOMBIE_HIT;
					}
					init_explode(mindex);
					if(--zombies[mindex].lives<=0) zombies[mindex].state=ZOMBIE_DYING;
					else zombies[mindex].state=ZOMBIE_CHASE;
					bullets[index].state=false;
					player_score+=50;
					break;
				}
			}
		  cell_x = bullet_x_center  >> 4;
		  cell_y = bullet_y_center  >> 4;
		  cell_id = world[screen_y][screen_x].cells[cell_y][cell_x];
		  if((bullet_x >= 316) || (bullet_x <= 0) ||
			(bullet_y > (SCREEN_HEIGHT-16)) || (bullet_y <= 0) ||
			(cell_id==0)|| (cell_id==1) || (cell_id==12) || (cell_id==17))
				bullets[index].state = false;
		}
	}
}

void init_explode(int zindex)
{
	int index;
	randomize();
	for(index=0;index<40;index++)
	{
		zombies[zindex].zexplode[index].x=random(16)+zombies[zindex].object.x;
		zombies[zindex].zexplode[index].y=random(16)+zombies[zindex].object.y;
		zombies[zindex].zexplode[index].xv=random(4)-2;
		zombies[zindex].zexplode[index].yv=random(4)-2;
		zombies[zindex].zexplode[index].state=true;
		zombies[zindex].zexplode[index].color=random(4)+36;
		zombies[zindex].zexplode[index].back=Read_Pixel_DB(zombies[zindex].zexplode[index].x,zombies[zindex].zexplode[index].x);
		zombies[zindex].zexplode[index].counter=0;
		zombies[zindex].zexplode[index].threshold=random(2)+5;
	}
}

void erase_explode()
{
	int index,index2;
	for(index=0;index<MAX_ZOMBIES;index++)
	{
		for(index2=0;index2<40;index2++)
		{
			if(zombies[index].zexplode[index2].state==true)
			{
				Write_Pixel_DB(zombies[index].zexplode[index2].x,zombies[index].zexplode[index2].y,zombies[index].zexplode[index2].back);
				if(++zombies[index].zexplode[index2].counter>zombies[index].zexplode[index2].threshold)
				{
					zombies[index].zexplode[index2].state=false;
					Write_Pixel_DB(zombies[index].zexplode[index2].x,zombies[index].zexplode[index2].y,zombies[index].zexplode[index2].back);
				}
			}
		}
	}
}

void under_explode()
{
	int index,index2;
	for(index=0;index<MAX_ZOMBIES;index++)
	{
		for(index2=0;index2<40;index2++)
		{
			if(zombies[index].zexplode[index2].state==true)
			{
				zombies[index].zexplode[index2].back=Read_Pixel_DB(zombies[index].zexplode[index2].x,zombies[index].zexplode[index2].y);
			}
		}
	}
}

void draw_explode()
{
	int index,index2;
	for(index=0;index<MAX_ZOMBIES;index++)
	{
		for(index2=0;index2<40;index2++)
		{
			if(zombies[index].zexplode[index2].state==true)
			{
				Write_Pixel_DB(zombies[index].zexplode[index2].x,zombies[index].zexplode[index2].y,zombies[index].zexplode[index2].color);
			}
		}
	}
}

void animate_explode()
{
		int index,index2;
	for(index=0;index<MAX_ZOMBIES;index++)
	{
		for(index2=0;index2<40;index2++)
		{
			if(zombies[index].zexplode[index2].state==true)
			{
				zombies[index].zexplode[index2].x+=zombies[index].zexplode[index2].xv;
				zombies[index].zexplode[index2].y+=zombies[index].zexplode[index2].yv;
			}
		}
	}
}


//initializes all zombies as dead
void init_zombies()
{
	int index;
	for(index=0;index<MAX_ZOMBIES;index++)
		zombies[index].state=ZOMBIE_DEAD;
}

//finds zombies to use for the next screen
int start_zombie(int x,int y)
{

	int index;
	for(index=0;index<MAX_ZOMBIES;index++)
	{
		if (zombies[index].state==ZOMBIE_DEAD)
		{
			zombies[index].x=x;
			zombies[index].y=y;
			zombies[index].xv=0;
			zombies[index].yv=0;
			zombies[index].state=1;
			zombies[index].lives=random(3)+difficulty+1;
			zombies[index].direction=0;
			zombies[index].threshold=0;
			zombies[index].counter=0;
			zombies[index].object.curr_frame = 0;
			zombies[index].object.x = zombies[index].x;
			zombies[index].object.y = zombies[index].y;
			Sprite_Under((sprite_ptr)&zombies[index].object,double_buffer);
			return(1);
		}
	}
	return(0);

}

//start all zombies if the zombie is not dead yet
void start_zombies(int wx,int wy)
{
	int count=0;
	int index,n_zombies;
	init_zombies();
	n_zombies = world[wy][wx].num_zombies;
	for (index=0;index<MAX_ZOMBIES && count<n_zombies;index++)
	{
		if(world[wy][wx].positions[index].state)
		{
			start_zombie(world[wy][wx].positions[index].x,world[wy][wx].positions[index].y);
			count++;
		}
	}
}

//erase all zombies if they are not dead
void erase_zombies()
{
	int index;
	for(index=0;index<MAX_ZOMBIES;index++)
	{
		if(zombies[index].state!=ZOMBIE_DEAD)
		{
			zombies[index].object.x=zombies[index].x;
			zombies[index].object.y=zombies[index].y;
			Sprite_Erase((sprite_ptr)&zombies[index].object,double_buffer);
		}
	}
}

//scan the background for the zombies if not dead
void under_zombies()
{
	int index;
	for(index=0;index<MAX_ZOMBIES;index++)
	{
		if(zombies[index].state!=ZOMBIE_DEAD)
		{
			zombies[index].object.x=zombies[index].x;
			zombies[index].object.y=zombies[index].y;
			Sprite_Under((sprite_ptr)&zombies[index].object,double_buffer);

		}
	}
}

//draw the zombies if not dead
void draw_zombies()
{
	int index;
	for(index=0;index<MAX_ZOMBIES;index++)
	{
		if(zombies[index].state!=ZOMBIE_DEAD)
		{
			zombies[index].object.x=zombies[index].x;
			zombies[index].object.y=zombies[index].y;
			if(zombies[index].state==ZOMBIE_DYING)
			{
				if(zombies[index].object.curr_frame<16)
				{
					world[screen_y][screen_x].positions[index].state=false;
					world[screen_y][screen_x].num_zombies--;
					zombies[index].object.curr_frame=16;
				}
				else
				{
					zombies[index].object.curr_frame++;
				}
				if(zombies[index].object.curr_frame>=24)
				{
					zombies[index].state=ZOMBIE_DEAD;
				}
				Sprite_Draw((sprite_ptr)&zombies[index].object,double_buffer,1);
			}
			else Sprite_Draw((sprite_ptr)&zombies[index].object,double_buffer,1);

		}
	}
}

//moves zombies if not dead
void move_zombies()
{
	int index,mx,my,mdx,mdy,cell_x,cell_id,cell_y,cell_bit,change_state;
	bool animate;
	for(index=0;index<MAX_ZOMBIES;index++)
	{
		if(zombies[index].state!=ZOMBIE_DEAD&&zombies[index].state!=ZOMBIE_DYING)
		{
			mdy=mdx=0;
			change_state=0;
			//switch for all the zombie states
			switch(zombies[index].state)
			{
				case ZOMBIE_STILL:
					mx=zombies[index].x+8;
					my=zombies[index].y+14;
					mdx=mdy=0;
					animate=false;
					break;
				case ZOMBIE_CHASE:
					mx=zombies[index].x+8;
					my=zombies[index].y+14;
					if(mx<player.x+8) mdx=2;
					else if(mx>player.x+8) mdx=-2;
					if(my<player.y+14) mdy=2;
					else if(my>player.y+14) mdy=-2;
					animate=true;
					break;
				case ZOMBIE_EVADE:
					mx=zombies[index].x+8;
					my=zombies[index].y+14;
					if(mx>player.x+8) mdx=2;
					else if(mx<player.x+8) mdx=-2;
					if(my>player.y+15) mdy=2;
					else if(my<player.y+15) mdy=-2;
					animate=true;
					break;
				case ZOMBIE_DEFAULT_RANDOM:
					mx=zombies[index].x+8;
					my=zombies[index].y+14;
					mdx=zombies[index].xv;
					mdy=zombies[index].yv;
					animate=true;
					break;
				case ZOMBIE_DYER_RANDOM:
					mx=zombies[index].x+8;
					my=zombies[index].y+14;
					mdx=random(5)-2;
					mdy=random(5)-2;
					animate=true;
					break;
			}
			//move zombie
			mx+=mdx;
			zombies[index].x+=mdx;

			//check for zombie
			cell_x=mx>>4;
			cell_y=my>>4;
			cell_id=world[screen_y][screen_x].cells[cell_y][cell_x];
			cell_bit=cell_id&cell_id;
			//check if zombie has hit a wall or has gone out of bounds
			if(mx>310||mx<12||cell_bit==GREY_WALL||cell_bit==BLUE_WALL||
			   cell_bit==BUSH||cell_bit==HEALTH_GENERATOR||
			   cell_bit==DOOR||cell_bit==FAKEWALL)
			{
				mx-=mdx;
				zombies[index].x-=mdx;
			}
			my+=mdy;
			zombies[index].y+=mdy;
			cell_x=mx>>4;
			cell_y=my>>4;
			cell_id=world[screen_y][screen_x].cells[cell_y][cell_x];
			cell_bit=cell_id&cell_id;
			if(my>150||my<12||cell_bit==GREY_WALL||cell_bit==BLUE_WALL||
			   cell_bit==BUSH||cell_bit==HEALTH_GENERATOR||
			   cell_bit==DOOR||cell_bit==FAKEWALL)
			{
				my-=mdy;
				zombies[index].y-=mdy;
			}
			//check for zombie hitting player
			if(player.state!=SPRITE_DEAD&&player.state!=SPRITE_DYING&&mx>player.x&&mx<player.x+16&&my>player.y&&my<player.y+16)
			{
				//take off health
				//if health is less than or equal to 0 player dying
				health-=difficulty;
				if(health<=0)
				{
					health=0;

					player.curr_frame=16;
					player.state=SPRITE_DYING;
				}
			}
			//if moving
			if(animate)
			{
				if ((mdx!=0 && mdy!=0) || (mdx!=0 && mdy==0 ))
				{
					if (mdx>0)
					{
						if (++zombies[index].object.curr_frame>7||zombies[index].object.curr_frame<4)
						zombies[index].object.curr_frame = 4;
					}
					else
					{
						if (++zombies[index].object.curr_frame > 15||zombies[index].object.curr_frame<12)
						zombies[index].object.curr_frame = 12;
					}
				}
				if (mdy!=0 && mdx==0)
				{
					if (mdy>0)
					{
						if (++zombies[index].object.curr_frame > 3||zombies[index].object.curr_frame<0)
							zombies[index].object.curr_frame = 0;

					}
					else
					{
						if (++zombies[index].object.curr_frame > 11||zombies[index].object.curr_frame<8)
								zombies[index].object.curr_frame = 8;
					}

				}
			}
			if(++zombies[index].counter>zombies[index].threshold) change_state=1;
			if(change_state)
			{
				//probability for the zombies state
				switch(rand()%20)
				{
					//15% of zombie being still
					case 0:
					case 1:
					case 2:
						zombies[index].state=ZOMBIE_STILL;
						zombies[index].counter=0;
						zombies[index].threshold=rand()%20+20;
						break;
				  //40% of zombie chasing player
					case 3:
					case 4:
					case 5:
					case 6:
					case 7:
					case 8:
					case 9:
					case 10:
						zombies[index].object.curr_frame=0;
						zombies[index].state=ZOMBIE_CHASE;
						zombies[index].counter=0;
						zombies[index].threshold=rand()%20+20;
						break;
					//20% of zombie evading player
					case 11:
					case 12:
					case 13:
					case 14:
						zombies[index].object.curr_frame=0;
						zombies[index].state=ZOMBIE_EVADE;
						zombies[index].counter=0;
						zombies[index].threshold=rand()%20+20;
						break;
					//15% of zombie picking random direction
					case 15:
					case 16:
					case 17:
						zombies[index].xv=random(5)-2;
						zombies[index].yv=random(5)-2;
						zombies[index].object.curr_frame=0;
						zombies[index].state=ZOMBIE_DEFAULT_RANDOM;
						zombies[index].counter=0;
						zombies[index].threshold=rand()%20+20;
						break;
					//10% of being totally random
					case 18:
					case 20:
						zombies[index].object.curr_frame=0;
						zombies[index].state=ZOMBIE_DYER_RANDOM;
						zombies[index].counter=0;
						zombies[index].threshold=rand()%20+20;
						break;
				}
			}
		}
	}
}

//setup snow screens

void init_snow()
{
	int index;
	if(world[screen_y][screen_x].snowing)
	{
		for(index=0;index<60;index++)
		{
			snow[index].x=random(320);
			snow[index].y=random(160);
			snow[index].yv=random(3)+1;
			snow[index].xv=random(2)-1;
			snow[index].color=random(3)+28;
			snow[index].stop=random(100)+60;
			snow[index].state=true;
		}
	}
}

//erase all the snow with color scan by under_snow() function
void erase_snow()
{
	int index;
	if(world[screen_y][screen_x].snowing)
	{
		for(index=0;index<60;index++)
				Write_Pixel_DB(snow[index].x,snow[index].y,snow[index].back);
	}
}

//scans the pixel behind snow
void under_snow()
{
	int index;
	if(world[screen_y][screen_x].snowing)
	{
		for(index=0;index<60;index++)
				snow[index].back=Read_Pixel_DB(snow[index].x,snow[index].y);
	}
}

//plots pixel for the snow
void draw_snow()
{
	int index;
	if(world[screen_y][screen_x].snowing)
	{
		for(index=0;index<60;index++)
				Write_Pixel_DB(snow[index].x,snow[index].y,snow[index].color);
	}
}

//moves the snow
void move_snow()
{
	int index;
	if(world[screen_y][screen_x].snowing)
	{
		for(index=0;index<60;index++)
		{
			if(snow[index].state)
			{
				snow[index].y+=snow[index].yv;
				snow[index].x+=snow[index].xv;
				snow[index].xv=random(2)-1;
				if(snow[index].y>snow[index].stop)
				{
					snow[index].yv=0;
					snow[index].xv=0;
					snow[index].state=false;
				}
			}
			else if(!snow[index].state)
			{
				snow[index].color--;
				if(snow[index].color<12)
				{
					snow[index].x=random(320);
					snow[index].y=0;
					snow[index].yv=random(3)+1;
					snow[index].xv=random(2)-1;
					snow[index].color=random(3)+28;
					snow[index].stop=random(160);
					snow[index].state=true;
				}
			}
		}
	}
}



//displays current game status
void show_game_status()
{
	longxy(45,174,player_score,36,0);
	Print_String(180,174,0,"   ",0);
	numxy(180,174,health,36,0);
	numxy(85,186,keys,36,0);
	Print_String(265,186,0,"   ",0);
	numxy(265,186,ammo,36,0);
}

//this function does everything for playing the game
int playgame(char *levelname,char *musicfile)
{
	int exit_game=0;
	randomize();
	int cell_id,cell_x,cell_y,cell_bit,index,index2;
	keys=0;
	int done=0;
	int players_dir=0;
	int dx=0,dy=0;
	load_sprites();
	load_screens(levelname);
	PCX_Init((pcx_picture_ptr)&background);
	PCX_Load("gamescreen.pcx",(pcx_picture_ptr)&background,1);
	PCX_Show_Buffer((pcx_picture_ptr)&background);
	PCX_Delete((pcx_picture_ptr)&background);
	Create_Double_Buffer(160);
	for(index=0;index<MAX_BULLETS;index++) bullets[index].state=false;
	Keyboard_Install_Driver();
	init_snow();
	draw_screen(screen_x,screen_y);
	start_zombies(screen_x,screen_y);
	under_snow();
	Sprite_Under((sprite_ptr)&player,double_buffer);
	load_sounds();
	mload=Music_Load(musicfile,(music_ptr)&song);
	if (mload)
	{
		Music_Stop();
		Music_Play((music_ptr)&song,0);
	}
	do
	{
		//MAIN GAME EVENT LOOP
		screen_change=0;
		dx=dy=0;
		//erase all game objects
		erase_snow();
		erase_bullets();
		erase_zombies();
		erase_explode();
		Sprite_Erase((sprite_ptr)&player,double_buffer);
		//controls for the player
		if((player.state==SPRITE_ALIVE)&&(keyboard_state[MAKE_ESC]||keyboard_state[MAKE_DOWN]||keyboard_state[MAKE_RIGHT]||
						keyboard_state[MAKE_UP]||keyboard_state[MAKE_LEFT]||keyboard_state[MAKE_SPACE]))
		{
			if(keyboard_state[MAKE_ESC]) exit_game=1;
			if(keyboard_state[MAKE_DOWN])
			{
				dy=4;
				player.curr_frame++;
				if(player.curr_frame<0||player.curr_frame>3) player.curr_frame=0;
				players_dir=3;
			}
			else if(keyboard_state[MAKE_RIGHT])
			{
				dx=4;
				player.curr_frame++;
				if(player.curr_frame<4||player.curr_frame>7) player.curr_frame=4;
				players_dir=0;
			}
			else if(keyboard_state[MAKE_UP])
			{
				dy=-4;
				player.curr_frame++;
				if(player.curr_frame<8||player.curr_frame>11) player.curr_frame=8;
				players_dir=2;
			}
			else if(keyboard_state[MAKE_LEFT])
			{
				dx=-4;
				player.curr_frame++;
				if(player.curr_frame<12||player.curr_frame>15) player.curr_frame=12;
				players_dir=1;
			}
			if(keyboard_state[MAKE_SPACE] && ammo)
			{
				if (sload[SHOT] && (currsnd>=SHOT || !Sound_Status()) && (bullets[0].state=false||
				    bullets[1].state==false||bullets[2].state==false||bullets[3].state==false||
				    bullets[4].state==false||bullets[5].state==false))
				{
					Sound_Stop();
					Sound_Play((sound_ptr)&soundfx[SHOT]);
					currsnd=SHOT;
				}
				shoot_bullets(player.x+6,player.y+5,bullet_vel_x[players_dir],bullet_vel_y[players_dir],players_dir);
			}
		}
		//death sequence for player
		else if(player.state==SPRITE_DYING)
		{
			if(++player.curr_frame==22)
			{
				player.state=SPRITE_DEAD;
				Time_Delay(6);
				done=1;
			}
		}
		//move all game objects
		move_snow();
		move_bullets();
		move_zombies();
		animate_explode();
		player.x+=dx;
		player.y+=dy;
		show_game_status();
		old_screen_x = screen_x;
		old_screen_y = screen_y;
		//see if there is a screen change
		if (player.x>320-16)
		{
			if (++screen_x>3)
			{
				screen_x=3;
				player.x=320-16;
			}
			else
			{
				screen_change=1;
				player.x=0;
			}
		  }
		  else if (player.x<0)
		  {
			if (--screen_x<0)
			{
				screen_x=0;
				player.x=0;
			}
			else
			{
				screen_change=1;
				player.x=320-16;
			}
		  }
		  if (player.y>160-16)
		  {
			if (++screen_y>5)
			{
				screen_y=3;
				player.y=160-16;
			}
			else
			{
				screen_change=1;
				player.y=0;
			}
		  }
		  else if (player.y<0)
		  {
		  if (--screen_y<0)
		  {
			screen_y=0;
			player.y=0;
		  }
		  else
		  {
			screen_change=1;
			player.y=160-16;
		  }
		}
		else
		{
			//check for players collision with a wall
			cell_x=(player.x+8)>>4;
			cell_y=(player.y+14)>>4;
			cell_id=world[screen_y][screen_x].cells[cell_y][cell_x];
			cell_bit=cell_id&cell_id;
			if(cell_bit==GREY_WALL||cell_bit==BLUE_WALL||cell_bit==BUSH)
			{
				player.x-=dx;
				player.y-=dy;
			}
			else
			{
				//items for player to pick up
				switch(cell_bit)
				{
					case TILE_HEALTH:
						if(health<100)
						{
							if (sload[HEALTH] && (currsnd>=HEALTH || !Sound_Status()) )
							{
								Sound_Stop();
								Sound_Play((sound_ptr)&soundfx[HEALTH]);
								currsnd=HEALTH;
							}
							health+=20;
							if(health>100) health=100;
							floors.x=cell_x<<4;
							floors.y=cell_y<<4;
							floors.curr_frame=0;
							Sprite_Draw((sprite_ptr)&floors,double_buffer,1);
							world[screen_y][screen_x].cells[cell_y][cell_x]=TILE_FLOOR;
						}
						break;
					case TILE_AMMO:
						if(ammo<300)
						{
							if (sload[AMMO] && (currsnd>=AMMO || !Sound_Status()) )
							{
								Sound_Stop();
								Sound_Play((sound_ptr)&soundfx[AMMO]);
								currsnd=AMMO;
							}
							ammo+=18;
							if(ammo>300) ammo=300;
							floors.x=cell_x<<4;
							floors.y=cell_y<<4;
							floors.curr_frame=0;
							Sprite_Draw((sprite_ptr)&floors,double_buffer,1);
							world[screen_y][screen_x].cells[cell_y][cell_x]=TILE_FLOOR;
						}
						break;
					case GRASS_HEALTH:
						if(health<100)
						{
							if (sload[HEALTH] && (currsnd>=HEALTH || !Sound_Status()) )
							{
								Sound_Stop();
								Sound_Play((sound_ptr)&soundfx[HEALTH]);
								currsnd=HEALTH;
							}
							health+=20;
							if(health>100) health=100;
							floors.x=cell_x<<4;
							floors.y=cell_y<<4;
							floors.curr_frame=3;
							Sprite_Draw((sprite_ptr)&floors,double_buffer,1);
							world[screen_y][screen_x].cells[cell_y][cell_x]=GRASS;
						}
						break;
					case GRASS_AMMO:
						if(ammo<300)
						{
							if (sload[AMMO] && (currsnd>=AMMO || !Sound_Status()) )
							{
								Sound_Stop();
								Sound_Play((sound_ptr)&soundfx[AMMO]);
								currsnd=AMMO;
							}
							ammo+=18;
							if(ammo>300) ammo=300;
							floors.x=cell_x<<4;
							floors.y=cell_y<<4;
							floors.curr_frame=3;
							Sprite_Draw((sprite_ptr)&floors,double_buffer,1);
							world[screen_y][screen_x].cells[cell_y][cell_x]=GRASS;
						}
						break;
					case HEALTH_GENERATOR:
						if(++health>100) health=100;
						break;
					case TILE_KEY:
						keys++;
						floors.x=cell_x<<4;
						floors.y=cell_y<<4;
						floors.curr_frame=0;
						Sprite_Draw((sprite_ptr)&floors,double_buffer,1);
						world[screen_y][screen_x].cells[cell_y][cell_x]=TILE_FLOOR;
						break;
					case GRASS_KEY:
						keys++;
						floors.x=cell_x<<4;
						floors.y=cell_y<<4;
						floors.curr_frame=3;
						Sprite_Draw((sprite_ptr)&floors,double_buffer,1);
						world[screen_y][screen_x].cells[cell_y][cell_x]=GRASS;
						break;
					case DOOR:
						if(keys)
						{
							keys--;
							floors.x=cell_x<<4;
							floors.y=cell_y<<4;
							floors.curr_frame=0;
							Sprite_Draw((sprite_ptr)&floors,double_buffer,1);
							world[screen_y][screen_x].cells[cell_y][cell_x]=TILE_FLOOR;
						}
						else
						{
							player.x-=dx;
							player.y-=dy;
						}
						break;
					case END:
						done = 2;
						break;

					default:break;
				}
			}
		}
		//if screen has change start every thing for new screen
		if(screen_change)
		{
			Fill_Double_Buffer(0);
			init_snow();
			init_bullets();
			draw_screen(screen_x,screen_y);
			start_zombies(screen_x,screen_y);
		}
		//scan under all objects in the game
		under_bullets();
		under_zombies();
		under_explode();
		Sprite_Under((sprite_ptr)&player,double_buffer);
		under_snow();
		//draw all objects in the game
		draw_bullets();
		draw_zombies();
		draw_explode();
		Sprite_Draw((sprite_ptr)&player,double_buffer,1);
		draw_snow();
		//copy all sprites to double buffer
		if(keyboard_state[MAKE_F1])
		{
			Music_Stop();
			Sprite_Under((sprite_ptr)&pause_pic,double_buffer);
			Sprite_Draw((sprite_ptr)&pause_pic,double_buffer,1);
			Display_Double_Buffer(double_buffer,0);
			while(!keyboard_state[MAKE_F2]);
			Sprite_Erase((sprite_ptr)&pause_pic,double_buffer);
			Music_Resume();
		}
		Display_Double_Buffer(double_buffer,0);
		if (Music_Status()==2 && mload)
		{
			Music_Stop();
			Music_Play((music_ptr)&song,0);
		}
		if (!Sound_Status()) currsnd=6;
		Time_Delay(1);
	}while(!exit_game&&player.state!=SPRITE_DEAD&&done==0);
	Keyboard_Remove_Driver(); //GETS RID OF KEYBOARD DRIVER
	Delete_Double_Buffer(); //DEALLOCATES MEMORY FOR DOUBLE BUFFER
	if (done==2)
	{
		Time_Delay(10);
		Screen_Transition(SCREEN_SWIPE_Y);
	}
	else if (done==0) Screen_Transition(SCREEN_DARKNESS); //FADES TO BLACK
	else Screen_Transition(SCREEN_WHITENESS); //FADES TO WHITE
	Sprite_Delete((sprite_ptr)&wall1); //DEALLOCATE ALL MEMORY FOR THE SPRITES
	Sprite_Delete((sprite_ptr)&wall2);
	Sprite_Delete((sprite_ptr)&bush);
	Sprite_Delete((sprite_ptr)&player);
	Sprite_Delete((sprite_ptr)&floors);
	Sprite_Delete((sprite_ptr)&door);
	Sprite_Delete((sprite_ptr)&endtile);
	Sprite_Delete((sprite_ptr)&pause_pic);
	for(index=0;index<MAX_ZOMBIES;index++) Sprite_Delete((sprite_ptr)&zombies[index].object);
	for(index=0;index<MAX_BULLETS;index++) Sprite_Delete((sprite_ptr)&bullets[index].object);
	for(index=0;index<6;index++)
		if (sload[index]) Sound_Unload((sound_ptr)&soundfx[index]);
	if (mload)
	{
		Music_Stop();
		Music_Unload((music_ptr)&song);
	}
	return done;
}

void load_sprites_boss()
{
	int index,index2,n_zombies;
	textcolor(15);
	PCX_Init((pcx_picture_ptr)&imagery);
	PCX_Load("graphics.pcx",(pcx_picture_ptr)&imagery,1);
	Sprite_Init((sprite_ptr)&wall1,0,0,16,16,0,0,0,0,0,0); //initializes the x,y,height,width,.........
	Sprite_Init((sprite_ptr)&wall2,0,0,16,16,0,0,0,0,0,0);
	Sprite_Init((sprite_ptr)&starttile,0,0,16,16,0,0,0,0,0,0);
	Sprite_Init((sprite_ptr)&floors,0,0,16,16,0,0,0,0,0,0);
	Sprite_Init((sprite_ptr)&pause_pic,83,50,157,20,0,0,0,0,0,0);
	Sprite_Init((sprite_ptr)&boss.object,0,0,32,32,0,0,0,0,0,0);
	PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&wall1,0,0,0); //grabs sprite from the picture that was loaded
	PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&wall2,0,1,0);
	PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&starttile,0,13,1);
	PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&pause_pic,0,0,8);
	pause_pic.curr_frame=0;
	for(index=0;index<9;index++)
		PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&floors,index,index,1);
	Sprite_Init((sprite_ptr)&player,144,128,16,16,0,0,0,0,0,0);
	for(index=0;index<16;index++)
		PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&player,index,index,2);
	for(index=16;index<23;index++)
		PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&player,index,index-16,4);
	player.state=SPRITE_ALIVE;
	player.curr_frame=0;
	if (!PCX_Load("boss.pcx",(pcx_picture_ptr)&imagery,1)) exit(1);
	for(index2=0;index2<8;index2++)
		PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&boss.object,index2,index2,0);
	for(index2=8;index2<16;index2++)
		PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&boss.object,index2,index2-8,1);
	for(index2=16;index2<24;index2++)
		PCX_Get_Sprite((pcx_picture_ptr)&imagery,(sprite_ptr)&boss.object,index2,index2-16,2);
	boss.state=1;
	boss.object.curr_frame=0;
	PCX_Delete((pcx_picture_ptr)&imagery);
	PCX_Init((pcx_picture_ptr)&weapons);
	PCX_Load("weapon.pcx",(pcx_picture_ptr)&weapons,1);
	for (index=0; index<MAX_BULLETS; index++)
	{
		 Sprite_Init((sprite_ptr)&bullets[index].object,0,0,4,4,0,0,0,0,0,0);
		 PCX_Get_Sprite((pcx_picture_ptr)&weapons,(sprite_ptr)&bullets[index].object,0,0,0);
		 PCX_Get_Sprite((pcx_picture_ptr)&weapons,(sprite_ptr)&bullets[index].object,1,1,0);
		 PCX_Get_Sprite((pcx_picture_ptr)&weapons,(sprite_ptr)&bullets[index].object,2,2,0);
		 PCX_Get_Sprite((pcx_picture_ptr)&weapons,(sprite_ptr)&bullets[index].object,3,3,0);
		 bullets[index].x                 = 0;
		 bullets[index].y                 = 0;
		 bullets[index].object.curr_frame = 0;
		 bullets[index].state             = false;
	}
	PCX_Delete((pcx_picture_ptr)&weapons);
	boss.lives=100*difficulty;
	for (index=0;index<MAX_ZOMBIES;index++)
		zombies[index].state=0;
}

void init_boss(int wx,int wy)
{
	int n_zombies = world[wy][wx].num_zombies;
	if(n_zombies)
	{
		boss.x=world[wx][wy].positions[0].x;
		boss.y=world[wx][wy].positions[0].y;
		boss.xv=0;
		boss.yv=0;
		boss.state=2;
		boss.direction=0;
		boss.threshold=0;
		boss.counter=0;
		boss.object.curr_frame = 0;
		boss.object.x = boss.x;
		boss.object.y = boss.y;
		Sprite_Under((sprite_ptr)&boss.object,double_buffer);
	}
	else boss.state=ZOMBIE_DEAD;
}

void erase_boss()
{
	if(boss.state!=ZOMBIE_DEAD)
	{
		boss.object.x=boss.x;
		boss.object.y=boss.y;
		Sprite_Erase((sprite_ptr)&boss.object,double_buffer);
	}
}

void under_boss()
{
	if(boss.state!=ZOMBIE_DEAD)
	{
		boss.object.x=boss.x;
		boss.object.y=boss.y;
		Sprite_Under((sprite_ptr)&boss.object,double_buffer);
	}
}

void move_boss()
{
	int index,mx,my,mdx,mdy,cell_x,cell_id,cell_y,cell_bit,change_state;
	bool animate;
	if(boss.state!=ZOMBIE_DEAD&&boss.state!=ZOMBIE_DYING)
	{
		mdy=mdx=0;
		change_state=0;
		//switch for all the zombie states
		switch(boss.state)
		{
			case ZOMBIE_STILL:
				mx=boss.x+16;
				my=boss.y+28;
				mdx=mdy=0;
				animate=false;
				break;
			case ZOMBIE_CHASE:
				mx=boss.x+16;
				my=boss.y+28;
				if(mx<player.x+16) mdx=4;
				else if(mx>player.x+16) mdx=-4;
				if(my<player.y+28) mdy=4;
				else if(my>player.y+28) mdy=-4;
				animate=true;
				break;
			case ZOMBIE_EVADE:
				mx=boss.x+16;
				my=boss.y+28;
				if(mx>player.x+16) mdx=4;
				else if(mx<player.x+16) mdx=-4;
				if(my>player.y+28) mdy=4;
				else if(my<player.y+28) mdy=-4;
				animate=true;
				break;
			case ZOMBIE_DEFAULT_RANDOM:
				mx=boss.x+16;
				my=boss.y+28;
				mdx=boss.xv;
				mdy=boss.yv;
				animate=true;
				break;
			case ZOMBIE_DYER_RANDOM:
				mx=boss.x+16;
				my=boss.y+28;
				mdx=random(5)-4;
				mdy=random(5)-4;
				animate=true;
				break;
		}
		//move zombie
		mx+=mdx;
		boss.x+=mdx;
			//check for zombie
		cell_x=mx>>4;
		cell_y=my>>4;
		cell_id=world[screen_y][screen_x].cells[cell_y][cell_x];
		cell_bit=cell_id&cell_id;
		//check if zombie has hit a wall or has gone out of bounds
		if(mx>290||mx<30||cell_bit==GREY_WALL||cell_bit==BLUE_WALL||
		   cell_bit==BUSH||cell_bit==HEALTH_GENERATOR||
		   cell_bit==DOOR||cell_bit==FAKEWALL)
		{
			mx-=mdx;
			boss.x-=mdx;
		}
		my+=mdy;
		boss.y+=mdy;
		cell_x=mx>>4;
		cell_y=my>>4;
		cell_id=world[screen_y][screen_x].cells[cell_y][cell_x];
		cell_bit=cell_id&cell_id;
		if(my>130||my<30||cell_bit==GREY_WALL||cell_bit==BLUE_WALL||
		   cell_bit==BUSH||cell_bit==HEALTH_GENERATOR||
		   cell_bit==DOOR||cell_bit==FAKEWALL)
		{
			my-=mdy;
			boss.y-=mdy;
		}
		//check for zombie hitting player
		if(player.state!=SPRITE_DEAD&&player.state!=SPRITE_DYING&&mx>player.x&&mx<player.x+32&&my>player.y&&my<player.y+32)
		{
			//take off health
			//if health is less than or equal to 0 player dying
			health-=difficulty*2;
			if(health<=0)
			{
				health=0;
				player.curr_frame=16;
				player.state=SPRITE_DYING;
			}
		}
		//if moving
		if(animate)
		{
			if ((mdx!=0 && mdy!=0) || (mdx!=0 && mdy==0 ))
			{
				if (mdx>0)
				{
					if (++boss.object.curr_frame>7||boss.object.curr_frame<4)
					boss.object.curr_frame = 4;
				}
				else
				{
					if (++boss.object.curr_frame > 15||boss.object.curr_frame<12)
					boss.object.curr_frame = 12;
				}
			}
			if (mdy!=0 && mdx==0)
			{
				if (mdy>0)
				{
					if (++boss.object.curr_frame > 3||boss.object.curr_frame<0)
						boss.object.curr_frame = 0;
					}
				else
				{
					if (++boss.object.curr_frame > 11||boss.object.curr_frame<8)
							boss.object.curr_frame = 8;
				}
			}
		}
		if(++boss.counter>boss.threshold) change_state=1;
		if(change_state)
		{
			//probability for the zombies state
			switch(rand()%10)
			{
				case 0:
					boss.state=ZOMBIE_STILL;
					boss.counter=0;
					boss.threshold=rand()%20+20;
					break;
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
					boss.object.curr_frame=0;
					boss.state=ZOMBIE_CHASE;
					boss.counter=0;
					boss.threshold=rand()%20+20;
					break;
				case 7:
					boss.object.curr_frame=0;
					boss.state=ZOMBIE_EVADE;
					boss.counter=0;
					boss.threshold=rand()%20+20;
					break;
				case 8:
					boss.xv=random(5)-2;
					boss.yv=random(5)-2;
					boss.object.curr_frame=0;
					boss.state=ZOMBIE_DEFAULT_RANDOM;
					boss.counter=0;
					boss.threshold=rand()%20+20;
					break;
				case 9:
					boss.object.curr_frame=0;
					boss.state=ZOMBIE_DYER_RANDOM;
					boss.counter=0;
					boss.threshold=rand()%20+20;
					break;
			}
		}
	}
}

int draw_boss()
{
	int rv=0;
	if(boss.state!=ZOMBIE_DEAD)
	{
		boss.object.x=boss.x;
		boss.object.y=boss.y;
		if(boss.state==ZOMBIE_DYING)
		{
			if(boss.object.curr_frame<16)
			{
				world[screen_y][screen_x].positions[0].state=false;
				world[screen_y][screen_x].num_zombies--;
				boss.object.curr_frame=16;
			}
			else
			{
				boss.object.curr_frame++;
			}
			if(boss.object.curr_frame>=23)
			{
				boss.state=ZOMBIE_DEAD;
				rv=1;
			}
			Sprite_Draw((sprite_ptr)&boss.object,double_buffer,1);
		}
		else Sprite_Draw((sprite_ptr)&boss.object,double_buffer,1);
	}
	return rv;
}

void load_screens_boss(char *filename)
{
	ifstream fin(filename);
	int x,y,index_x,index_y,index,temp,row,col;
	int zombie_x,zombie_y;
	for (row=0;row<3;row++)
	{
		for (col=0;col<3;col++)
		{
			world[row][col].num_zombies=0;
			for (index=0;index<MAX_ZOMBIES;index++)
				world[row][col].positions[index].state=false;
		}
	}
	if(fin.good())
	{
		for (row=0;row<3;row++)
		{
			for (col=0;col<3;col++)
			{
				fin>>temp;
				if (temp==1) world[row][col].snowing=on;
				else world[row][col].snowing=off;
			}
		}
		while(!fin.eof())
		{
			for(index_y=0;index_y<3;index_y++)
			{
				for(index_x=0;index_x<3;index_x++)
				{
					index=0;
					for(y=0;y<CELL_ROWS;y++)
					{
						for(x=0;x<CELL_COLS;x++)
						{
							fin>>world[index_y][index_x].cells[y][x];
							if(world[index_y][index_x].cells[y][x]==START)
							{
								screen_y=index_y;
								screen_x=index_x;
								player.x=x<<4;
								player.y=y<<4;
							}
							if(world[index_y][index_x].cells[y][x]==13)
							{
								zombie_x=x<<4;
								zombie_y=y<<4;
								world[index_y][index_x].positions[0].x=zombie_x;
								world[index_y][index_x].positions[0].y=zombie_y;
								world[index_y][index_x].positions[0].state=true;
								world[index_y][index_x].cells[y][x]=TILE_FLOOR;
								world[index_y][index_x].num_zombies=1;
							}
							else if(world[index_y][index_x].cells[y][x]==14)
							{
								zombie_x=x<<4;
								zombie_y=y<<4;
								world[index_y][index_x].positions[0].x=zombie_x;
								world[index_y][index_x].positions[0].y=zombie_y;
								world[index_y][index_x].positions[0].state=true;
								world[index_y][index_x].cells[y][x]=GRASS;
								world[index_y][index_x].num_zombies=1;
							}
						}
					}
				}
			}
		}
		fin.close();
	}
}

int play_game_boss(char *worldfile,char *musicfile)
{
	int exit_game=0;
	randomize();
	int cell_id,cell_x,cell_y,cell_bit,index,index2;
	keys=0;
	int done=0;
	int players_dir=0;
	int dx=0,dy=0;
	load_sprites_boss();
	load_screens_boss(worldfile);
	PCX_Init((pcx_picture_ptr)&background);
	if (!PCX_Load("gamescreen.pcx",(pcx_picture_ptr)&background,1)) return 0;
	PCX_Show_Buffer((pcx_picture_ptr)&background);
	PCX_Delete((pcx_picture_ptr)&background);
	Create_Double_Buffer(160);
	for(index=0;index<MAX_BULLETS;index++) bullets[index].state=false;
	Keyboard_Install_Driver();
	init_snow();
	draw_screen(screen_x,screen_y);
	init_boss(screen_x,screen_y);
	under_snow();
	Sprite_Under((sprite_ptr)&player,double_buffer);
	load_sounds();
	mload=Music_Load(musicfile,(music_ptr)&song);
	if (mload)
	{
		Music_Stop();
		Music_Play((music_ptr)&song,0);
	}
	do
	{
		//MAIN GAME EVENT LOOP
		screen_change=0;
		dx=dy=0;
		//erase all game objects
		erase_snow();
		erase_bullets();
		erase_boss();
		erase_explode();
		Sprite_Erase((sprite_ptr)&player,double_buffer);
		//controls for the player
		if((player.state==SPRITE_ALIVE)&&(keyboard_state[MAKE_ESC]||keyboard_state[MAKE_DOWN]||keyboard_state[MAKE_RIGHT]||
						keyboard_state[MAKE_UP]||keyboard_state[MAKE_LEFT]||keyboard_state[MAKE_SPACE]))
		{
			if(keyboard_state[MAKE_ESC]) exit_game=1;
			if(keyboard_state[MAKE_DOWN])
			{
				dy=4;
				player.curr_frame++;
				if(player.curr_frame<0||player.curr_frame>3) player.curr_frame=0;
				players_dir=3;
			}
			else if(keyboard_state[MAKE_RIGHT])
			{
				dx=4;
				player.curr_frame++;
				if(player.curr_frame<4||player.curr_frame>7) player.curr_frame=4;
				players_dir=0;
			}
			else if(keyboard_state[MAKE_UP])
			{
				dy=-4;
				player.curr_frame++;
				if(player.curr_frame<8||player.curr_frame>11) player.curr_frame=8;
				players_dir=2;
			}
			else if(keyboard_state[MAKE_LEFT])
			{
				dx=-4;
				player.curr_frame++;
				if(player.curr_frame<12||player.curr_frame>15) player.curr_frame=12;
				players_dir=1;
			}
			if(keyboard_state[MAKE_SPACE] && ammo)
			{
				if (sload[SHOT] && (currsnd>=SHOT || !Sound_Status()) )
				{
					Sound_Stop();
					Sound_Play((sound_ptr)&soundfx[SHOT]);
					currsnd=SHOT;
				}
				shoot_bullets(player.x+6,player.y+5,bullet_vel_x[players_dir],bullet_vel_y[players_dir],players_dir);
			}
		}
		//death sequence for player
		else if(player.state==SPRITE_DYING)
		{
			if(++player.curr_frame==22)
			{
				player.state=SPRITE_DEAD;
				Time_Delay(6);
				done=1;
			}
		}
		//move all game objects
		move_snow();
		move_bullets();
		move_boss();
		animate_explode();
		player.x+=dx;
		player.y+=dy;
		show_game_status();
		old_screen_x = screen_x;
		old_screen_y = screen_y;
		//see if there is a screen change
		if (player.x>320-16)
		{
			if (++screen_x>2)
			{
				screen_x=3;
				player.x=320-16;
			}
			else
			{
				screen_change=1;
				player.x=0;
			}
		  }
		  else if (player.x<0)
		  {
			if (--screen_x<0)
			{
				screen_x=0;
				player.x=0;
			}
			else
			{
				screen_change=1;
				player.x=320-16;
			}
		  }
		  if (player.y>160-16)
		  {
			if (++screen_y>2)
			{
				screen_y=3;
				player.y=160-16;
			}
			else
			{
				screen_change=1;
				player.y=0;
			}
		  }
		  else if (player.y<0)
		  {
		  if (--screen_y<0)
		  {
			screen_y=0;
			player.y=0;
		  }
		  else
		  {
			screen_change=1;
			player.y=160-16;
		  }
		}
		else
		{
			//check for players collision with a wall
			cell_x=(player.x+8)>>4;
			cell_y=(player.y+14)>>4;
			cell_id=world[screen_y][screen_x].cells[cell_y][cell_x];
			cell_bit=cell_id&cell_id;
			if(cell_bit==GREY_WALL||cell_bit==BLUE_WALL||cell_bit==BUSH)
			{
				player.x-=dx;
				player.y-=dy;
			}
			else
			{
				//items for player to pick up
				switch(cell_bit)
				{
					case TILE_HEALTH:
						if(health<100)
						{
							if (sload[HEALTH] && (currsnd>=HEALTH || !Sound_Status()) )
							{
								Sound_Stop();
								Sound_Play((sound_ptr)&soundfx[HEALTH]);
								currsnd=HEALTH;
							}
							health+=20;
							if(health>100) health=100;
							floors.x=cell_x<<4;
							floors.y=cell_y<<4;
							floors.curr_frame=0;
							Sprite_Draw((sprite_ptr)&floors,double_buffer,1);
							world[screen_y][screen_x].cells[cell_y][cell_x]=TILE_FLOOR;
						}
						break;
					case TILE_AMMO:
						if(ammo<300)
						{
							if (sload[AMMO] && (currsnd>=AMMO || !Sound_Status()) )
							{
								Sound_Stop();
								Sound_Play((sound_ptr)&soundfx[AMMO]);
								currsnd=AMMO;
							}
							ammo+=18;
							if(ammo>300) ammo=300;
							floors.x=cell_x<<4;
							floors.y=cell_y<<4;
							floors.curr_frame=0;
							Sprite_Draw((sprite_ptr)&floors,double_buffer,1);
							world[screen_y][screen_x].cells[cell_y][cell_x]=TILE_FLOOR;
						}
						break;
					case GRASS_HEALTH:
						if(health<100)
						{
							if (sload[HEALTH] && (currsnd>=HEALTH || !Sound_Status()) )
							{
								Sound_Stop();
								Sound_Play((sound_ptr)&soundfx[HEALTH]);
								currsnd=HEALTH;
							}
							health+=20;
							if(health>100) health=100;
							floors.x=cell_x<<4;
							floors.y=cell_y<<4;
							floors.curr_frame=3;
							Sprite_Draw((sprite_ptr)&floors,double_buffer,1);
							world[screen_y][screen_x].cells[cell_y][cell_x]=GRASS;
						}
						break;
					case GRASS_AMMO:
						if(ammo<300)
						{
							if (sload[AMMO] && (currsnd>=AMMO || !Sound_Status()) )
							{
								Sound_Stop();
								Sound_Play((sound_ptr)&soundfx[AMMO]);
								currsnd=AMMO;
							}
							ammo+=18;
							if(ammo>300) ammo=300;
							floors.x=cell_x<<4;
							floors.y=cell_y<<4;
							floors.curr_frame=3;
							Sprite_Draw((sprite_ptr)&floors,double_buffer,1);
							world[screen_y][screen_x].cells[cell_y][cell_x]=GRASS;
						}
						break;
					case HEALTH_GENERATOR:
						if(++health>100) health=100;
						break;
					case TILE_KEY:
						keys++;
						floors.x=cell_x<<4;
						floors.y=cell_y<<4;
						floors.curr_frame=0;
						Sprite_Draw((sprite_ptr)&floors,double_buffer,1);
						world[screen_y][screen_x].cells[cell_y][cell_x]=TILE_FLOOR;
						break;
					case GRASS_KEY:
						keys++;
						floors.x=cell_x<<4;
						floors.y=cell_y<<4;
						floors.curr_frame=3;
						Sprite_Draw((sprite_ptr)&floors,double_buffer,1);
						world[screen_y][screen_x].cells[cell_y][cell_x]=GRASS;
						break;
					case DOOR:
						if(keys)
						{
							keys--;
							floors.x=cell_x<<4;
							floors.y=cell_y<<4;
							floors.curr_frame=0;
							Sprite_Draw((sprite_ptr)&floors,double_buffer,1);
							world[screen_y][screen_x].cells[cell_y][cell_x]=TILE_FLOOR;
						}
						else
						{
							player.x-=dx;
							player.y-=dy;
						}
						break;

					default:break;
				}
			}
		}
		//if screen has change start every thing for new screen
		if(screen_change)
		{
			Fill_Double_Buffer(0);
			init_snow();
			init_bullets();
			draw_screen(screen_x,screen_y);
			init_boss(screen_x,screen_y);
		}
		//scan under all objects in the game
		under_bullets();
		under_boss();
		under_explode();
		Sprite_Under((sprite_ptr)&player,double_buffer);
		under_snow();
		//draw all objects in the game
		for (index=0;index<MAX_BULLETS;index++)
		{
			if (bullets[index].state==true)
			{
				if (boss.state != ZOMBIE_DEAD && boss.state != ZOMBIE_DYING &&
					bullets[index].object.x+3 > boss.x && bullets[index].object.x+3 < boss.x+32 &&
					bullets[index].object.y+3 > boss.y && bullets[index].object.y+3 < boss.y+32)
				{
					if (sload[ZOMBIE_HIT] && (currsnd>=ZOMBIE_HIT || !Sound_Status()) )
					{
						Sound_Stop();
						Sound_Play((sound_ptr)&soundfx[ZOMBIE_HIT]);
						currsnd=ZOMBIE_HIT;
					}
					zombies[index].object.x=boss.object.x+16;
					zombies[index].object.y=boss.object.y+16;
					init_explode(index);
					if(--boss.lives<=0) boss.state=ZOMBIE_DYING;
					else boss.state=ZOMBIE_CHASE;
					bullets[index].state=false;
					player_score+=50;
				}
			}
		}
		draw_bullets();
		if (draw_boss()) done=2;
		draw_explode();
		Sprite_Draw((sprite_ptr)&player,double_buffer,1);
		draw_snow();
		//copy all sprites to double buffer
		if(keyboard_state[MAKE_F1])
		{
			Music_Stop();
			Sprite_Under((sprite_ptr)&pause_pic,double_buffer);
			Sprite_Draw((sprite_ptr)&pause_pic,double_buffer,1);
			Display_Double_Buffer(double_buffer,0);
			while(!keyboard_state[MAKE_F2]);
			Sprite_Erase((sprite_ptr)&pause_pic,double_buffer);
			Music_Resume();
		}
		Display_Double_Buffer(double_buffer,0);
		if (Music_Status()==2 && mload)
		{
			Music_Stop();
			Music_Play((music_ptr)&song,0);
		}
		if (!Sound_Status()) currsnd=6;
		Time_Delay(1);
	}while(!exit_game&&player.state!=SPRITE_DEAD&&done==0);
	Keyboard_Remove_Driver(); //GETS RID OF KEYBOARD DRIVER
	Delete_Double_Buffer(); //DEALLOCATES MEMORY FOR DOUBLE BUFFER
	if (done==2)
	{
		Time_Delay(10);
		Screen_Transition(SCREEN_SWIPE_Y);
	}
	else if (done==0) Screen_Transition(SCREEN_DARKNESS); //FADES TO BLACK
	else Screen_Transition(SCREEN_WHITENESS); //FADES TO WHITE
	Sprite_Delete((sprite_ptr)&wall1); //DEALLOCATE ALL MEMORY FOR THE SPRITES
	Sprite_Delete((sprite_ptr)&wall2);
	Sprite_Delete((sprite_ptr)&player);
	Sprite_Delete((sprite_ptr)&floors);
	Sprite_Delete((sprite_ptr)&pause_pic);
	Sprite_Delete((sprite_ptr)&boss.object);
	for(index=0;index<MAX_BULLETS;index++) Sprite_Delete((sprite_ptr)&bullets[index].object);
	for(index=0;index<6;index++)
		if (sload[index]) Sound_Unload((sound_ptr)&soundfx[index]);
	if (mload)
	{
		Music_Stop();
		Music_Unload((music_ptr)&song);
	}
	return done;
}

