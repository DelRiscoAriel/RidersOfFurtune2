#pragma once

#ifndef ENEMY_H
#define ENEMY_H
#include <iostream>

using namespace std;

class Enemy
{
    private:
	    int HP = 0;

    public:
        Enemy(int);

        int getHP() { return HP; }
        void setHP(int);
        int attackMonster(int);
        virtual int attack(int);
};

#endif //!ENEMY_H