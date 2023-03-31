#include "Enemy.h"

Enemy::Enemy(int player)
{
	srand((unsigned)time(0));
	int a = (rand() % (2 - 1 + 1)) + 1;

	HP = player * 2; //Monster life is double the EXP
	if (HP < 5)
		HP = 5;

	bool addOrsub = false;
	if (a == 1)
		addOrsub = true;
	else if (a == 2)
		addOrsub = false;

	if (addOrsub == true)
		HP -= a;
	else if (addOrsub == false)
		HP += a;
}

void Enemy::setHP(int attack)
{
	HP -= attack;
	if (HP <= 0)
		HP = 0;
}

int Enemy::attackMonster(int player)
{
	int attack = 0;
	srand((unsigned)time(0));

	if (player <= 6)
		attack = (rand() % (8 - 1 + 1)) + 1;

	else if (player > 6 && player <= 15)
		attack = (rand() % (12 - 7 + 1)) + 7;

	else if (player > 16)
		attack = (rand() % (20 - 12 + 1)) + 12;

	return attack;
}

int Enemy::attack(int a)
{
	return a + 2;
}