#ifndef CORE_HEADER_FILE
#define CORE_HEADER_FILE

#include <SDL2/SDL.h>

#include <Game.hpp>

class Core {
	friend class Game;

	private:
		static SDL_Window * w;
		static SDL_Renderer * r;

		static void Update();

	public:
		static void Initialize(const char * title, int width, int height);
		static void Terminate();

		static SDL_Renderer * GetSDLRenderer();
};

#endif