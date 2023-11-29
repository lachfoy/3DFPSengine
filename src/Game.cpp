#include "Game.h"

#include "Common.h"
#include "Renderer.h"
#include "DebugRenderer.h"
#include "Input.h"
#include "Player.h"
#include "Texture.h"
#include "Camera.h"

#include "GuiRenderer.h"
#include "ResourceManager.h"

#include "TextRenderer.h"

#include "ScreenshotManager.h"

#include <deque>
#include "Enemy.h"
#include "Sound.h"

#include "AudioEngine.h"
#include "StateManager.h"

#define DEBUG_DRAW 1
#define TARGET_FPS 60
#define CAP_FRAMERATE 1

bool Game::Init(int windowedWidth, int windowedHeight, bool fullscreen)
{
	srand((unsigned int)time(NULL)); // dont do this here. but whatever for now
	m_windowWidth = windowedWidth;
	m_windowHeight = windowedHeight;
	m_viewportWidth = m_windowWidth / 2; // for now
	m_viewportHeight = m_windowHeight / 2; // for now

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("An error occurred while initializing SDL2: %s\n", SDL_GetError());
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
		printf("An error occurred while creating an SDL2 window: %s\n", SDL_GetError());
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

	// Init global state
	m_renderer = new Renderer();
	m_renderer->Init();

	gDebugRenderer.Init();
	
	m_guiRenderer = new GuiRenderer();
	m_guiRenderer->Init();
	m_guiRenderer->SetProjection(m_viewportWidth, m_viewportHeight);

	gTextRenderer.Init();
	gTextRenderer.SetProjection(m_viewportWidth, m_viewportHeight);

	g_audioEngine.Init();

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

	const float physicsTimeStep = 1.0f / 60; // 60 Hz
	float accumulator = 0.0f;

	// main loop
	bool running = true;
	while (running)
	{
		running = Input::Instance().HandleEvents();
		if (Input::Instance().IsKeyPressed(SDL_SCANCODE_ESCAPE)) running = false;

		// TODO This sort of thing could be handled in a window class with events SDL_WINDOWEVENT_FOCUS_GAINED and SDL_WINDOWEVENT_FOCUS_LOST instead
		// so input shouldnt actually handle the event loop. a window class probably should instead
		Uint32 windowFlags = SDL_GetWindowFlags(m_window);
		if (windowFlags & SDL_WINDOW_INPUT_FOCUS)
		{
			// Warp the mouse to the center of the screen while the window has focus
			SDL_WarpMouseInWindow(m_window, m_windowWidth / 2, m_windowHeight / 2);
		}

		// calculate delta time
		Uint32 currentTime = SDL_GetTicks();
		float dt = (currentTime - lastTime) / 1000.0f;

		if (CAP_FRAMERATE && dt < targetFrameTime) // cap FPS
		{
			SDL_Delay(static_cast<Uint32>((targetFrameTime - dt) * 1000.0f)); // I dont think this works very well
			currentTime = SDL_GetTicks();
			dt = (currentTime - lastTime) / 1000.0f;
			lastTime = currentTime;
		}
		else
		{
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

		std::string titleStr = "fps: " + std::to_string(fps);
		SDL_SetWindowTitle(m_window, titleStr.c_str());

		// Accumulate time for physics update
		accumulator += dt;

		// Update physics at fixed intervals
		while (accumulator >= physicsTimeStep)
		{
			FixedUpdate();
			gPhysicsWorld.StepSimulation(physicsTimeStep, 16);
			accumulator -= physicsTimeStep;
		}

		// Update logic
		Update(dt);
		Input::Instance().Update();

		// Render
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		Render();
		gPhysicsWorld.DebugDraw();
		m_renderer->Render(m_player->GetCamera());

		if (DEBUG_DRAW)
		{
			gDebugRenderer.Render(m_player->GetCamera());
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

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Game::Create()
{
	ResourceManager::Instance().LoadResource<Texture>("data/images/round_cat.png", "cat");
	ResourceManager::Instance().LoadResource<Texture>("data/images/missing.png", "missing");
	ResourceManager::Instance().LoadSound("pew", "data/sounds/pew.wav");

	ResourceManager::Instance().GetSound("pew")->SetGain(1.0f);

	m_player = new Player(gPhysicsWorld.CreateCharacter(glm::vec3(0.f, 5.0f, 0.0f)));
	m_player->GetCamera()->UpdateProjection(static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight));

	m_level = new Level();
	m_renderer->AddToRenderList(m_level);

	gPhysicsWorld.CreateStaticLevelGeometry("data/models/test.obj");

	Enemy* enemy = new Enemy(glm::vec3(0.0f, 10.0f, 0.0f), m_player);
	m_renderer->AddToRenderList(enemy);
	m_entities.push_back(enemy);

	//StateManager::Instance().GoToState(new GameplayState());
}

void Game::FixedUpdate()
{
	m_player->FixedUpdate();

	for (Entity* entity : m_entities)
	{
		entity->FixedUpdate();
	}

	StateManager::Instance().FixedUpdate();
}

void Game::Update(float dt)
{
	if (Input::Instance().IsKeyPressed(SDL_SCANCODE_Z))
	{
		CatCube* catCube = gPhysicsWorld.AddCatCube(glm::vec3(0, 5.0f, 0)); // these never get deleted but thats ok ig
		m_renderer->AddToRenderList(catCube);
	}

	if (Input::Instance().IsKeyPressed(SDL_SCANCODE_0))
	{
		ScreenshotManager::TakeScreenshot(m_windowWidth, m_windowHeight);
	}

	if (Input::Instance().IsKeyPressed(SDL_SCANCODE_F))
	{
		// This just switches to fullscreen. But can't switch back (lol)
		// We need to save the windowed configuration to switch back to it.
		SDL_DisplayMode displayMode;
		SDL_GetDesktopDisplayMode(0, &displayMode); // can return int error
		
		m_windowWidth = displayMode.w;
		m_windowHeight = displayMode.h;

		m_viewportWidth = m_windowWidth / 2;
		m_viewportHeight = m_windowHeight / 2;

		SDL_SetWindowSize(m_window, m_windowWidth, m_windowHeight);
		SDL_SetWindowBordered(m_window, SDL_FALSE);

		glViewport(0, 0, m_windowWidth, m_windowHeight);

		m_player->GetCamera()->UpdateProjection(static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight));
		gTextRenderer.SetProjection(m_viewportWidth, m_viewportHeight);
	}

	m_player->Update(dt);

	for (Entity* entity : m_entities)
	{
		entity->Update(dt);
	}

	StateManager::Instance().Update(dt);
}

void Game::Render()
{
	StateManager::Instance().Render();
}

void Game::Destroy()
{
	for (Entity* entity : m_entities)
	{
		SAFE_DELETE(entity);
	}

	SAFE_DELETE(m_player);
	SAFE_DELETE(m_level);
}

void Game::Cleanup()
{
	ResourceManager::Instance().UnloadResources();

	g_audioEngine.Destroy();

	gTextRenderer.Dispose();

	m_guiRenderer->Dispose();
	SAFE_DELETE(m_guiRenderer);

	SAFE_DELETE(m_renderer);

	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);

	SDL_Quit();
}
