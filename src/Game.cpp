#include "Common.h"
#include "Game.h"

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

#define DEBUG_DRAW 1
#define TARGET_FPS 60
#define CAP_FRAMERATE 1

bool Game::Init(int windowedWidth, int windowedHeight, bool fullscreen)
{
	srand((unsigned int)time(NULL)); // dont do this here. but whatever for now

	// Init global state
	WindowConfig config;
	gWindow.Init(config);

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
	Uint32 lastTime = SDL_GetTicks();
	std::deque<float> frameTimes;
	const size_t maxFrameSamples = 10;
	float frameTimeAccumulator = 0.0f;

	const float targetFrameTime = 1.0f / TARGET_FPS;

	const float physicsTimeStep = 1.0f / 60; // 60 Hz
	float accumulator = 0.0f;

	// main loop
	while (!gWindow.QuitRequested())
	{
		gWindow.PollEvents(); // I actually dont like the window class. TODO Get rid of

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

		// Update logic
		gSceneManager.Update(dt);
		if (gInput.IsKeyPressed(SDL_SCANCODE_ESCAPE))
		{
			gWindow.Quit();
		}

		if (gInput.IsKeyPressed(SDL_SCANCODE_F))
		{
			gWindow.ToggleFullscreen();
		}

		// Update input state
		gInput.Update();

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
}
