#include "DoorPlatform.h"
#include "Collectable.h"

GLuint DoorPlatform::tx_yellow = NULL;

DoorPlatform::DoorPlatform(int height_units, float x, float y) : Platform(1, x, y)
{
	height = height_units * 50;
	ypos_start = ypos_next = y;
}

void DoorPlatform::display(float dt)
{
	if (tx_yellow == NULL) tx_yellow = loadPNG("Resource/platform-yellow.png");
	if (tx_grey_2 == NULL) tx_grey_2 = loadPNG("Resource/platform-grey_2.png");

	if (texture == NULL) {
		active_texture = tx_yellow;
		if (active) texture = active_texture;
		else texture = tx_grey_2;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	float yrepeat = height / width;

	glPushMatrix();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTranslatef(xpos + width / 2, ypos + height / 2, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex2f(-width / 2, -height / 2);
			glTexCoord2f(0.0, yrepeat); glVertex2f(-width / 2, height / 2);
			glTexCoord2f(1.0, yrepeat); glVertex2f(width / 2, height / 2);
			glTexCoord2f(1.0, 0.0); glVertex2f(width / 2, -height / 2);
		glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void DoorPlatform::update(float dt)
{
	if (Collectable::isCollected(3) && !active) activate();

	if (active && (ySpeed!=0 || ypos!= ypos_next)) {
		ypos = ypos_next;
		ypos_next += ySpeed * dt;
		if (ypos_next > ypos_start + distance) {
			ypos_next = ypos_start + distance;
			ySpeed = 0;
		}
		matrix[12] = xpos + width / 2;
		matrix[13] = ypos + height / 2;
	}
}


DoorPlatform::~DoorPlatform()
{
}
