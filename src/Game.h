#pragma once

#include <SDL.h>
#include <glad/glad.h>
#include <SDL_opengl.h>

class Renderer;

class Player;
class Camera;
class Entity;

class Panel;
class Button;
class GuiRenderer;

#include "PhysicsWorld.h"

#include "Player.h"

class Game
{
public:
	Game() {}
	bool Init(int windowedWidth, int windowedHeight, bool fullscreen);
	void Run();

private:
	void SetupGL();
	void Cleanup();

	SDL_Window* m_window;
	SDL_GLContext m_context;
	int m_windowWidth;
	int m_windowHeight;
	int m_viewportWidth;
	int m_viewportHeight;
	Renderer* m_renderer;
	GuiRenderer* m_guiRenderer;

private:
	void FixedUpdate();
	void Update(float dt);
	void Create(); // scene related
	void Render(); // scene related
	void Destroy(); // scene related

	Player* m_player;	
	Level* m_level;
	std::vector<Entity*> m_entities;

};
