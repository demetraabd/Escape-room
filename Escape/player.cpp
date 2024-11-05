#include "player.h"
#include "sgg/graphics.h"
#include "game.h"
#include <math.h>

Player::Player(const Game& mygame)
	:GameObject(mygame)
{
}
//move player using vector.h and chance oriantation
void Player::update()
{
	if (graphics::getKeyState(graphics::SCANCODE_LEFT))
	{
		orientation += 2.0f * graphics::getDeltaTime() / 10.0f;
		if (orientation > 360)
		{
			orientation = 0;
		}
	}

	if (graphics::getKeyState(graphics::SCANCODE_RIGHT))
	{
		orientation -= 2.0f * graphics::getDeltaTime() / 10.0f;
		if (orientation < 0)
		{
			orientation = 360;
		}

	}

	direction.fromAngle(orientation);

	if (graphics::getKeyState(graphics::SCANCODE_UP))
	{
		pos_x += direction.x * speed * graphics::getDeltaTime() / 10.0f;
		pos_y += direction.y * speed * graphics::getDeltaTime() / 10.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_DOWN))
	{
		pos_x -= direction.x * speed * graphics::getDeltaTime() / 10.0f;
		pos_y -= direction.y * speed * graphics::getDeltaTime() / 10.0f;

	}



	if (pos_x < 0)pos_x = 2;
	if (pos_x > CANVAS_WIDTH) pos_x = CANVAS_WIDTH;

	if (pos_y < 0)pos_y = CANVAS_HEIGHT;
	if (pos_y > CANVAS_HEIGHT) pos_y = 0;

	//elegxos max
	if (graphics::getKeyState(graphics::SCANCODE_SPACE) && stars.size() < MAX_STAR_COUNT) {
		//dimiourgia star
		Star* b = new Star(game, pos_x, pos_y, 20.0f, direction);
		stars.push_back(b);
		graphics::playSound(std::string(ASSET_PATH) + "shot.mp3", 0.2f, false);
	}
	updateStars();

}

void Player::updateStars() {

	bool activeStars[MAX_STAR_COUNT];

	for (int i = 0; i < stars.size(); i++) {
		stars[i]->update();
		activeStars[i] = stars[i]->getActive();
	}

	//mi xrisimopoioumena stars
	for (int i = stars.size() - 1; i >= 0; i--) {
		if (!activeStars[i]) {
			delete stars[i];
			stars.erase(stars.begin() + i);
		}
	}

}

void Player::draw()
{
	float glow = 0.5f + 0.5f * sinf(graphics::getGlobalTime() / 10);

	graphics::Brush br;
	if (life > 0.5f) {
		br.outline_opacity = 0.0f;
		graphics::setOrientation(orientation);
		br.fill_opacity = 1.0f;

		br.texture = std::string(ASSET_PATH) + "ufo.png";
		graphics::drawRect(pos_x, pos_y, 120, 120, br);
	}
	if (life <= 0.5f && life > 0.1f)
	{
		br.outline_opacity = 0.0f;
		br.fill_opacity = 1.0f;
		graphics::setOrientation(orientation);


		br.texture = std::string(ASSET_PATH) + "ufo_cracked.png";
		graphics::drawRect(pos_x, pos_y, 170, 170, br);
	}
	if (life < 0.1f) {

		br.outline_opacity = 0.0f;
		br.fill_opacity = 1.0f;
		graphics::setOrientation(orientation);


		br.texture = std::string(ASSET_PATH) + "ufo_dead.png";
		graphics::drawRect(pos_x, pos_y, 90, 90, br);
	}

	graphics::resetPose();

	float player_life = getRemainingLife();
	br.texture = "";
	br.outline_opacity = 0.0f;
	br.fill_color[0] = 0.0f;
	br.fill_color[1] = 1.0f;
	br.fill_color[2] = 0.0f;
	br.fill_secondary_color[0] = 0.2f;
	br.fill_secondary_color[1] = 1.0f * (1.0f - player_life) + player_life * 0.2f;
	br.fill_secondary_color[2] = 0.2f * (1.0f - player_life) + player_life * 0.2f;
	br.gradient = true;
	br.gradient_dir_u = 2.0f;
	br.gradient_dir_v = 0.0f;
	graphics::drawRect(100 - ((1.0f - player_life) * 120 / 2), 30.0f, player_life * 120, 20, br);
	br.outline_opacity = 1.0f;
	br.gradient = false;
	br.fill_opacity = 0.0f;
	br.outline_width = 3.0f;
	graphics::drawRect(100, 30.0f, 120, 20, br);

	if (game.getDebugMode())
	{
		br.outline_opacity = 1.0f;
		br.texture = "";
		br.fill_color[0] = 0.3f;
		br.fill_color[1] = 1.0f;
		br.fill_color[2] = 0.3f;
		br.fill_opacity = 0.3f;
		br.gradient = false;

		Disk hull = getCollisionHull();
		graphics::drawDisk(hull.cx, hull.cy, hull.radius, br);
	}


	for (Star* b : stars)
		b->draw();

}

void Player::init()
{
}

Disk Player::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = 35.0f;
	return disk;
}