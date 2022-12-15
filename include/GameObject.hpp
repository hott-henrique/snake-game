#ifndef GAME_OBJECT_HEADER_FILE
#define GAME_OBJECT_HEADER_FILE

#include <SDL2/SDL.h>

class GameObject{
	public:

		virtual void Update();
		virtual void PostUpdate();
		virtual void Render();
		virtual void OnKeyDown(SDL_Keycode k);

};

#endif
