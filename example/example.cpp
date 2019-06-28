#include <stdio.h>
#include "CLogger.h"

int main()
{
	//printf("hello world!\n");
	for (int i = 0; i < 100; i++)
	{
		LOG_DEBUG << "hello world!";
	}
	
	return 0;
}
