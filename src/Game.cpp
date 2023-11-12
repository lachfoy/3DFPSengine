#include "Game.h"

#include "Renderer.h"
#include "DebugRenderer.h"
#include "Input.h"
#include <iostream>
#include "Player.h"
#include "Texture.h"
#include "Camera.h"

#include "Panel.h"
#include "Button.h"
#include "GuiRenderer.h"
#include "TextureManager.h"

#include "TextRenderer.h"

#include "World.h"

#include <deque>

#define DEBUG_DRAW 1

void RenderChildren(Panel* panel)
{
	if (panel)
	{
		//printf("Rendering %s...\n", panel->GetName());
		
		panel->Render();

		for (Panel* child : panel->GetChildren())
		{
			RenderChildren(child);
		}
	}
}

bool PropogateInput(Panel* panel, Input* input)
{
	bool inputHandled = false;

	if (panel)
	{
		if (panel->HandleInput(input))
		{
			inputHandled = true;
		}

		for (Panel* child : panel->GetChildren())
		{
			if (PropogateInput(child, input))
			{
				inputHandled = true;
				break;
			}
		}
	}

	return inputHandled;
}

bool Game::Init(int width, int height, bool fullscreen, const char* title)
{
	m_windowWidth = width;
	m_windowHeight = height;
	m_viewportWidth = m_windowWidth / 2; // for now
	m_viewportHeight = m_windowHeight / 2; // for now

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "An error occurred while initializing SDL2:" << SDL_GetError() << ".\n";
		return false;
	}

	Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	if (fullscreen)
	{
		//windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		SDL_DisplayMode displayMode;
		SDL_GetDesktopDisplayMode(0, &displayMode); // can return int error
		m_windowWidth = displayMode.w;
		m_windowHeight = displayMode.h;

		//m_viewportWidth = m_windowWidth;
		//m_viewportHeight = m_windowHeight;

		windowFlags |= SDL_WINDOW_BORDERLESS;
	}

	m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_windowWidth, m_windowHeight, windowFlags);
	if (!m_window)
	{
		std::cerr << "An error occurred while creating an SDL2 window:" << SDL_GetError() << ".\n";
		SDL_Quit();
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	m_context = SDL_GL_CreateContext(m_window);
	if (!m_context)
	{
		printf("GL Context could not be created: %s\n", SDL_GetError());
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		printf("GLAD could not be loaded\n");
		SDL_GL_DeleteContext(m_context);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}

	SetupGL();
	//SDL_GL_SetSwapInterval(0);

	// Init systems
	m_renderer = new Renderer();
	m_renderer->Init();
	m_renderer->SetProjection(m_viewportWidth, m_viewportHeight);

	gDebugRenderer.Init();
	gDebugRenderer.SetProjection(m_viewportWidth, m_viewportHeight);

	m_guiRenderer = new GuiRenderer();
	m_guiRenderer->Init();
	m_guiRenderer->SetProjection(m_viewportWidth, m_viewportHeight);

	gTextRenderer.Init();
	gTextRenderer.SetProjection(m_viewportWidth, m_viewportHeight);

	m_input = new Input();

	return true;
}

void Game::Run()
{
	Create();

	Uint32 last_time = SDL_GetTicks();
	std::deque<float> frameTimes;
	const size_t maxFrameSamples = 100;
	float frameTimeAccumulator = 0.0f;

	// main loop
	bool running = true;
	while (running)
	{
		running = m_input->HandleEvents();
		HandleInput();
		m_input->Update();

		// calculate delta time
		Uint32 current_time = SDL_GetTicks();
		float dt = (current_time - last_time) / 1000.0f;
		last_time = current_time;

		// maintain a fixed-size deque of frame times
		frameTimeAccumulator += dt;
		frameTimes.push_back(dt);
		if (frameTimes.size() > maxFrameSamples)
		{
			frameTimeAccumulator -= frameTimes.front();
			frameTimes.pop_front();
		}

		// calculate average frame time and FPS over the sliding window
		float averageFrameTime = frameTimeAccumulator / frameTimes.size();
		float fps = 1.0f / averageFrameTime;

		// update
		Update(dt);
		m_physicsWorld.stepSimulation(1.0f / 60.0f);

		std::string titleStr = "fps: " + std::to_string(fps);
		SDL_SetWindowTitle(m_window, titleStr.c_str());

		// render
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		Render();
		m_physicsWorld.DebugDraw();
		m_renderer->Render(m_camera);

		if (DEBUG_DRAW)
		{
			gDebugRenderer.Render(m_camera);
			gDebugRenderer.PostRenderUpdate(dt);
		}

		glDisable(GL_DEPTH_TEST);
		m_guiRenderer->RenderQuads();
		gTextRenderer.RenderQuads();
		glEnable(GL_DEPTH_TEST);

		// swap buffers
		SDL_GL_SwapWindow(m_window);
	}

	Destroy();
	Cleanup();
}

