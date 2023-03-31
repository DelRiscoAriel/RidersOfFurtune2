#include "Yarra.h"

void Player::setArrey()
{
	string* temp = new string[20];
	sizeArrey = sizeArrey + 5;

	for (int a = 0; a < numItem; a++)
		temp[a] = item[a];

	item = new string[sizeArrey];
	for (int a = 0; a < numItem; a++)
		item[a] = temp[a];
}

void Player::addItem(string potion)
{
	item[numItem] = potion;
	numItem++;
	if (numItem == sizeArrey)
		setArrey();
}

void Player::useItem(int choice)
{
	if (item[choice] == "PotionLv1")
		hp += 5;
	else if (item[choice] == "PotionLv2")
		hp += 10;

	if (hp > 100)
		hp = 100;

	//Delete the use Item
	for (int abc = choice; abc < numItem; abc++)
	{
		item[abc] = item[abc + 1];
	}
	numItem--;
}

void Player::setPosition(int x, int y)
{
	positionX = x;
	positionY = y;
}

void Player::setHealth(int paramHP)
{
	hp = paramHP;
}

void Player::setEXP(int e)
{
	exp += e;
}

void Player::setTurns(int t)
{
	turn += t;
}

void Player::addWeapon(string name, int damage)
{
	saveWeapons weapon;
	weapon.weaponName = name;
	weapon.damage = damage;

	myCurrentWeapons.push_back(weapon);
}

void Player::findWeapon(string weapon)
{
	for (int abc = 0; abc < myCurrentWeapons.size(); abc++)
	{
		if (myCurrentWeapons[abc].weaponName == weapon)
		{
			throw WeaponLimit();
		}
	}
}

void Player::deleteWeapon(int d)
{
	myCurrentWeapons.erase(myCurrentWeapons.begin() + d);
}

void Player::setEquipedWeapon(string a, int b)
{
	weaponName = a;
	damgeMod = b;
}

void Player::changeAlligment()
{
	if (alligment == GOOD)
		alligment = BAD;
	else
		alligment = GOOD;
}

void Player::setAlligmnet(int a)
{
	alligment = a;
}