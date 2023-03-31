#pragma once

#ifndef YARRA_H
#define YARRA_H
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Player
{
private:
	struct saveWeapons
	{
		string weaponName;
		int damage;
	};
	vector<saveWeapons> myCurrentWeapons;

	int numItem = 0;
	int sizeArrey = 5;
	string* item = new string[sizeArrey];

	int hp = 100;
	int exp = 0;
	int positionX = 0, positionY = 0;
	int turn = 0;
	string weaponName = "Knife";
	int damgeMod = 0;
	enum alligments { GOOD, BAD };
	int alligment = GOOD;

public:
	class WeaponLimit { }; //Exeption Class

	int getPositionX() { return positionX; }
	int getPositionY() { return positionY; }
	int getHP() { return hp; }
	int getEXP() { return exp; }
	int getTurn() { return turn; }
	int getAlligment() { return alligment; }

	//Items
	int getItem() { return numItem; }
	void addItem(string);
	void setArrey();
	string getItems(int a) { return item[a]; }
	void useItem(int);
	//------

	//Weapons
	vector<saveWeapons> getVector() { return myCurrentWeapons; }
	void setWeapons(vector<saveWeapons> a) { myCurrentWeapons = a; }
	void addWeapon(string, int);
	string getWeaponName() { return weaponName; }
	int getWeaponDamage() { return damgeMod; }
	void setEquipedWeapon(string, int);
	void findWeapon(string);
	void deleteWeapon(int);
	//-----

	void setPosition(int, int);
	void setHealth(int);
	void setEXP(int);
	void setTurns(int);
	void changeAlligment();
	void setAlligmnet(int);
};
#endif // !YARRA_H