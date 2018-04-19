#include "game.h"
#include <sys/time.h>

void AI_turn(int *circles, int &circles_count, int *crosses, int &crosses_count, bool hard, std::string AI_sign)
{
	if((crosses_count + circles_count) < 9)
	{
		if(!hard) /* Generate random field number */
		{
			bool AI_turn_done = false;
			while(AI_turn_done == false)
			{
				int field = 0;
				while(field == 0) /* Don't let the field be 0 */
				{
					struct timeval cur_time;
					gettimeofday(&cur_time, NULL);
					srand(cur_time.tv_usec);
					field = rand() % 10;
				}
		
				/* Copy field numbers into one array */
				/* Check if field isn't already used */
				if(!field_used(field, crosses, crosses_count) && !field_used(field, circles, circles_count))
				{
					if(AI_sign == "X")
					{
						crosses[crosses_count] = field;
						crosses_count++;
					}
					if(AI_sign == "O")
					{
						circles[circles_count] = field;
						circles_count++;
					}
					AI_turn_done = true;
				}
			}
		}
	}	
}