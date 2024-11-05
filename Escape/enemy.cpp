#include "enemy.h"
#include "sgg/graphics.h"
#include "config.h"
#include "util.h"
#include "game.h"
#include <iostream>

//enimerwsi enemy kai an efugan apo ta oria tis othonis diagrafoume
void Enemy::update()
{
	pos_x -= speed * graphics::getDeltaTime();
	rotation += 0.05f * graphics::getDeltaTime();
	rotation = fmodf(rotation, 360);
	if (pos_x < -size) {
		active = false;
	}

}

void Enemy::draw()
{
	if (life >= 0.5f) {
		graphics::setOrientation(rotation);
		br.texture = std::string(ASSET_PATH) + "planet.png";

		br.fill_opacity = 1.0f;
		graphics::drawRect(pos_x, pos_y, size, size, br);
	}
	else {
		graphics::setOrientation(rotation);
		br.texture = std::string(ASSET_PATH) + "cracked.png";

		br.fill_opacity = 1.0f;
		graphics::drawRect(pos_x, pos_y, size, size, br);

	}
	graphics::resetPose();

	if (game.getDebugMode())
	{
		graphics::Brush br;
		br.outline_opacity = 1.0f;
		br.texture = "";
		br.fill_color[0] = 1.3f;
		br.fill_color[1] = 0.3f;
		br.fill_color[2] = 0.3f;
		br.fill_opacity = 0.3f;
		br.gradient = false;

		Disk hull = getCollisionHull();
		graphics::drawDisk(hull.cx, hull.cy, hull.radius, br);

	}
}
void Enemy::init()
{
	speed = 0.1f;
	size = 50 + 100 * rand0to1();
	pos_x = CANVAS_WIDTH + 1.1f * size;
	pos_y = CANVAS_HEIGHT * rand0to1(); //gives planets randomly from 0 to 1 in the height of the canvas
	rotation = 360 * rand0to1();
	br.outline_opacity = 0.0f;
}


Enemy::Enemy(const Game& mygame) :GameObject(mygame)
{
	init();
}

Enemy::~Enemy()
{
}

Disk Enemy::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size * 0.40f;
	return disk;
}