#include "stdafx.h"
#include "Game.h"


Game::Game(HWND hWnd) :
    graphics(hWnd)
{
    
}


Game::~Game()
{
}

void Game::Update()
{
    this->graphics.Update();
}
