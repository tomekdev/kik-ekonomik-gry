#include "game.h"

coords2d fieldtocoords2d(int fld)
{
	coords2d buf;
//	errno = 0; /* Clear errno we want only know the errno set by strtol() */
//	int field = strtol(str.c_str(), NULL, 10);
//	if(errno != 0)
//	{
//		buf.x = -1;
//		buf.y = -1;
//	}
//	else
//	{
//		switch(field)
		switch(fld)
		{
			case 1: buf.x = 0; buf.y = 2; break;
			case 2: buf.x = 1; buf.y = 2; break;
			case 3: buf.x = 2; buf.y = 2; break;
			case 4: buf.x = 0; buf.y = 1; break;
			case 5: buf.x = 1; buf.y = 1; break;
			case 6: buf.x = 2; buf.y = 1; break;
			case 7: buf.x = 0; buf.y = 0; break;
			case 8: buf.x = 1; buf.y = 0; break;
			case 9: buf.x = 2; buf.y = 0; break;
			default: buf.x = -2; buf.y = -2; break;
		}
//	}
	return buf;
}


bool field_used(int field, int *fields_array, int arraysz)
{
	int i = 0;
	while(i < arraysz)
	{
		if(fields_array[i] == field)
		{
			return true;
		}
		i++;
	}
	return false;
}

bool if_array_has_value(int *array, int size, int value)
{
	int i = 0;
	while(i < size)
	{
		if(array[i] == value)
		{
			return true;
		}
		i++;
	}
	return false;
}

int who_won(int *crosses_array, int crosses_count, int *circles_array, int circles_count, std::string AI_sign, std::string player1_sgn) /* Returns 0 when player won, 1 when AI and -1 when no one */
{																	/* Or 0 when player 1 won, 1 when player 2 if arg AI_sign was "NaN" */
	int cross_value = -1;
	int circle_value = -1;

	if(AI_sign == "NaN")
	{
		if(player1_sgn == "X")
		{
			cross_value = 0;
			circle_value = 1;
		}
		else
		{
			cross_value = 1;
			circle_value = 0;
		}
	}

	if(AI_sign == "X")
	{
		cross_value = 1;
		circle_value = 0;
	}
	if(AI_sign == "O")
	{
		cross_value = 0;
		circle_value = 1;
	}

	/* Check if there are three crosses across or vertically or horizontally */
	if(if_array_has_value(crosses_array, crosses_count, 1) &&
	   if_array_has_value(crosses_array, crosses_count, 4) &&
	   if_array_has_value(crosses_array, crosses_count, 7) )
	{
		return cross_value; /* Crosses won */
	}
	if(if_array_has_value(crosses_array, crosses_count, 1) &&
	   if_array_has_value(crosses_array, crosses_count, 5) &&
	   if_array_has_value(crosses_array, crosses_count, 9) )
	{
		return cross_value; /* Crosses won */
	}
	if(if_array_has_value(crosses_array, crosses_count, 1) &&
	   if_array_has_value(crosses_array, crosses_count, 2) &&
	   if_array_has_value(crosses_array, crosses_count, 3) )
	{
		return cross_value; /* Crosses won */
	}
	if(if_array_has_value(crosses_array, crosses_count, 7) &&
	   if_array_has_value(crosses_array, crosses_count, 8) &&
	   if_array_has_value(crosses_array, crosses_count, 9) )
	{
		return cross_value; /* Crosses won */
	}
	if(if_array_has_value(crosses_array, crosses_count, 9) &&
	   if_array_has_value(crosses_array, crosses_count, 6) &&
	   if_array_has_value(crosses_array, crosses_count, 3) )
	{
		return cross_value; /* Crosses won */
	}
	if(if_array_has_value(crosses_array, crosses_count, 2) &&
	   if_array_has_value(crosses_array, crosses_count, 5) &&
	   if_array_has_value(crosses_array, crosses_count, 8) )
	{
		return cross_value; /* Crosses won */
	}
	if(if_array_has_value(crosses_array, crosses_count, 4) &&
	   if_array_has_value(crosses_array, crosses_count, 5) &&
	   if_array_has_value(crosses_array, crosses_count, 6) )
	{
		return cross_value; /* Crosses won */
	}
	if(if_array_has_value(crosses_array, crosses_count, 7) &&
	   if_array_has_value(crosses_array, crosses_count, 5) &&
	   if_array_has_value(crosses_array, crosses_count, 3) )
	{
		return cross_value; /* Crosses won */
	}
	/* Check if there are three circles across or vertically or horizontally */
	if(if_array_has_value(circles_array, circles_count, 1) &&
	   if_array_has_value(circles_array, circles_count, 4) &&
	   if_array_has_value(circles_array, circles_count, 7) )
	{
		return circle_value; /* circles won */
	}
	if(if_array_has_value(circles_array, circles_count, 1) &&
	   if_array_has_value(circles_array, circles_count, 5) &&
	   if_array_has_value(circles_array, circles_count, 9) )
	{
		return circle_value; /* circles won */
	}
	if(if_array_has_value(circles_array, circles_count, 1) &&
	   if_array_has_value(circles_array, circles_count, 2) &&
	   if_array_has_value(circles_array, circles_count, 3) )
	{
		return circle_value; /* circles won */
	}
	if(if_array_has_value(circles_array, circles_count, 7) &&
	   if_array_has_value(circles_array, circles_count, 8) &&
	   if_array_has_value(circles_array, circles_count, 9) )
	{
		return circle_value; /* circles won */
	}
	if(if_array_has_value(circles_array, circles_count, 9) &&
	   if_array_has_value(circles_array, circles_count, 6) &&
	   if_array_has_value(circles_array, circles_count, 3) )
	{
		return circle_value; /* circles won */
	}
	if(if_array_has_value(circles_array, circles_count, 2) &&
	   if_array_has_value(circles_array, circles_count, 5) &&
	   if_array_has_value(circles_array, circles_count, 8) )
	{
		return circle_value; /* circles won */
	}
	if(if_array_has_value(circles_array, circles_count, 4) &&
	   if_array_has_value(circles_array, circles_count, 5) &&
	   if_array_has_value(circles_array, circles_count, 6) )
	{
		return circle_value; /* circles won */
	}
	if(if_array_has_value(circles_array, circles_count, 7) &&
	   if_array_has_value(circles_array, circles_count, 5) &&
	   if_array_has_value(circles_array, circles_count, 3) )
	{
		return circle_value; /* circles won */
	}

	return -1;
}

