#include "General.h"
#include "Game.h"
#include "Controller.h"
#include "View.h"


using namespace game_module;

int main() {


	Map my_map(80);

	print_map(my_map);

	system("pause > nul");
}
