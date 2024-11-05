#pragma once
#include "player.h"
#include "alien.h"


class Game
{
	typedef enum { START, PLAYING, LOSS, ALIEN, ENDING } status;

	Player* player = nullptr;
	Alien* alien = nullptr;
	float score = 0;
	bool player_initialized = false;
	bool alien_initialized = false;
	bool debug_mode = false;
	std::vector<Enemy*> enemies;

	void spawnEnemies();
	void updateEnemies();
	void checkPlayer();
	void checkActiveEnemies();
	bool checkCollision();
	bool checkCollisionAlien();
	status stat = START;
	void drawStartScreen();
	void drawLevelScreen();
	void drawAlienScreen();
	void drawLossScreen();
	void drawEndingScreen();
	void updateStartScreen();
	void updateLevelScreen();
	void updateAlienScreen();
	void updateLossScreen();
	void updateEndingScreen();
	void reset();
public:
	void setDebugMode(bool d) { debug_mode = d; }
	bool getDebugMode()const { return debug_mode; }
	void update();
	void draw();
	void init();



	Game();

	~Game();


};

