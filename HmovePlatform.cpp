#include "HmovePlatform.h"
#include "Collectable.h"

GLuint HmovePlatform::tx_blue = NULL;

HmovePlatform::HmovePlatform(int width_units, float x, float y, float distance) : Platform(width_units, x, y)
{
	xpos_next = x;
	xpos_start = x;
	this->distance = distance;
}

void HmovePlatform::display(float dt)
{
	if (tx_blue == NULL) tx_blue = loadPNG("Resource/platform-blue.png");
	if (tx_grey == NULL) tx_grey = loadPNG("Resource/platform-grey.png");

	if (texture == NULL) {
		active_texture = tx_blue;
		if (active) texture = active_texture;
		else texture = tx_grey;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	float xrepeat = width / height;

	glPushMatrix();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTranslatef(xpos + width / 2, ypos + height / 2, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex2f(-width / 2, -height / 2);
			glTexCoord2f(0.0, 1.0); glVertex2f(-width / 2, height / 2);
			glTexCoord2f(xrepeat, 1.0); glVertex2f(width / 2, height / 2);
			glTexCoord2f(xrepeat, 0.0); glVertex2f(width / 2, -height / 2);
		glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void HmovePlatform::update(float dt)
{
	if (Collectable::isCollected(1) && !active) activate();

	if (active) {
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

float HmovePlatform::getIncrement()
{
	return xpos_next-xpos;
}

HmovePlatform::~HmovePlatform()
{
}
