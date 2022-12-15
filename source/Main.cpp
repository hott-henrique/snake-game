#include <SDL2/SDL.h>

#include <memory>
#include <vector>
#include <chrono>
#include <thread>

#include <Vector2D.hpp>
#include <Core.hpp>
#include <Game.hpp>
#include <GameObject.hpp>

using IVector2D = Vector2D<int>;

#define GRID_SIZE 30
#define CELL_SIZE 20

class Apple: public GameObject {
	public:
		IVector2D position;

		Apple() {
			position.x = std::rand() % GRID_SIZE;
			position.y = std::rand() % GRID_SIZE;
		}

		void GetNewPosition() {
			auto lastPosition = position;
			do {
				position.x = std::rand() % GRID_SIZE;
				position.y = std::rand() % GRID_SIZE;
			} while (lastPosition == position);
		}

		void Render() {
			auto r = Core::GetSDLRenderer();

			SDL_Rect R = {
				.x = 0,
				.y = 0,
				.w = CELL_SIZE,
				.h = CELL_SIZE,
			};

			SDL_SetRenderDrawColor(r, 255, 70, 60, 0xFF);
			R.x = position.x * CELL_SIZE;
			R.y = position.y * CELL_SIZE;
			SDL_RenderFillRect(r, &R);
		}
};

class Snake: public GameObject {
	public:
		std::vector<IVector2D> segments;
		IVector2D direction;
		bool bHasChangedDirection;
		Apple * a = nullptr;

		Snake(Apple * a):
			direction(1, 0),
			bHasChangedDirection(false),
			a(a)
		{
			segments = {
				IVector2D(0, 0),
				IVector2D(1, 0),
				IVector2D(2, 0)
			};
		}

		void Render() {
			auto r = Core::GetSDLRenderer();

			SDL_Rect R = {
				.x = 0,
				.y = 0,
				.w = CELL_SIZE,
				.h = CELL_SIZE,
			};

			SDL_SetRenderDrawColor(r, 50, 105, 70, 0xFF);
			for (size_t i = 0; i < segments.size(); i++) {
				R.x = segments[i].x * CELL_SIZE;
				R.y = segments[i].y * CELL_SIZE;
				SDL_RenderFillRect(r, &R);
			}
		}

		void OnKeyDown(SDL_Keycode k) {
			if (bHasChangedDirection) return;
			switch (k) {
				case SDLK_w:
				case SDLK_UP:
					if (direction.y == 1) return;
					bHasChangedDirection = true;
					direction.x = 0;
					direction.y = -1;
				break;

				case SDLK_s:
				case SDLK_DOWN:
					if (direction.y == -1) return;
					bHasChangedDirection = true;
					direction.x = 0;
					direction.y = 1;
				break;

				case SDLK_d:
				case SDLK_RIGHT:
					if (direction.x == -1) return;
					bHasChangedDirection = true;
					direction.y = 0;
					direction.x = 1;
				break;

				case SDLK_a:
				case SDLK_LEFT:
					if (direction.x == 1) return;
					bHasChangedDirection = true;
					direction.y = 0;
					direction.x = -1;
				break;
			}
		}

		void Update() {
			bHasChangedDirection = false;
		}

		void PostUpdate() {
			/* Check end game conditions and update location. */
			int headIndex = segments.size() - 1;

			/* Go to next cell. Queue style: FIFO */
			segments.push_back(segments[headIndex] + direction);
			segments.erase(segments.begin());

			if (segments[headIndex] == a->position) {
				segments.emplace(segments.begin(), segments[0]);
				a->GetNewPosition();
				return;
			}

			/* Case an apple was eaten. */
			headIndex = segments.size() - 1;

			/* Check end game conditions: self-collision, wall collision */
			for (size_t i = 0; i < segments.size() - 1; ++i) {
				if (segments[headIndex] == segments[i]) {
					Game::FinishGame();
					break;
				}
			}

			if (
				segments[headIndex].x >= GRID_SIZE || segments[headIndex].x < 0 ||
				segments[headIndex].y >= GRID_SIZE || segments[headIndex].y < 0
			) {
				Game::FinishGame();
			}
		}
};

class Grid : public GameObject {
	public:
		void Render() {
			auto r = Core::GetSDLRenderer();

			/* Background */
			SDL_SetRenderDrawColor(r, 75, 155, 105, 0xFF);
			SDL_RenderClear(r);

			/* Grid */
			SDL_SetRenderDrawColor(r, 0, 0, 0, 0xFF);
			for (int i = 0; i < GRID_SIZE; ++i) {
				SDL_RenderDrawLine(r, i * CELL_SIZE, 0, i * CELL_SIZE, GRID_SIZE * CELL_SIZE);
				SDL_RenderDrawLine(r, 0, i * CELL_SIZE, GRID_SIZE * CELL_SIZE, i * CELL_SIZE);
			}
		}
};

int main(int argc, const char * argv[]) {
	std::srand(time(NULL));

	Core::Initialize("Snake", GRID_SIZE * CELL_SIZE, GRID_SIZE * CELL_SIZE);

	Apple * apple = new Apple();

	Game::AddGameObject(new Grid());
	Game::AddGameObject(apple);
	Game::AddGameObject(new Snake(apple));

	Game::RunLoop();

	Core::Terminate();

	return 0;
}
