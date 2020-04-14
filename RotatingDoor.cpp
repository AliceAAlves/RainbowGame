#include "RotatingDoor.h"
#include "Collectable.h"

GLuint RotatingDoor::tx_red = NULL;

RotatingDoor::RotatingDoor(int width_units, float x, float y, bool leftDoor) : RotatingPlatform(width_units,x,y)
{
	matrix[12] = xpos + width / 2;
	matrix[13] = ypos + height / 2;
	this->leftDoor = leftDoor;
	rotation = 0;
}

void RotatingDoor::display(float dt)
{
	if (tx_red == NULL) tx_red = loadPNG("Resource/platform-red.png");
	if (tx_grey_2 == NULL) tx_grey_2 = loadPNG("Resource/platform-grey_2.png");

	if (texture == NULL) {
		active_texture = tx_red;
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
		glTranslatef(xpos, ypos, 0.0);
		if (!leftDoor) {
			glTranslatef(width, 0.0, 0.0);
		}
		glRotatef(rotation, 0.0, 0.0, 1.0);
		if(!leftDoor){
			glTranslatef(-width, 0.0, 0.0);
		}
		glTranslatef(width / 2,height / 2, 0.0);
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

void RotatingDoor::update(float dt)
{
	if (Collectable::isCollected(5) && !active) activate();

	if (active && rotationSpeed != 0) {
		rotating = true;
		if (roundf(matrix[4]) == 0) matrix[4] = 0.5;
		if (roundf(matrix[0]) == 1) matrix[0] = 0.5;
		if (leftDoor) {
			float rotation_temp = rotation - rotationSpeed * dt;
			if (rotation_temp < -90) {
				rotation = -90;
				rotationSpeed = 0;
				rotating = false;
				horizontal = true;
				if (roundf(matrix[4]) != 1) matrix[4] = 1;
				if (roundf(matrix[0]) != 0) matrix[0] = 0;
			}
			else rotation = rotation_temp;
		}
		else {
			float rotation_temp = rotation + rotationSpeed * dt;
			if (rotation_temp > 90) {
				rotation = 90;
				rotationSpeed = 0;
				rotating = false;
				horizontal = true;
				if (roundf(matrix[4]) != 1) matrix[4] = 1;
				if (roundf(matrix[0]) != 0) matrix[0] = 0;
			}
			else rotation = rotation_temp;
		}
		
	}
}


RotatingDoor::~RotatingDoor()
{
}
