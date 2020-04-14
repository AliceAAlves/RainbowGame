#include "Enemy.h"
#include "InfoPanel.h"

GLuint Enemy::tx_enemy1 = NULL;
GLuint Enemy::tx_cloud = NULL;

Enemy::Enemy(float x, float y) : Actor(98, 70)
{
	this->xpos = x;
	this->ypos = y;
	this->xpos_start = x;
	this->xpos_next = x;
	this->distance = 400;
	this->currentFrame = 0;
	this->defeated = false;
}

void Enemy::display(float dt)
{
	if(!InfoPanel::paused) updateFrame();

	if (tx_enemy1 == NULL) { tx_enemy1 = loadPNG("Resource/enemy1_spreadsheet.png"); }
	if (tx_cloud == NULL) { tx_cloud = loadPNG("Resource/cloud_spreadsheet.png"); }
	if (texture == NULL) { texture = tx_enemy1; }

	float smin = currentFrame * 0.5, smax = (currentFrame + 1)* 0.5, tmin = 0.0, tmax = 1.0;
	if (defeated) {
		smin = currentFrame * 0.1, smax = (currentFrame + 1)* 0.1;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTranslatef(xpos + width / 2, ypos + height / 2, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		glBegin(GL_QUADS);
			glTexCoord2f(smin, tmin); glVertex2f(-width / 2, -height / 2);
			glTexCoord2f(smin, tmax); glVertex2f(-width / 2, height / 2);
			glTexCoord2f(smax, tmax); glVertex2f(width / 2, height / 2);
			glTexCoord2f(smax, tmin); glVertex2f(width / 2, -height / 2);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void Enemy::update(float dt)
{
	if (!defeated) {
		xpos = xpos_next;
		xpos_next += xSpeed * dt;
		if (xpos_next > xpos_start + distance) {
			xpos_next = xpos_start + distance;
			xSpeed = -xSpeed;
		}
		else if (xpos_next < xpos_start) {
			xpos_next = xpos_start;
			xSpeed = -xSpeed;
		}
		matrix[12] = xpos + width / 2;
		matrix[13] = ypos + height / 2;
	}
}

void Enemy::updateFrame()
{
	currentTime = std::chrono::high_resolution_clock::now();
	float dt = (float)((currentTime - lastFrameTime).count()) * 1000 * std::chrono::high_resolution_clock::period::num /
		std::chrono::high_resolution_clock::period::den; // milliseconds

	if (!defeated && dt>300) { 
		if (currentFrame == 0)  currentFrame++;
		else currentFrame--;
		lastFrameTime = currentTime;
	}
	else if (defeated && dt > 100) {
		if (currentFrame == 8) to_delete = true;
		else {
			currentFrame++;
			lastFrameTime = currentTime;
		}
	}
}

void Enemy::defeat()
{
	defeated = true;
	height = 98;
	texture = tx_cloud;
	currentFrame = 0;
	InfoPanel::addScore(defeat_points);
	PlaySound("Resource/defeat.wav", NULL, SND_ASYNC | SND_FILENAME);
}

Enemy::~Enemy()
{
}
