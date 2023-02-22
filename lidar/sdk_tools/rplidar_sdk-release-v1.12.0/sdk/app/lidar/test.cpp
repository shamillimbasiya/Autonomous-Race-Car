#include "mainLoop.h"


int main()
{	
	MainLoop main = MainLoop();
	main.mainInit();
	while(true)
	{
		int status = main.run();
		
		if(status == -1){return -1;}
	}
}
