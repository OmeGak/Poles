#ifndef POLESGAME_H
#define POLESGAME_H

#include "engine/Game.h"

class PolesGame : public Game
{
public:
    PolesGame();

    ~PolesGame();

    void start();
};

#endif // POLESGAME_H