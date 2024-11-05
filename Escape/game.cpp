#include "game.h"
#include "sgg/graphics.h"
#include "config.h"
#include <iostream>


using namespace std;

//new
void Game::spawnEnemies()
{
	while (enemies.size() < MAX_ENEMY_COUNT) { //ligoteroi apo to max
		Enemy* e = new Enemy(*this);

		enemies.push_back(e);
	}
}


void Game::checkActiveEnemies() {

	bool activeEnemyStatus[MAX_ENEMY_COUNT];

	for (int i = 0; i < enemies.size(); i++) {
		activeEnemyStatus[i] = enemies[i]->isActive();
	}

	for (int i = enemies.size() - 1; i >= 0; i--) {
		if (!activeEnemyStatus[i]) {
			delete enemies[i];
			enemies.erase(enemies.begin() + i);
		}
	}

}

void Game::updateEnemies() {
	for (Enemy* e : enemies) {
		e->update();
	}


}

bool Game::checkCollision()
{
	if (!player)
	{
		return false;
	}


	Disk d1 = player->getCollisionHull();

	for (Enemy* e : enemies) {
		Disk d2 = e->getCollisionHull();
		float dx = d1.cx - d2.cx;
		float dy = d1.cy - d2.cy;
		if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
		{
			e->setActive(false);
			player->drainLife(0.1f);
			return true;
		}
	}


	for (Star* b : player->stars) {
		d1 = b->getCollisionHull();
		for (Enemy* e : enemies) {
			Disk d2 = e->getCollisionHull();
			float dx = d1.cx - d2.cx;
			float dy = d1.cy - d2.cy;
			if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
			{
				b->setActive(false);
				e->drainLife(0.1f);
				graphics::playSound(std::string(ASSET_PATH) + "hit.mp3", 0.1f, false);
				score += 125;
				return true;
			}
		}
	}

	return false;
}

bool Game::checkCollisionAlien()
{

	if (!player)
	{
		return false;
	}
	if (!alien)
	{
		return false;
	}


	Disk d1 = player->getCollisionHull();
	Disk d2 = alien->getCollisionHull();
	float dx = d1.cx - d2.cx;
	float dy = d1.cy - d2.cy;
	if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
	{

		player->drainLife(1.0f);
		return true;
	}

	for (Star* b : player->stars) {
		d1 = b->getCollisionHull();
		float dx = d1.cx - d2.cx;
		float dy = d1.cy - d2.cy;
		if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
		{
			alien->drainLife(0.01f);
			b->setActive(false);
			graphics::playSound(std::string(ASSET_PATH) + "explosion.mp3", 0.1f, false);
			score += 123;
			return true;
		}
	}

	d1 = player->getCollisionHull();
	for (Star* b_b : alien->S_stars) {
		d2 = b_b->getCollisionHull();
		float dx = d1.cx - d2.cx;
		float dy = d1.cy - d2.cy;
		if (sqrt(dx * dx + dy * dy) < d1.radius + d2.radius)
		{
			player->drainLife(0.1f);
			b_b->setActive(false);
			graphics::playSound(std::string(ASSET_PATH) + "hit.mp3", 0.1f, false);
			return true;
		}


	}


	return false;
}
void Game::checkPlayer()
{
	if (player->getRemainingLife() == 0.0f) {
		delete player;
		player = nullptr;
		stat = LOSS;
		graphics::playMusic(std::string(ASSET_PATH) + "LOSS.mp3", 0.5f, false);
	}
}
//reseting the game
void Game::reset()
{
	if (player) {
		delete player;
		player = nullptr;
		player_initialized = false;
	}


	score = 0.0f;


	if (alien) {
		alien_initialized = false;
		delete alien;
		alien = nullptr;
	}
	if (!alien) {
		alien_initialized = false;
	}
	graphics::setFont(std::string(ASSET_PATH) + "machine.ttf"); //SET FONT FOR RESET PAGE
	graphics::playMusic(std::string(ASSET_PATH) + "GAMEPLAY.mp3", 0.5f, false);

	for (Enemy* e : enemies) {
		e->setActive(false);
	}
	checkActiveEnemies();
}
//call reset
void Game::updateStartScreen()
{
	if (graphics::getKeyState(graphics::SCANCODE_RETURN)) {
		stat = PLAYING;

	}

}
void Game::updateLevelScreen()
{

	if (!player_initialized || player == nullptr)
	{
		player = new Player(*this);
		player_initialized = true;
	}
	else {
		player->update();
	}

	updateEnemies();
	checkActiveEnemies();

	checkCollision();
	spawnEnemies();
	checkPlayer();
	//next level
	if (score > 30000) {

		for (Enemy* e : enemies) {
			e->setActive(false);
		}
		checkActiveEnemies();

		graphics::playMusic(std::string(ASSET_PATH) + "DEFEAT_THE_ALIEN.mp3", 0.5f, false, 1000);

		stat = ALIEN;
	}

}
void Game::drawStartScreen()
{
	graphics::Brush br;
	br.outline_opacity = 0.0f;
	br.texture = std::string(ASSET_PATH) + "keys.png";
	graphics::drawRect(CANVAS_WIDTH / 4, CANVAS_HEIGHT / 2, 300, 250, br);
	br.texture = std::string(ASSET_PATH) + "alien_start.png";
	graphics::drawRect(CANVAS_WIDTH - 300, CANVAS_HEIGHT / 2, 250, 250, br);
	char info[40];
	sprintf_s(info, "PRESS ENTER TO CONTINUE");
	graphics::drawText(CANVAS_WIDTH / 2, CANVAS_HEIGHT - 100, 30, info, br);
	sprintf_s(info, "ESCAPE THE ALIEN");
	graphics::drawText(275, 100, 40, info, br);
	sprintf_s(info, "Press 'LEFT' - 'RIGHT' TO ROTATE");
	graphics::drawText(CANVAS_WIDTH / 4 - 200, CANVAS_HEIGHT / 2 + 150, 25, info, br);
	sprintf_s(info, "Press 'UP' - 'DOWN' TO MOVE");
	graphics::drawText(CANVAS_WIDTH / 4 - 200, CANVAS_HEIGHT / 2 + 200, 25, info, br);
}
void Game::drawLevelScreen()
{
	graphics::Brush br;
	br.outline_opacity = 0.0f;
	br.texture = std::string(ASSET_PATH) + "background.png";
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

	if (player) {
		player->draw();
	}

	for (Enemy* e : enemies) {
		e->draw();
	}



	if (player && debug_mode) {
		char info[40];
		sprintf_s(info, "(%6.0f,%6.2f,%6.2f)", player->getPosX(), player->getPosY(), player->getOri());
		graphics::drawText(50, 50, 40, info, br);

	}
	char score_c[40];
	sprintf_s(score_c, "Score:%6.0f/30000", score);
	graphics::drawText(30.0f, 17.0f, 20, score_c, br);




}
void Game::updateAlienScreen()
{
	if (!alien_initialized) {
		alien = new Alien(*this);
		alien_initialized = true;
		alien->setActive(true);
	}

	if (player) {
		player->update();

	}

	if (alien) {
		alien->update();
	}

	checkCollisionAlien();

	if (!alien->isActive()) {
		delete alien;
		alien = nullptr;
		graphics::playMusic(std::string(ASSET_PATH) + "VICTORY.mp3", 0.5f, false, 1000);

		stat = ENDING;
	}

	checkPlayer();
}

