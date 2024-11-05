#pragma once
#include "gameobject.h"
#include "sgg/graphics.h"
#include "config.h"
#include "star.h"


class Enemy : public GameObject, public Collidable
{

public:
	float rotation;
	float size = 0.0f;
	float pos_x;
	float pos_y;
	float speed;
	float life = 1.0f;
	graphics::Brush br;
	bool active = true;
public:
	void update() override;
	void draw() override;
	void init() override;
	bool isActive() { return active; }
	void setActive(bool actv) {
		active = actv;
	}
	Enemy(const class Game& mygame);
	~Enemy();
	Disk getCollisionHull() const override;

	//zwi tou enemy
	void drainLife(float am) {
		life = std::max<float>(0.0f, life - am);
		if (life == 0.0f)
		{
			setActive(false);

		}

	}


};