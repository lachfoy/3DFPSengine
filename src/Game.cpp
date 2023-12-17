#include "Common.h"
#include "Game.h"

#include "Global.h"
#include "Renderer.h"
#include "DebugRenderer.h"
#include "Input.h"
#include "Camera.h"
#include "GuiRenderer.h"
#include "ResourceManager.h"
#include "TextRenderer.h"
#include "AudioEngine.h"
#include "PhysicsWorld.h"
#include "GameplayScene.h"

bool Game::Init(int windowedWidth, int windowedHeight, bool fullscreen)
{
	srand((unsigned int)time(NULL)); // dont do this here. but whatever for now

	// Init global state
	Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

	m_windowWidth = windowedWidth;
	m_windowHeight = windowedHeight;

	m_window = SDL_CreateWindow("game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_windowWidth, m_windowHeight, windowFlags);
	if (!m_window)
	{
		printf("An error occurred while creating an SDL2 window: %s\n", SDL_GetError());
		SDL_Quit();
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
	}

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		printf("GLAD could not be loaded\n");
		SDL_GL_DeleteContext(m_context);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
	}

	// Hide the cursor. I don't think we wanna expose this to the user
	SDL_ShowCursor(SDL_DISABLE);

	SetupGL();

	global.input = new Input(m_window);

	gRenderer.Init();
	gDebugRenderer.Init();

	m_guiRenderer = new GuiRenderer();
	m_guiRenderer->Init();
	m_guiRenderer->SetProjection(m_windowWidth / 2, m_windowHeight / 2);

	gTextRenderer.Init();
	gTextRenderer.SetProjection(m_windowWidth / 2, m_windowHeight / 2);

	gAudioEngine.Init();

	//gSceneManager.GoToScene(std::make_unique<GameplayScene>());

	m_scene = std::make_unique<GameplayScene>();
	m_scene->Create();

	return true;
}

void Game::Run()
{
	Uint64 lastCounter = SDL_GetPerformanceCounter();

	const float physicsTimeStep = 1.0f / 60; // 60 Hz
	float accumulator = 0.0f;

	// main loop
	while (!global.input->QuitRequested())
	{
		global.input->Update(); // Update input state

		// Calculate delta time
		Uint64 currentCounter = SDL_GetPerformanceCounter();
		float dt = float(currentCounter - lastCounter) / SDL_GetPerformanceFrequency();
		dt = std::min(dt, 0.25f);
		lastCounter = currentCounter;

		// Accumulate time for physics update
		accumulator += dt;

		// Update physics at fixed intervals
		while (accumulator >= physicsTimeStep)
		{
			m_scene->FixedUpdate();
			gPhysicsWorld.StepSimulation(physicsTimeStep, 16);
			accumulator -= physicsTimeStep;
		}

		// Update logic
		m_scene->Update(dt);

		if (global.input->KeyPressed(SDL_SCANCODE_ESCAPE)) // exit game
		{
			break;
		}

		// Alpha is a normalized range of how far to interpolate between physics states
		const float alpha = accumulator / dt;

		std::string debugString = "frametime: " + std::to_string(dt) + "\n";
		debugString += "alpha: " + std::to_string(alpha) + "\n";
		gTextRenderer.AddStringToBatch(debugString, 0, 0, glm::vec3(1.0f));

		// Render
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		m_scene->Render();
		gPhysicsWorld.DebugDraw();

		gDebugRenderer.PostRenderUpdate(dt);

		glDisable(GL_DEPTH_TEST);
		m_guiRenderer->RenderQuads();
		gTextRenderer.RenderQuads();
		glEnable(GL_DEPTH_TEST);

		// swap buffers
		SDL_GL_SwapWindow(m_window);
	}

	Cleanup();
}

void Game::SetupGL()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
}

void Game::Cleanup()
{
	gAudioEngine.Destroy();

	gTextRenderer.Dispose();

	m_guiRenderer->Dispose();
	SAFE_DELETE(m_guiRenderer);

	delete global.input;

	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);

	SDL_Quit();
}
