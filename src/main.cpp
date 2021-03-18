#include "../inc/GameObj.hpp"

int main( int argc, char* args[] )
{
	GameObj game{};
	game.start();
	
	//Free resources and close SDL
	game.close();

	return 0;
}
