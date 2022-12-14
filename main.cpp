#include <SDL2/SDL.h>

#include <vector>
#include <chrono>
#include <thread>

struct Vector2D {
	int x, y;

	Vector2D(): x(0), y(0) { }
	Vector2D(int x, int y): x(x), y(y) { }

	Vector2D operator+(Vector2D const &V) {
		return Vector2D(x + V.x, y + V.y);
	}

	bool operator==(Vector2D const &V) {
		return (x == V.x) && (y == V.y);
	}
};

#define GRID_SIZE 15
#define CELL_SIZE 20

SDL_Window * w; SDL_Renderer * r;

bool bGameIsRunning = true;

std::vector<Vector2D> snake = {
	Vector2D(0, 0),
	Vector2D(1, 0),
	Vector2D(2, 0)
};
Vector2D snakeDirection(1, 0);

Vector2D applePosition;

void handleEvents();
void update();
void renderGame();

int main(int argc, const char * argv[]) {
	std::srand(time(NULL));

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fputs("Could not init SDL.", stderr);
		return EXIT_FAILURE;
	}

	w = SDL_CreateWindow(
		"Snake",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		GRID_SIZE * CELL_SIZE, GRID_SIZE * CELL_SIZE,
		SDL_WINDOW_SHOWN
	);

	if (!w) {
		fputs("Could not create window.\n", stderr);
		return EXIT_FAILURE;
	}

	r = SDL_CreateRenderer(w, -1, 0);

	if (!r) {
		fputs("Could not create renderer.\n", stderr);
		return EXIT_FAILURE;
	}

	applePosition.x = std::rand() % GRID_SIZE;
	applePosition.y = std::rand() % GRID_SIZE;

	while (bGameIsRunning) {
		renderGame();

		handleEvents();

		update();

		std::this_thread::sleep_for(std::chrono::milliseconds(150));
	}

	SDL_DestroyRenderer(r);
	SDL_DestroyWindow(w);

	SDL_Quit();

	return 0;
}

void handleEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT:
				bGameIsRunning = false;
				return;

			case SDL_KEYDOWN:
				switch (e.key.keysym.sym) {
					case SDLK_w:
					case SDLK_UP:
						if (snakeDirection.y == 1) continue;
						snakeDirection.x = 0;
						snakeDirection.y = -1;
					break;

					case SDLK_s:
					case SDLK_DOWN:
						if (snakeDirection.y == -1) continue;
						snakeDirection.x = 0;
						snakeDirection.y = 1;
					break;

					case SDLK_d:
					case SDLK_RIGHT:
						if (snakeDirection.x == -1) continue;
						snakeDirection.y = 0;
						snakeDirection.x = 1;
					break;

					case SDLK_a:
					case SDLK_LEFT:
						if (snakeDirection.x == 1) continue;
						snakeDirection.y = 0;
						snakeDirection.x = -1;
					break;
				}
			break;
		}
	}
}

void update() {
	int headIndex = snake.size() - 1;

	/* Go to next cell. Queue style: FIFO */

	snake.push_back(snake[headIndex] + snakeDirection);
	snake.erase(snake.begin());

	/* Check colision with app. */

	if (snake[headIndex] == applePosition) {
		snake.emplace(snake.begin(), snake[0]);

		auto backupApplePosition = applePosition;
		do {
			applePosition.x = std::rand() % GRID_SIZE;
			applePosition.y = std::rand() % GRID_SIZE;
		} while (backupApplePosition == applePosition);

		return;
	}

	/* Check end game conditions: self-collision, wall collision */

	for (int i = 0; i < snake.size() - 1; ++i) {
		if (snake[headIndex] == snake[i]) {
			bGameIsRunning = false;
			break;
		}
	}

	if (
		snake[headIndex].x >= GRID_SIZE || snake[headIndex].x < 0 ||
		snake[headIndex].y >= GRID_SIZE || snake[headIndex].y < 0
	) {
		bGameIsRunning = false;
	}
}

void renderGame() {
	/* Background */
	SDL_SetRenderDrawColor(r, 75, 155, 105, 0xFF);
	SDL_RenderClear(r);

	/* Grid */
	SDL_SetRenderDrawColor(r, 0, 0, 0, 0xFF);
	for (int i = 0; i < GRID_SIZE; ++i) {
		SDL_RenderDrawLine(r, i * CELL_SIZE, 0, i * CELL_SIZE, GRID_SIZE * CELL_SIZE);
		SDL_RenderDrawLine(r, 0, i * CELL_SIZE, GRID_SIZE * CELL_SIZE, i * CELL_SIZE);
	}

	SDL_Rect R = {
		.x = 0,
		.y = 0,
		.w = CELL_SIZE,
		.h = CELL_SIZE,
	};

	/* Apple */
	SDL_SetRenderDrawColor(r, 255, 70, 60, 0xFF);
	R.x = applePosition.x * CELL_SIZE;
	R.y = applePosition.y * CELL_SIZE;
	SDL_RenderFillRect(r, &R);

	/* Snake */
	SDL_SetRenderDrawColor(r, 50, 105, 70, 0xFF);
	for (int i = 0; i < snake.size(); i++) {
		R.x = snake[i].x * CELL_SIZE;
		R.y = snake[i].y * CELL_SIZE;
		SDL_RenderFillRect(r, &R);
	}

	SDL_RenderPresent(r);
}
