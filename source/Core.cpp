#include <Core.hpp>
#include <Game.hpp>

SDL_Window * Core::w = NULL;
SDL_Renderer * Core::r = NULL;

void Core::Initialize(const char * title, int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fputs("Could not init SDL.", stderr);
		exit(EXIT_FAILURE);
	}

	Core::w = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_SHOWN
	);

	if (!Core::w) {
		fputs("Could not create window.\n", stderr);
		exit(EXIT_FAILURE);
	}

	Core::r = SDL_CreateRenderer(Core::w, -1, 0);

	if (!Core::r) {
		fputs("Could not create renderer.\n", stderr);
		exit(EXIT_FAILURE);
	}

	Game::Initialize();
}

SDL_Renderer * Core::GetSDLRenderer() {
	return Core::r;
}

void Core::Terminate() {
	SDL_DestroyRenderer(Core::r);
	SDL_DestroyWindow(Core::w);

	delete Game::GetInstance();

	SDL_Quit();
}

void Core::Update() {
	SDL_Event e;

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT:
				Game::FinishGame();
				return;

			case SDL_KEYDOWN:
				Game::OnKeyDown(e.key.keysym.sym);
				// switch () {
				// 	case SDLK_w:
				// 	case SDLK_UP:
				// 		// Do something.
				// 	break;

				// 	case SDLK_s:
				// 	case SDLK_DOWN:
				// 		// Do something.
				// 	break;

				// 	case SDLK_d:
				// 	case SDLK_RIGHT:
				// 		// Do something.
				// 	break;

				// 	case SDLK_a:
				// 	case SDLK_LEFT:
				// 		// Do something.
				// 	break;
				// }
			break;
		}
	}

}