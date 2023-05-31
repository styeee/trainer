#include <windows.h>

#define WAIT_TIME 1000

typedef unsigned size_t;

inline void SetWindowSize(const short w,const short h)
{
	COORD coord;
    	coord.X=0;
    	coord.Y=0;
    	SMALL_RECT Rect;
    	Rect.Top=0;
    	Rect.Left=0;
    	Rect.Bottom=h;
    	Rect.Right=w;
    	const HANDLE Handle=GetStdHandle(STD_OUTPUT_HANDLE);
    	SetConsoleScreenBufferSize(Handle,coord);
    	SetConsoleWindowInfo(Handle,1,&Rect); 
}

unsigned x=1;
unsigned y=6;

#define w 16
#define h 8

#define wh w*h

char screen[]="#################  #    #i     ##    #  ##     ## ####  #      ##    #  #      ##### #  #      ##    #         #################";

HANDLE console;

const char skins[]={'>','^','<','v'};
char skin_id=0;
char skin='>';

void init()
{
	SetWindowSize(w-1,h-1);

	console=CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE,0,0,CONSOLE_TEXTMODE_BUFFER,0);
	SetConsoleActiveScreenBuffer(console);
	
	//for(size_t i=0;i<wh;i++)screen[i]=' ';

	screen[x%w+y*w]=skin;

	unsigned long count;
	COORD coord;
    	coord.X=0;
    	coord.Y=0;
	WriteConsoleOutputCharacterA(console,screen,wh,coord,&count);
}

inline void draw()
{
	unsigned long count;
	COORD coord;
    	coord.X=0;
    	coord.Y=0;
	WriteConsoleOutputCharacterA(console,screen,wh,coord,&count);
	Sleep(WAIT_TIME);
}

void act(char c)
{
	switch(c)
	{
		case '#':
			Sleep(WAIT_TIME);
			exit(1);
		break;
	}
}

static void right()
{
	screen[x++%w+y*w]=' ';
	act(screen[x%w+y*w]);
	screen[x%w+y*w]=skin;
	draw();
}

static void left()
{
	screen[x--%w+y*w]=' ';
	act(screen[x%w+y*w]);
	screen[x%w+y*w]=skin;
	draw();
}

static void up()
{
	screen[x%w+y--*w]=' ';
	act(screen[x%w+y*w]);
	screen[x%w+y*w]=skin;
	draw();
}

static void down()
{
	screen[x%w+y++*w]=' ';
	act(screen[x%w+y*w]);
	screen[x%w+y*w]=skin;
	draw();
}

void rotate()
{
	screen[x%w+y*w]=
		(skin=skins[skin_id=((skin_id+1)%4)]);
	draw();
}

void forward()
{
	switch(skin_id)
	{
		case 0:
			right();
		break;
		case 1:
			up();
		break;
		case 2:
			left();
		break;
		case 3:
			down();
		break;
		default:exit(2);
	}
}

char see()
{
	switch(skin_id)
	{
		case 0:return screen[(x+1)%w+y*w];
		case 1:return screen[x%w+(y-1)*w];
		case 2:return screen[(x-1)%w+y*w];
		case 3:return screen[x%w+(y+1)*w];
		default:exit(3);
	}
}