void Game::SetupGL()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	glViewport(0, 0, m_windowWidth, m_windowHeight);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Game::Create()
{
	gTextureManager.LoadTexture("cat", "data/images/round_cat.png");

	m_physicsWorld.CreateCharacter();

	//m_player = new Player(glm::vec2(rand() % m_viewportWidth, rand() % m_viewportHeight), &m_projectiles);
	m_camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f));

	m_player = new Player();
	//m_renderer->AddToRenderList(m_player);

	// GUI test stuff
	m_rootPanel = new Panel("Root", m_guiRenderer, glm::vec2(0, 0), glm::vec2(m_viewportWidth, m_viewportHeight));
	m_rootPanel->SetAlpha(0.0f);

	m_testPanel = new Panel("Panel", m_guiRenderer, glm::vec2(0, 0), glm::vec2(150, 170));
	m_rootPanel->AddChild(m_testPanel);
	m_testPanel->SetColor(glm::vec3(0.4, 0.4, 0.4));
	m_testPanel->CenterX();
	m_testPanel->CenterY();

	m_button1 = new Button("Button1", m_guiRenderer);
	m_testPanel->AddChild(m_button1);
	m_button1->SetPosition(glm::vec2(25, 20));
	m_button1->SetSize(glm::vec2(100, 30));
	m_button1->AddCallback([]()
		{
			printf("Button1 callback!\n");
		});


	m_button2 = new Button("Button2", m_guiRenderer);
	m_testPanel->AddChild(m_button2);
	m_button2->SetPosition(glm::vec2(25, 70));
	m_button2->SetSize(glm::vec2(100, 30));
	m_button2->AddCallback([]()
		{
			printf("Button2 callback!\n");
		});

	m_button3 = new Button("Button3", m_guiRenderer);
	m_testPanel->AddChild(m_button3);
	m_button3->SetPosition(glm::vec2(25, 120));
	m_button3->SetSize(glm::vec2(100, 30));
	m_button3->SetEnabled(false);
}

void Game::HandleInput()
{
	//m_player->HandleInput(m_input);

	m_camera->HandleInput(m_input);

	//PropogateInput(m_rootPanel, m_input);

	if (m_input->IsKeyPressed(SDL_SCANCODE_Z))
	{
		TestBox* testBox = m_physicsWorld.AddTestBox(glm::vec3(0, 5.0f, 0));
		m_renderer->AddToRenderList(testBox);
		m_testBoxes.push_back(testBox);
	}
}

void Game::Update(float dt)
{
	m_camera->Update(dt);

	m_player->OnUpdate(dt);

	for (const auto& testBox : m_testBoxes)
	{
		testBox->UpdateTransform();
	}
}

void Game::Render()
{


	//RenderChildren(m_rootPanel);

	//gTextRenderer.AddStringToBatch("Hello World!!", 0.0f, 0.0f, glm::vec3(1.0f));
}

void Game::Destroy()
{


	//delete m_player;
	//m_player = nullptr;

	delete m_rootPanel;
	delete m_testPanel;
	delete m_button1;
	delete m_button2;
	delete m_button3;
}

void Game::Cleanup()
{
	delete m_renderer;
	m_renderer = nullptr;

	m_guiRenderer->Dispose();
	delete m_guiRenderer;
	m_guiRenderer = nullptr;
	
	delete m_input;
	m_input = nullptr;

	gTextureManager.UnloadResources();

	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);

	SDL_Quit();
}
