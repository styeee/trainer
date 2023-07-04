#include <stdio.h>
#include <windows.h>

int main()
{
	label:

	printf("second passed\n");
	Sleep(1000);

	goto label;
	
	return 0;
}