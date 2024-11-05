#pragma once
#include "gameobject.h"
#include "sgg/graphics.h"
#include "Vector.h"


class Star :public GameObject, public Collidable
{
	Vector2D direction;
	float pos_x = 0.0f;
	float pos_y = 0.0f;
	float speed = 8.0f;
	float size;
	graphics::Brush br;
	bool active = true;
public:
	void update() override;
	void draw() override;
	void init() override;

	Star(const class Game& mygame, float x, float y, float size, Vector2D init_dir);
	~Star();
	Disk getCollisionHull() const override;
	bool getActive() { return active; }
	void setActive(bool act) { active = act; }



};