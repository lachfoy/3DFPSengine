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

//#include "NavGrid.h"

#include "PhysicsWorld.h"

#include "Player.h"

// TODO move this
#include <AL/al.h>
#include <AL/alc.h>

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

	// AL stuff
	// TODO move this
	ALCdevice* m_device;
	ALCcontext* m_alcontext;

private:
	void FixedUpdate();
	void Update(float dt);
	void Create(); // scene related
	void Render(); // scene related
	void Destroy(); // scene related

	Camera* m_debugCamera;

	Player* m_player;
	
	Level* m_level;

	//NavGrid m_navGrid; // Let's ignore complex pathfinding for now

	std::vector<Entity*> m_entities;

};
