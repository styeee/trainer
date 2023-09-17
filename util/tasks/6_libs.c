#include <stdio.h>
#include <windows.h>

int main()
{
	printf("start\n");
	
	Beep(1000,1000);
	Beep(1100,1000);
	Beep(1200,1000);
	Beep(1300,1000);	
	Beep(1400,1000);	
	Beep(1500,1000);

	printf("end\n");
	
	return 0;
}