#include "Game.h"

#include "Common.h"
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

#include "ScreenshotManager.h"

#include <deque>
#include "Enemy.h"


#define DEBUG_DRAW 1
#define TARGET_FPS 60 // broken dont use this
#define CAP_FRAMERATE 0 // broken dont use this

bool Game::Init(int windowedWidth, int windowedHeight, bool fullscreen)
{
	srand(time(NULL));
	m_windowWidth = windowedWidth;
	m_windowHeight = windowedHeight;
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

		m_viewportWidth = m_windowWidth / 2;
		m_viewportHeight = m_windowHeight / 2;

		windowFlags |= SDL_WINDOW_BORDERLESS;
	}

	m_window = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_windowWidth, m_windowHeight, windowFlags);
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

	gDebugRenderer.Init();
	
	m_guiRenderer = new GuiRenderer();
	m_guiRenderer->Init();
	m_guiRenderer->SetProjection(m_viewportWidth, m_viewportHeight);

	gTextRenderer.Init();
	gTextRenderer.SetProjection(m_viewportWidth, m_viewportHeight);

	m_input = new Input();

	SDL_ShowCursor(SDL_DISABLE);

	return true;
}

void Game::Run()
{
	Create();

	Uint32 lastTime = SDL_GetTicks();
	std::deque<float> frameTimes;
	const size_t maxFrameSamples = 10;
	float frameTimeAccumulator = 0.0f;

	const float targetFrameTime = 1.0f / TARGET_FPS;

	// main loop
	bool running = true;
	while (running)
	{
		running = m_input->HandleEvents();
		if (m_input->IsKeyPressed(SDL_SCANCODE_ESCAPE)) running = false;
		HandleInput();
		m_input->Update();

		// TODO This sort of thing could be handled in a window class with events SDL_WINDOWEVENT_FOCUS_GAINED and SDL_WINDOWEVENT_FOCUS_LOST instead
		// so input shouldnt actually handle the event loop. a window class probably should instead
		// BUT.... its fine for now
		Uint32 windowFlags = SDL_GetWindowFlags(m_window);
		if (windowFlags & SDL_WINDOW_INPUT_FOCUS)
		{
			SDL_WarpMouseInWindow(m_window, m_windowWidth / 2, m_windowHeight / 2);
		}

		// calculate delta time
		Uint32 currentTime = SDL_GetTicks();
		float dt = (currentTime - lastTime) / 1000.0f;
		lastTime = currentTime;

		if (CAP_FRAMERATE && dt < targetFrameTime) // cap FPS
		{
			SDL_Delay(static_cast<Uint32>((targetFrameTime - dt) * 1000.0f)); // I dont think this works very well
			currentTime = SDL_GetTicks();
			dt = (currentTime - lastTime) / 1000.0f;
			lastTime = currentTime;
		}

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
		PhysicsUpdate(1.0f / 50.0f);
		gPhysicsWorld.StepSimulation(1.0f/ 50.0f, 16);
		Update(dt);

		std::string titleStr = "fps: " + std::to_string(fps);
		SDL_SetWindowTitle(m_window, titleStr.c_str());

		// render
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		Render();
		gPhysicsWorld.DebugDraw();
		m_renderer->Render(m_fpsController);

		if (DEBUG_DRAW)
		{
			gDebugRenderer.Render(m_fpsController);
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
	gTextureManager.LoadTexture("missing", "data/images/missing.png");

	//m_character = gPhysicsWorld.CreateCharacter();
	m_fpsController = new FirstPersonController(gPhysicsWorld.CreateCharacter(glm::vec3(0.0f, 5.0f, 0.0f)));
	m_renderer->SetProjection(m_fpsController->GetProjection(m_viewportWidth, m_viewportHeight));
	gDebugRenderer.SetProjection(m_fpsController->GetProjection(m_viewportWidth, m_viewportHeight));

	m_level = new Level();
	//m_renderer->AddToRenderList(m_level);

	gPhysicsWorld.CreateStaticLevelGeometry("data/models/level.obj");
	m_navGrid.Generate(&gPhysicsWorld);

	//m_player = new Player(glm::vec2(rand() % m_viewportWidth, rand() % m_viewportHeight), &m_projectiles);
	m_camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f));

	m_player = new Player();
	//m_renderer->AddToRenderList(m_player);

	Enemy* enemy = new Enemy(glm::vec3(0.0f, 10.0f, 0.0f));
	m_renderer->AddToRenderList(enemy);
	m_entities.push_back(enemy);
}

void Game::HandleInput()
{
	//m_player->HandleInput(m_input);

	//m_camera->HandleInput(m_input);
	m_fpsController->HandleInput(m_input);

	if (m_input->IsKeyPressed(SDL_SCANCODE_Z))
	{
		CatCube* catCube = gPhysicsWorld.AddCatCube(glm::vec3(0, 5.0f, 0));
		m_renderer->AddToRenderList(catCube);
	}

	if (m_input->IsMouseButtonPressed(SDL_BUTTON_LEFT))
	{
		gPhysicsWorld.RayCast(m_fpsController->GetPosition(), m_fpsController->GetFront());
	}

	if (m_input->IsKeyPressed(SDL_SCANCODE_0))
	{
		ScreenshotManager::TakeScreenshot(m_windowWidth, m_windowHeight);
	}

	if (m_input->IsKeyPressed(SDL_SCANCODE_N))
	{
		//m_navGrid.DebugDrawPath(m_navGrid.RandomPath());
	}
}

void Game::PhysicsUpdate(float dt)
{
	m_fpsController->PhysicsUpdate(dt);
}

void Game::Update(float dt)
{
	//m_camera->Update(dt);

	m_player->OnUpdate(dt);

	m_fpsController->Update(dt);

	for (Entity* entity : m_entities)
	{
		entity->OnUpdate(dt);
	}
}

void Game::Render()
{
	//gTextRenderer.AddStringToBatch("Hello World!!", 0.0f, 0.0f, glm::vec3(1.0f));
	//m_navGrid.DebugDraw();

	//m_navGrid.DebugDrawPath(
	//	m_navGrid.FindPath(m_navGrid.GetRandomStartNode(), m_navGrid.NodeClosestTo(m_fpsController->GetPosition()))
	//);
	
}

void Game::Destroy()
{
	SAFE_DELETE(m_player);
	SAFE_DELETE(m_camera);

	SAFE_DELETE(m_fpsController);
	SAFE_DELETE(m_level);
}

void Game::Cleanup()
{
	SAFE_DELETE(m_renderer);

	m_guiRenderer->Dispose();
	SAFE_DELETE(m_guiRenderer);
	
	SAFE_DELETE(m_input);

	gTextureManager.UnloadResources();

	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);

	SDL_Quit();
}
