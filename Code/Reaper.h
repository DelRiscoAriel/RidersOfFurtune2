#pragma once

#ifndef REAPER_H
#define REAPER_H
#include "Enemy.h"
#include "Yarra.h"

class Reaper : public Enemy
{
    public:
        Reaper(int);
        virtual int attack(Player);

};
#endif // !REAPER_H
