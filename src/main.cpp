#include "Game.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

constexpr int kScreenWidth = 800;
constexpr int kScreenHeight = 600;

int main(int argc, char* argv[])
{
	Game game;
	if (game.Init(kScreenWidth, kScreenHeight, false, "test"))
		game.Run();

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
	return 0;
}