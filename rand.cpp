#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main()
{
	int i = 0;
	unsigned int r = 1211;
	while(i < 10)
	{
		srand(time(NULL));
		r = rand() % 10;
		std::cout << r << std::endl;
		i++;
		sleep(1);
	}
return 0;
}

