#pragma once
#include "gameobject.h"
#include "config.h"
#include "star.h"
#include "util.h"
#include "Vector.h"

class Player :public GameObject, public Collidable
{
	Vector2D direction = Vector2D(0.0f, -1.0f);
	float life = 1.0f;
	float orientation = 0.0f;
	float speed = 5.0f;
	float height = 1.0f;
	float pos_x = CANVAS_WIDTH / 2, pos_y = CANVAS_HEIGHT / 2;


public:
	std::vector<Star*> stars;
	Player(const class Game& mygame);
	void update() override;
	void updateStars();
	void draw()override;
	void init()override;
	float getRemainingLife() const { return life; }
	float getPosY() { return pos_y; }
	float getPosX() { return pos_x; }
	float getOri() { return orientation; }
	Disk getCollisionHull() const override;
	//zwi tou player
	void drainLife(float am) {
		life = std::max<float>(0.0f, life - am);
		graphics::playSound(std::string(ASSET_PATH) + "hit.mp3", 0.1f, false);

	}
};