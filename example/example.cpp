#include <stdio.h>
#include "CLogger.h"
#include <time.h>

int main()
{
	//printf("hello world!\n");
	time_t now = time(nullptr);
	int i = 0;
	while(i<1000000)
	{
		LOG_DEBUG << "hello world!";
		i++;
	}
	time_t end = time(nullptr);
	int diff = end - now;
	
	std::cout << "diff:" << diff <<std::endl;

	time_t now1 = time(nullptr);

	int j = 0;
	while (j < 1000000)
	{
		j++;
	}
	time_t end1 = time(nullptr);
	int diff1 = end1 - now1;

	std::cout << "diff:" << diff << std::endl;

	return 0;
}