int which_field_player_clicked(int posx, int posy)
{
	/* Fields' dimensions and positions */
	SDL_Rect fields[9];
	fields[6].x = 0.125 * windowx;
	fields[6].y = 0.064 * windowy;
	fields[6].h = windowy / 4;
	fields[6].w = windowx / 4;

	fields[7].x = 0.125 * windowx + fields[6].w;
	fields[7].y = 0.064 * windowy;
	fields[7].w = windowx / 4;
	fields[7].h = windowy / 4;

	fields[8].x = 0.125 * windowx + fields[6].w * 2;
	fields[8].y = 0.064 * windowy;
	fields[8].w = windowx / 4;
	fields[8].h = windowx / 4;

	fields[3].x = 0.125 * windowx;
	fields[3].y = 0.064 * windowy + fields[6].h;
	fields[3].h = windowy / 4;
	fields[3].w = windowx / 4;

	fields[4].x = 0.125 * windowx + fields[6].w;
	fields[4].y = 0.064 * windowy + fields[6].h;
	fields[4].h = windowy / 4;
	fields[4].w = windowx / 4;

	fields[5].x = 0.125 * windowx + fields[6].w * 2;
	fields[5].y = 0.064 * windowy + fields[6].h;
	fields[5].h = windowy / 4;
	fields[5].w = windowx / 4;

	fields[0].x = 0.125 * windowx;
	fields[0].y = 0.064 * windowy + fields[6].h * 2;
	fields[0].h = windowy / 4;
	fields[0].w = windowx / 4;

	fields[1].x = 0.125 * windowx + fields[6].w;
	fields[1].y = 0.064 * windowy + fields[6].h * 2;
	fields[1].h = windowy / 4;
	fields[1].w = windowx / 4;

	fields[2].x = 0.125 * windowx + fields[6].w * 2;
	fields[2].y = 0.064 * windowy + fields[6].h * 2;
	fields[2].h = windowy / 4;
	fields[2].w = windowx / 4;

	int field_no = -1; /* By default clicked outside any field */
	int i = 0; /* Check for all fields */
	while(i < 9 && field_no == -1)
	{
		if(posx > fields[i].x && posx < (fields[i].x + fields[i].w) && posy > fields[i].y && posy < (fields[i].y + fields[i].h))
		{
			field_no = i + 1;
		}
		i++;
	}
	return field_no;
}

