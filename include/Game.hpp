#ifndef GAME_SINGLETON_HEADER_FILE
#define GAME_SINGLETON_HEADER_FILE

#include <vector>
#include <memory>

#include <GameObject.hpp>
#include <Core.hpp>


class Game {
	friend class Core;

	private:
		bool bIsRunning = false;
		bool bIsFinished = false;
		std::vector<GameObject *> objects;

		static Game * instance;

		static void Initialize();
		static Game * GetInstance();

	public:
		static void RunLoop();

		static void AddGameObject(GameObject * go);

		static void SetIsRunning(bool state);
		static void FinishGame();

		~Game();

	private:
		void Render();
		void Update();
		void PostUpdate();
		static void OnKeyDown(SDL_Keycode k);
};

#endif
