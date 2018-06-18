#pragma once
#include "Graphics.h"

class Game
{
public:
    Game(HWND hWnd);
    ~Game();

    void Update();

private:
    Graphics graphics;
};
