#include "Collectable.h"
#include "InfoPanel.h"

std::shared_ptr<Character> Collectable::head_follow = NULL;
const float Collectable::MAX_SPEED = 0.5f;

GLuint Collectable::tx_purple = NULL;
GLuint Collectable::tx_blue = NULL;
GLuint Collectable::tx_green = NULL;
GLuint Collectable::tx_yellow = NULL;
GLuint Collectable::tx_orange = NULL;
GLuint Collectable::tx_red = NULL;
GLuint Collectable::tx_pink = NULL;

bool Collectable::collected_purple = false;
bool Collectable::collected_blue = false;
bool Collectable::collected_green = false;
bool Collectable::collected_yellow = false;
bool Collectable::collected_orange = false;
bool Collectable::collected_red = false;
bool Collectable::collected_pink = false;

Collectable::Collectable(float x, float y, int colour, std::shared_ptr<Actor> following) : Actor(30, 30)
{
	this->xpos = x;
	this->ypos = y;
	this->colour = colour;
	this->following = NULL;
	if (std::shared_ptr<Character> f = std::dynamic_pointer_cast<Character>(following)) {
		if (head_follow == NULL) head_follow = f;
	}
	else if(std::shared_ptr<Collectable> f = std::dynamic_pointer_cast<Collectable>(following)) {
		this->following = f;
	}
	matrix[12] = xpos;
	matrix[13] = ypos;
}

void Collectable::display(float dt)
{
	if (tx_purple == NULL) {
		tx_purple = loadPNG("Resource/collect_item_purple.png");
		tx_blue = loadPNG("Resource/collect_item_blue.png");
		tx_green = loadPNG("Resource/collect_item_green.png");
		tx_yellow = loadPNG("Resource/collect_item_yellow.png");
		tx_orange = loadPNG("Resource/collect_item_orange.png");
		tx_red = loadPNG("Resource/collect_item_red.png");
		tx_pink = loadPNG("Resource/collect_item_pink.png");
	}

	if (texture == NULL) {
		texture = getTexture(colour);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTranslatef(xpos, ypos, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex2f(-width / 2, -height / 2);
			glTexCoord2f(0.0, 1.0); glVertex2f(-width / 2, height / 2);
			glTexCoord2f(1.0, 1.0); glVertex2f(width / 2, height / 2);
			glTexCoord2f(1.0, 0.0); glVertex2f(width / 2, -height / 2);
		glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void Collectable::update(float dt)
{
	if (Collectable::isCollected(colour)) {
		// MOVING
		// Moving from C to A
		// D = (A-C) -> direction

		float Ax, Ay;
		float Cx = xpos, Cy = ypos;
		float max_speed;

		float d_target = 35.0f;
		if (colour == 0) d_target = 65.0f;
		
		if (colour == 0) {
			Ax = head_follow->getX_center();
			Ay = head_follow->getY_center();
		}
		else {
			Ax = following->getX_center();
			Ay = following->getY_center();
		}
		
		// distance between target position and current positionsquared
		float d_sq = (Ax - Cx)*(Ax - Cx) + (Ay - Cy)*(Ay - Cy);

		float Dx = Ax - Cx, Dy = Ay - Cy;
		std::cout << "d_sq=" << d_sq << " / dtarget_sq=" << d_target * d_target << std::endl;
		float d1 = sqrt(d_sq) - d_target;
		if (roundf(d1 * 10) / 10 > 0) {
			float d1 = sqrt(d_sq) - d_target;
			std::cout << "d_sq > dtarget_sq " << d1 << std::endl;
			float t = sqrt(d1*d1 / (Dx*Dx + Dy * Dy));
			xpos += t * Dx;
			ypos += t * Dy;
		}
		else {
			if (roundf(Cy) > roundf(Ay)) {
				ypos -= 0.5f * dt;
			}
			else if(head_follow->getYspeed() == 0){
				if (head_follow->isFacingLeft() && roundf(xpos + 0.2f * dt) <= roundf(Ax + 0.8*d_target)) {
					xpos += 0.2f * dt;
				}
				else if (!head_follow->isFacingLeft() && roundf(xpos - 0.2f * dt) >= roundf(Ax - 0.8*d_target)) {
					xpos -= 0.2f * dt;
				}
			}
		}
		matrix[12] = xpos;
		matrix[13] = ypos;
	}
}

void Collectable::collect() {
	if (colour == 0) {
		collected_purple = true;
	}
	else if (colour == 1) {
		collected_blue = true;
	}
	else if (colour == 2) {
		collected_green = true;
	}
	else if (colour == 3) {
		collected_yellow = true;
	}
	else if (colour == 4) {
		collected_orange = true;
	}
	else if (colour == 5) {
		collected_red = true;
	}
	else if (colour == 6) {
		collected_pink = true;
	}
	InfoPanel::addScore(collect_points);
	PlaySound("Resource/collect.wav", NULL, SND_ASYNC | SND_FILENAME );
}

void Collectable::reset()
{
	collected_purple = collected_blue = collected_green = collected_yellow = collected_orange = collected_red = collected_pink = false;
}

GLuint Collectable::getTexture(int colour)
{
	if (colour == 0) {
		return tx_purple;
	}
	else if (colour == 1) {
		return tx_blue;
	}
	else if (colour == 2) {
		return tx_green;
	}
	else if (colour == 3) {
		return tx_yellow;
	}
	else if (colour == 4) {
		return tx_orange;
	}
	else if (colour == 5) {
		return tx_red;
	}
	else if (colour == 6) {
		return tx_pink;
	}
	return NULL;
}

bool Collectable::isCollected(int colour)
{
	if (colour == 0) {
		return collected_purple;
	}
	else if (colour == 1) {
		return collected_blue;
	}
	else if (colour == 2) {
		return collected_green;
	}
	else if (colour == 3) {
		return collected_yellow;
	}
	else if (colour == 4) {
		return collected_orange;
	}
	else if (colour == 5) {
		return collected_red;
	}
	else if (colour == 6) {
		return collected_pink;
	}
	return false;
}


Collectable::~Collectable()
{
}
