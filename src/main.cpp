#define SDL_MAIN_HANDLED
#include "Game.h"

int main(int, char**) {
	pong::Game game;

	if (game.initialize()) {
		game.run_loop();
	}
	game.shutdown();

	return 0;
}