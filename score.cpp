#include "game.h"
#include <iostream>
#include <string>
#include <fstream>
#include <errno.h>

void save_score(const char *player_name, double player_time, int player_turns)
{
	std::string outstr;
	outstr = player_name;
	outstr = outstr + " #";
//	char player_time_secs_str[2048];
//	sprintf(player_time_secs_str, "%'.3f", player_time);
//	outstr = outstr + player_time_secs_str;
//	outstr = outstr + " #";
	char num_player_turns_str[10];
	sprintf(num_player_turns_str, "%d", player_turns);
	outstr = outstr + num_player_turns_str;
	outstr = outstr + " #";
	char game_mode_str[5];
	std::fstream file;
	file.open("score.txt", std::ios::in | std::ios::out);
	if(!file.good()) /* Maybe file does not exist ? */
	{
		file.open("score.txt", std::ios::out);
		if(!file.good())
		{
			std::cout << "Could not write score.txt: " /*<< strerror(errno)*/ << std::endl;
		}
	}
	std::string linebuf; /* Append to file */
	std::string filebuf;
	/* Get file size */
	file.seekg(0, std::ios::end);
	ssize_t filesz = file.tellg();
	while(file.tellg() != filesz)
	{
		getline(file, linebuf);
		filebuf = filebuf + linebuf;
	}
	filebuf = filebuf + outstr;
	file << filebuf << std::endl;
	file.close();
}