void Game::drawAlienScreen()
{
	graphics::Brush br;

	br.outline_opacity = 0.0f;
	br.texture = std::string(ASSET_PATH) + "background.png";
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

	char score_c[40];
	sprintf_s(score_c, "Score:%6.0f", score);
	graphics::drawText(30.0f, 10.0f, 27, score_c, br);

	if (alien)
		alien->draw();
	if (player) {
		player->draw();
	}
}
//reset button
void Game::updateLossScreen()
{
	if (graphics::getKeyState(graphics::SCANCODE_R)) {
		stat = START;
		reset();

	}


}

void Game::drawLossScreen()
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "wasted.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
}

void Game::updateEndingScreen()
{
	if (graphics::getKeyState(graphics::SCANCODE_R)) {
		reset();

		stat = START;
	}

}

void Game::drawEndingScreen()
{
	graphics::Brush br;
	graphics::setFont(std::string(ASSET_PATH) + "machine.ttf");

	char info[40];
	sprintf_s(info, "Victory!");
	graphics::drawText(400, 80, 50, info, br);
	sprintf_s(info, "Thank you For Playing!");
	graphics::drawText(300, 200, 30, info, br);
	br.texture = std::string(ASSET_PATH) + "background.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);
}

void Game::update()
{


	switch (stat) {
	case START:
		updateStartScreen();
		break;
	case PLAYING:
		updateLevelScreen();
		break;
	case LOSS:
		updateLossScreen();

		break;
	case ALIEN:
		updateAlienScreen();

		break;
	case ENDING:
		updateEndingScreen();

		break;
	default:
		cout << "No status" << endl;

	}
}
//call draw depending on status
void Game::draw()
{

	switch (stat) {
	case START:
		drawStartScreen();
		break;
	case PLAYING:
		drawLevelScreen();
		break;
	case LOSS:
		drawLossScreen();
		break;
	case ALIEN:
		drawAlienScreen();
		break;
	case ENDING:
		drawEndingScreen();
		break;
	default:
		cout << "No status" << endl;

	}

}

void Game::init()
{
	graphics::playMusic(std::string(ASSET_PATH) + "GAMEPLAY.mp3", 0.5f, true, 4000);

	graphics::setFont(std::string(ASSET_PATH) + "machine.ttf");
}

Game::Game()
{
	enemies = vector<Enemy*>();

}
Game::~Game()
{
	if (player)
		delete player;

	//Delete enemies
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i] != nullptr)
			delete enemies[i];
	}

	enemies.clear();

}
