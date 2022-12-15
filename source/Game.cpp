#include <Game.hpp>

#include <SDL2/SDL.h>
#include <Core.hpp>

#include <chrono>
#include <thread>

Game * Game::instance = nullptr;

void Game::Initialize() {
	if (Game::instance == NULL) {
		Game::instance = new Game();
	}
}

Game * Game::GetInstance() {
	return Game::instance;
}

void Game::RunLoop() {
	Game * g = GetInstance();

	g->bIsRunning = true;
	g->bIsFinished = false;

	while (!g->bIsFinished) {
		if (!g->bIsRunning) continue;

		g->Render();

		Core::Update();

		g->Update();

		g->PostUpdate();

		std::this_thread::sleep_for(std::chrono::milliseconds(150));
	}
}

void Game::SetIsRunning(bool state) {
	Game * g = Game::GetInstance();
	g->bIsRunning = state;
}

void Game::FinishGame() {
	Game * g = Game::GetInstance();
	g->bIsFinished = true;
}

void Game::AddGameObject(GameObject * go) {
	Game * g = Game::GetInstance();
	g->objects.push_back(go);
}

void Game::Render() {
	for (size_t i = 0; i < objects.size(); ++i) {
		objects[i]->Render();
	}

	SDL_RenderPresent(Core::GetSDLRenderer());
}

void Game::Update() {
	for (size_t i = 0; i < objects.size(); ++i) {
		objects[i]->Update();
	}
}

void Game::OnKeyDown(SDL_Keycode k) {
	Game * g = Game::GetInstance();
	for (size_t i = 0; i < g->objects.size(); ++i) {
		g->objects[i]->OnKeyDown(k);
	}
}

void Game::PostUpdate() {
	for (size_t i = 0; i < objects.size(); ++i) {
		objects[i]->PostUpdate();
	}
}

Game::~Game() {
	objects.clear();
}
