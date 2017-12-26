#include "General.h"
#include "Game.h"
#include "Controller.h"
#include "View.h"
#include "Bot.h"
#include "dynamic.h"
#include <ctime> 

using namespace game_module;

int main() 
{
	std::srand(time(0));
	Game game(500, 29, 33, MAP_TYPE);
	game.start_game(true);
	return 0;
}
