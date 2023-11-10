#pragma once

#include <SDL.h>
#include <glad/glad.h>
#include <SDL_opengl.h>

#include <memory>
#include <vector>
#include "Enemy.h"
#include "Metal.h"
#include "Projectile.h"
#include "Turret.h"

class SpriteRenderer;

class Input;
class Player;
class Turret;
class TileMap;

class EnemySpawner;

class Panel;
class Button;
class GuiRenderer;

typedef std::vector<Enemy*> tEnemiesVec;
typedef std::vector<std::unique_ptr<Metal>> tMetalVec;
typedef std::vector<std::unique_ptr<Projectile>> tProjectilesVec;
typedef std::vector<Turret*> tTurretVec;

class Game
{
public:
	Game() {}
	bool Init(int width, int height, bool fullscreen, const char* title);
	void Run();

	const tEnemiesVec& GetEnemies() const { return m_enemies; }

private:
	void SetupGL();
	void Cleanup();

	SDL_Window* m_window;
	SDL_GLContext m_context;
	int m_windowWidth;
	int m_windowHeight;
	int m_viewportWidth;
	int m_viewportHeight;
	SpriteRenderer* m_renderer;
	Input* m_input;
	GuiRenderer* m_guiRenderer;

private:
	void HandleInput();
	void Update(float dt);
	void Create(); // scene related
	void Render(); // scene related
	void Destroy(); // scene related

	Player* m_player;
	tEnemiesVec m_enemies;
	tMetalVec m_metal;
	tTurretVec m_turrets;
	tProjectilesVec m_projectiles;

	EnemySpawner* m_enemySpawner;

	TileMap* m_tileMap;

	Panel* m_rootPanel;
	Panel* m_testPanel;
	Button* m_button1;
	Button* m_button2;
	Button* m_button3;

};
