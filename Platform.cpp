#include "Platform.h"
#include "Collectable.h"

GLuint Platform::tx_purple = NULL;
GLuint Platform::tx_grey = NULL;
GLuint Platform::tx_grey_2 = NULL;

Platform::Platform(int width_units, float x, float y) : Actor(width_units*50, 50)
{
	xpos = x;
	ypos = y;
	active = false;
	active_texture = NULL;

}

void Platform::display(float dt)
{
	if(tx_purple == NULL) tx_purple = loadPNG("Resource/platform-purple.png");
	if(tx_grey == NULL) tx_grey = loadPNG("Resource/platform-grey.png");

	if (texture == NULL) { 
		active_texture = tx_purple;
		if (active) texture = active_texture;
		else texture = tx_grey;
	}

	if (Collectable::isCollected(0) && !active) activate();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	float xrepeat = width / 50;

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

void Platform::activate() { 
	if (!active) {
		texture = active_texture;
		active = true;
	}
}

Platform::~Platform()
{
}
