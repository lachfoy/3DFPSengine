#pragma once

#include <SDL.h>
#include <glad/glad.h>
#include <SDL_opengl.h>

class Renderer;

class Input;
class Player;
class Camera;

class Panel;
class Button;
class GuiRenderer;

#include "PhysicsWorld.h"

class Game
{
public:
	Game() {}
	bool Init(int width, int height, bool fullscreen, const char* title);
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
	Input* m_input;
	GuiRenderer* m_guiRenderer;

private:
	void HandleInput();
	void Update(float dt);
	void Create(); // scene related
	void Render(); // scene related
	void Destroy(); // scene related

	Player* m_player;

	Camera* m_camera;

	Panel* m_rootPanel;
	Panel* m_testPanel;
	Button* m_button1;
	Button* m_button2;
	Button* m_button3;

	PhysicsWorld m_physicsWorld;

	btRigidBody* m_testBox;


	std::vector<TestBox*> m_testBoxes;

};
