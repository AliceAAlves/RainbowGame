#include "VmovePlatform.h"
#include "Collectable.h"

GLuint VmovePlatform::tx_green = NULL;

VmovePlatform::VmovePlatform(int width_units, float x, float y, float distance) : Platform(width_units, x, y)
{
	ypos_next = y;
	ypos_start = y;
	this->distance = distance;
}

float VmovePlatform::getYnext()
{
	return ypos_next;
}

void VmovePlatform::display(float dt)
{
	if (tx_green == NULL) tx_green = loadPNG("Resource/platform-green.png");
	if (tx_grey == NULL) tx_grey = loadPNG("Resource/platform-grey.png");

	if (texture == NULL) {
		active_texture = tx_green;
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
}


void VmovePlatform::update(float dt)
{
	if (Collectable::isCollected(2) && !active) activate();

	if (active) {
		ypos = ypos_next;
		ypos_next += ySpeed * dt;
		if (ypos_next > ypos_start + distance) {
			ypos_next = ypos_start + distance;
			ySpeed = -ySpeed;
		}
		else if (ypos_next < ypos_start) {
			ypos_next = ypos_start;
			ySpeed = -ySpeed;
		}
		matrix[12] = xpos + width / 2;
		matrix[13] = ypos + height / 2;
	}
}

VmovePlatform::~VmovePlatform()
{
}


