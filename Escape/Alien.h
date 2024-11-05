#pragma once
#include "enemy.h"


//dimiourgia Alien (enemy)
class Alien : public Enemy {

	float alien_life = 5.0f;
	bool chanceDir = true;
	Vector2D direction1 = Vector2D(-1.0f, 0.0f);

public:
	std::vector<Star*> S_stars;
	void AlienUpdateStars();
	float size = 0.0f;
	void update() override;
	void draw() override;
	void init() override;
	float getAlienLife() { return alien_life; }
	Disk getCollisionHull() const override;

	//zwi tou Alien
	void drainLife(float amount) {
		alien_life = std::max<float>(0.0f, alien_life - amount);
		if (alien_life == 0.0f)
		{
			setActive(false);

		}
	}
	Alien(const class Game& mygame);
	~Alien();

};