#include "Actor.h"
#include "Background.h"
#include "Collectable.h"
#include "InfoPanel.h"

GLuint Background::rb0 = NULL;
GLuint Background::rb1 = NULL;
GLuint Background::rb2 = NULL;
GLuint Background::rb3 = NULL;
GLuint Background::rb4 = NULL;
GLuint Background::rb5 = NULL;
GLuint Background::rb6 = NULL;
GLuint Background::rb_none = NULL;

Background::Background(float width, float height, float x, float y) : Actor(width, height)
{
	this->xpos = x;
	this->ypos = y;
	this->rb_texture = NULL;
	matrix[12] = xpos + width / 2;
	matrix[13] = ypos + height / 2;
}

void Background::display(float dt)
{
	if (rb0 == NULL) {
		rb0 = loadPNG("Resource/rainbow-0.png");
		rb1 = loadPNG("Resource/rainbow-1.png");
		rb2 = loadPNG("Resource/rainbow-2.png");
		rb3 = loadPNG("Resource/rainbow-3.png");
		rb4 = loadPNG("Resource/rainbow-4.png");
		rb5 = loadPNG("Resource/rainbow-5.png");
		rb6 = loadPNG("Resource/rainbow-6.png");
		rb_none = loadPNG("Resource/rainbow-none.png");
	}
	
	if (texture == NULL) { texture = loadPNG("Resource/sky-1.png"); }
	if (rb_texture == NULL) { rb_texture = rb_none; }

	setRBtexture();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	// Sky

	float s_width = InfoPanel::getCam_width() / (800 * 1.5f);
	float t_height = InfoPanel::getCam_height() / (800 * 1.5f);

	s += 0.0003*InfoPanel::getCam_dx();
	t += 0.0001*InfoPanel::getCam_dy();

	glPushMatrix();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);
			glTexCoord2f(s, t); glVertex2f(InfoPanel::getCam_xpos(), InfoPanel::getCam_ypos());
			glTexCoord2f(s, t + t_height); glVertex2f(InfoPanel::getCam_xpos(), InfoPanel::getCam_ypos() + InfoPanel::getCam_height());
			glTexCoord2f(s + s_width, t + t_height); glVertex2f(InfoPanel::getCam_xpos() + InfoPanel::getCam_width(), InfoPanel::getCam_ypos() + InfoPanel::getCam_height());
			glTexCoord2f(s + s_width, t); glVertex2f(InfoPanel::getCam_xpos() + InfoPanel::getCam_width(), InfoPanel::getCam_ypos());
		glEnd();
	glPopMatrix();

	//Rainbow

	float rb_width = 1701 * InfoPanel::getCam_height() / 720;

	float s_rb_width = 0.9;
	float t_rb_height = 0.9;

	s_rb += 0.000004*InfoPanel::getCam_dx();
	t_rb += 0.00003*InfoPanel::getCam_dy();

	glPushMatrix();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, rb_texture);
		glBegin(GL_QUADS);
			glTexCoord2f(s_rb, t_rb); glVertex2f(InfoPanel::getCam_xpos(), InfoPanel::getCam_ypos());
			glTexCoord2f(s_rb, t_rb + t_rb_height); glVertex2f(InfoPanel::getCam_xpos(), InfoPanel::getCam_ypos() + InfoPanel::getCam_height());
			glTexCoord2f(s_rb + s_rb_width, t_rb + t_rb_height); glVertex2f(InfoPanel::getCam_xpos() + rb_width, InfoPanel::getCam_ypos() + InfoPanel::getCam_height());
			glTexCoord2f(s_rb + s_rb_width, t_rb); glVertex2f(InfoPanel::getCam_xpos() + rb_width, InfoPanel::getCam_ypos());
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
}

void Background::setRBtexture() {
	if (Collectable::isCollected(6)) {
		rb_texture = rb6;
	}
	else if (Collectable::isCollected(5)) {
		rb_texture = rb5;
	}
	else if (Collectable::isCollected(4)) {
		rb_texture = rb4;
	}
	else if (Collectable::isCollected(3)) {
		rb_texture = rb3;
	}
	else if (Collectable::isCollected(2)) {
		rb_texture = rb2;
	}
	else if (Collectable::isCollected(1)) {
		rb_texture = rb1;
	}
	else if (Collectable::isCollected(0)) {
		rb_texture = rb0;
	}
	else rb_texture = rb_none;
}

Background::~Background()
{
}
