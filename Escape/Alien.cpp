#include "Alien.h"
#include "sgg/graphics.h"
#include "config.h"
#include "util.h"
#include "game.h"
#include <iostream>


Alien::Alien(const Game& mygame) : Enemy(mygame)
{
	init();
}

Alien::~Alien()
{

}

void Alien::update()
{
	if (chanceDir)
	{
		pos_y -= speed * graphics::getDeltaTime();
		if (pos_y < 20.0f)
			chanceDir = false;
	}
	else {
		pos_y += speed * graphics::getDeltaTime();
		if (pos_y > 500.0f)
			chanceDir = true;
	}
	if (S_stars.size() < 1) {
		Star* b_b = new Star(game, pos_x, pos_y, 40, direction1);
		S_stars.push_back(b_b);

	}

	AlienUpdateStars();

}

void Alien::draw()
{
	graphics::Brush br;

	br.texture = std::string(ASSET_PATH) + "TheAlien.png";

	br.fill_opacity = 1.0f;
	br.outline_opacity = 0.0f;
	graphics::drawRect(pos_x, pos_y, size, size, br);
	graphics::resetPose();

	if (game.getDebugMode())
	{
		graphics::Brush br;
		br.texture = "";
		br.fill_color[0] = 1.3f;
		br.fill_color[1] = 0.3f;
		br.fill_color[2] = 0.3f;
		br.fill_opacity = 0.3f;
		br.gradient = false;

		Disk hull = getCollisionHull();
		graphics::drawDisk(hull.cx, hull.cy, hull.radius, br);

	}
	float player_life = getAlienLife();
	br.texture = "";
	br.fill_color[0] = 0.0f;
	br.fill_color[1] = 1.0f;
	br.fill_color[2] = 0.0f;
	br.fill_secondary_color[0] = 0.2f;
	br.fill_secondary_color[1] = 1.0f * (1.0f - player_life) + player_life * 0.2f;
	br.fill_secondary_color[2] = 0.2f * (1.0f - player_life) + player_life * 0.2f;
	br.gradient = true;
	br.gradient_dir_u = 2.0f;
	br.gradient_dir_v = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 2 - 100 - ((1.0f - player_life) * 120 / 2), 60.0f, player_life * 120, 20, br); //30.0f
	br.outline_opacity = 1.0f;
	br.gradient = false;
	br.fill_opacity = 0.0f;
	br.outline_width = 3.0f;
	graphics::drawRect(CANVAS_WIDTH / 2 + 140, 30.0f, 600, 20, br);

	for (Star* b_b : S_stars)
		b_b->draw();

}

void Alien::AlienUpdateStars() {

	bool AlienActiveStars[1];

	for (int i = 0; i < S_stars.size(); i++) {
		S_stars[i]->update();
		AlienActiveStars[i] = S_stars[i]->getActive();
	}

	for (int i = S_stars.size() - 1; i >= 0; i--) {
		if (!AlienActiveStars[i]) {
			delete S_stars[i];
			S_stars.erase(S_stars.begin() + i);
		}
	}

}
void Alien::init()
{
	speed = 0.1f;
	size = 200.0f;
	pos_x = CANVAS_WIDTH - 100;
	pos_y = CANVAS_HEIGHT / 2.0f;

	br.outline_opacity = 0.0f;
}


Disk Alien::getCollisionHull() const
{

	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size * 0.40f;
	return disk;
}

