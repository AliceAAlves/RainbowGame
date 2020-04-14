#include "RotatingPlatform.h"
#include "Collectable.h"

GLuint RotatingPlatform::tx_orange = NULL;

RotatingPlatform::RotatingPlatform(int width_units, float x, float y) : Platform(width_units, x, y)
{
	matrix[12] = xpos + width / 2;
	matrix[13] = ypos + height / 2;
	matrix[4] == 1;
}

void RotatingPlatform::display(float dt)
{
	if (tx_orange == NULL) tx_orange = loadPNG("Resource/platform-orange.png");
	if (tx_grey_2 == NULL) tx_grey_2 = loadPNG("Resource/platform-grey_2.png");

	if (texture == NULL) {
		active_texture = tx_orange;
		if (active) texture = active_texture;
		else texture = tx_grey_2;
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
		glRotatef(rotation, 0.0, 0.0, 1.0);
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

void RotatingPlatform::update(float dt)
{
	if (Collectable::isCollected(4) && !active) activate();

	if (active && rotationSpeed != 0) {
		rotating = true;
		if (roundf(matrix[4]) == 1) matrix[4] = 0.5;
		if (roundf(matrix[0]) == 0) matrix[0] = 0.5;
		float rotation_temp = rotation - rotationSpeed * dt;
		if (rotation_temp < 0) {
			rotation = 0;
			rotationSpeed = 0;
			rotating = false;
			horizontal = true;
			if (roundf(matrix[4]) != 0) matrix[4] = 0;
			if (roundf(matrix[0]) != 1) matrix[0] = 1;
		}
		else rotation = rotation_temp;
	}
}


RotatingPlatform::~RotatingPlatform()
{
}
