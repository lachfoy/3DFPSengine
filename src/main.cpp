#include "Game.h"

int main(int argc, char* argv[])
{
	Game* game = new Game();
	if (game->Init(1280, 720, false))
		game->Run();

	delete game;
	game = nullptr;

	return 0;
}