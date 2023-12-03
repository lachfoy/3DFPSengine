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
#include "SceneManager.h"
#include "PhysicsWorld.h"
#include "GameplayScene.h"
#include "Window.h"

#include <deque>

bool Game::Init(int windowedWidth, int windowedHeight, bool fullscreen)
{
	srand((unsigned int)time(NULL)); // dont do this here. but whatever for now

	// Init global state
	WindowConfig config;
	gWindow.Init(config);

	global.input = new Input(gWindow.GetWindow());

	gRenderer.Init();
	gDebugRenderer.Init();

	m_guiRenderer = new GuiRenderer();
	m_guiRenderer->Init();
	m_guiRenderer->SetProjection(1280 / 2, 720 / 2);

	gTextRenderer.Init();
	gTextRenderer.SetProjection(1280 / 2, 720 / 2);

	gAudioEngine.Init();

	gSceneManager.GoToScene(std::make_unique<GameplayScene>());

	return true;
}

void Game::Run()
{
	std::deque<float> frameTimes;
	const size_t maxFrameSamples = 10;
	float frameTimeAccumulator = 0.0f;

	Uint64 lastCounter = SDL_GetPerformanceCounter();

	const float physicsTimeStep = 1.0f / 60; // 60 Hz
	float accumulator = 0.0f;

	// main loop
	while (!global.input->QuitRequested())
	{
		global.input->Update(); // Update input state
		gWindow.WarpMouseInWindow(); // I think ... maybe we delete the window class. what do you guys think?

		// Calculate delta time
		Uint64 currentCounter = SDL_GetPerformanceCounter();
		float dt = float(currentCounter - lastCounter) / SDL_GetPerformanceFrequency();
		dt = std::min(dt, 0.25f); // Clamp dt (apparently this is good practice)
		lastCounter = currentCounter;

		// Average the frame times for fps display
		frameTimeAccumulator += dt;
		frameTimes.push_back(dt);
		if (frameTimes.size() > maxFrameSamples)
		{
			frameTimeAccumulator -= frameTimes.front();
			frameTimes.pop_front();
		}

		float averageFrameTime = frameTimeAccumulator / frameTimes.size();
		float fps = 1.0f / averageFrameTime;

		std::string titleStr = "fps: " + std::to_string(fps);
		gWindow.SetTitle(titleStr.c_str());
		
		// Accumulate time for physics update
		accumulator += dt;

		// Update physics at fixed intervals
		while (accumulator >= physicsTimeStep)
		{
			gSceneManager.FixedUpdate();
			gPhysicsWorld.StepSimulation(physicsTimeStep, 16);
			accumulator -= physicsTimeStep;
		}

		gTextRenderer.AddStringToBatch("frametime: " + std::to_string(dt), 0, 0, glm::vec3(1.0f));

		// Update logic
		gSceneManager.Update(dt);
		if (global.input->KeyPressed(SDL_SCANCODE_ESCAPE)) // exit game
		{
			break;
		}

		// Render
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		gSceneManager.Render();
		gPhysicsWorld.DebugDraw();

		gDebugRenderer.PostRenderUpdate(dt);

		glDisable(GL_DEPTH_TEST);
		m_guiRenderer->RenderQuads();
		gTextRenderer.RenderQuads();
		glEnable(GL_DEPTH_TEST);

		// swap buffers
		gWindow.SwapBuffers();
	}

	Cleanup();
}

void Game::Cleanup()
{
	ResourceManager::Instance().UnloadResources();

	gAudioEngine.Destroy();

	gTextRenderer.Dispose();

	m_guiRenderer->Dispose();
	SAFE_DELETE(m_guiRenderer);

	delete global.input;
}
