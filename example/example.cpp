#include <stdio.h>
#include "CLogger.h"
#include <time.h>

int main()
{
	//printf("hello world!\n");

	CLogger::setLogFmtType(CLogger::LogFmtType::JSON);

	time_t now = time(nullptr);
	int i = 0;
	while(i<10000000)
	{
		LOG_DEBUG << "hello world!";
		i++;
	}
	time_t end = time(nullptr);
	int diff = end - now;
	
	std::cout << "diff:" << diff <<std::endl;

	return 0;
}
