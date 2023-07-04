#include <winsock.h>
#include <windows.h>
#include <time.h>

unsigned int WAIT_TIME=1000;

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

static char remote_mode=0;

#define start_x 1
#define start_y 6

unsigned x=start_x;
unsigned y=start_y;

#define w 16
#define h 8

#define wh w*h

char screen[]="#################  #    #i     ##    #  ##     ## ####  #      ##    #  #      ##### #  #      ##    #         #################";

HANDLE console;

const char skins[]={'>','^','<','v'};
char skin_id=0;
char skin='>';

static HANDLE console_orig;
static HANDLE console;

void init()
{
	srand(time(0));
	SetWindowSize(w-1,h-1);

	console_orig=GetStdHandle(STD_OUTPUT_HANDLE);
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
			//Sleep(WAIT_TIME);
			//exit(3);
			
			Sleep(WAIT_TIME);
			screen[x%w+y*w]='X';
			draw();
			Sleep(WAIT_TIME*2);
			
			screen[x%w+y*w]='#';
			screen[(x=start_x)%w+(y=start_y)*w]=skin='>';
			Sleep(WAIT_TIME*2);
		break;
		case 'i':
			{
				unsigned pos;			

				do
				{
					pos=(rand()%w)+(rand()%h)*w;
				}
				while(screen[pos]!=' ');
				screen[pos]='i';
			}
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

#include <stdio.h>
void echo(const char*format,...)
{
	SetConsoleActiveScreenBuffer(console_orig);
	va_list argptr;
    	va_start(argptr, format);
    	vfprintf(stderr, format, argptr);
    	va_end(argptr);
	Sleep(WAIT_TIME);
	system("cls");
	SetConsoleActiveScreenBuffer(console);
}

void remote(unsigned short port)
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET listener;
    listener = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    bind(listener, (struct sockaddr*)&server, sizeof(server));

    listen(listener, 1);

    int c=sizeof(struct sockaddr_in);
    struct sockaddr_in client;
    SOCKET socket = accept(listener, (struct sockaddr*)&client, &c);

#define text_info "(m - for change mode)\n\nmode 1:\n\n0 - rotate\n1 - forward\n\nmode 2:\n\n00 - down\n01 - up\n10 - left\n11 - right\n" 
    send(socket,text_info,sizeof(text_info)+1, 0);

    char act;
    while(1)
    {
        recv(socket,&act,1,0);
        switch(act)
        {
        	case '0':
			if(remote_mode)
			{
				recv(socket,&act,1,0);
				if(act=='0'){down();skin='v';}
				else if(act=='1'){up();skin='^';}
			}
			else rotate();
        	break;
        	case '1':
			if(remote_mode)
			{
				recv(socket,&act,1,0);
				if(act=='0'){left();skin='<';}
				else if(act=='1'){right();skin='>';}
			}
            		else forward();
       		break;
		case 'm':
			remote_mode=!remote_mode;
		break;
		case 't':
			echo("test");
		break;
		case 'w':
			Sleep(WAIT_TIME);
		break;
		case 'e':
			exit(4);
        }
    }

    closesocket(listener);
    WSACleanup();
}