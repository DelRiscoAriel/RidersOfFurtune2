#include "Reaper.h"

Reaper::Reaper(int player) : Enemy(player)
{
	
}

int Reaper::attack(Player A)
{
	srand((unsigned)time(0));
	int resutl;
	if (A.getAlligment() == 0)
		resutl = A.getHP() / 2;
	else
	{
		int z = (rand() % (25 - 1 + 1)) + 1 * 0.01;
		resutl = A.getHP() * z;
	}
	return resutl;
}