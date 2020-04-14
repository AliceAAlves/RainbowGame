#include "Actor.h"
#include "InfoPanel.h"
#include "RotatingPlatform.h"
#include <windows.h>		// Header file for Windows
#include <iostream>
#include "Image_Loading/nvImage.h"
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>			// Header file for the GLu32 Library

Actor::Actor(float width, float height)
{
	this->width = width;
	this->height = height;
	this->xpos = 0;
	this->ypos = 0;
	this->texture = NULL;
	matrix[12] = xpos + width / 2;
	matrix[13] = ypos + height / 2;
}

 void Actor::display(float dt)
{
	glPushMatrix();
		glTranslatef(xpos, ypos, 0.0);
		glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_POLYGON);
			glVertex2f(0, 0);
			glVertex2f(0, height);
			glVertex2f(width, height);
			glVertex2f(width, 0);
		glEnd();
	glPopMatrix();
}

 void Actor::update(float dt)
 {
 }

float Actor::getX() const { 
	if (roundf(matrix[4]) == 0) { // sin = 0 -> no rotation or 180
		return getX_center() - width / 2;
	}
	else if(roundf(matrix[0]) == 0) { // cos = 0 -> 90 or 270 rotation
		return getX_center() - height / 2;
	}
	else {
		float temp = width;
		if (height > width) temp = height;
		return getX_center() - temp / 2;
	}
}
float Actor::getY() const { 
	if (roundf(matrix[4]) == 0) { // sin = 0 -> no rotation or 180
		return getY_center() - height / 2;
	}
	else if (roundf(matrix[0]) == 0) { // cos = 0 -> 90 or 270 rotation
		return getY_center() - width / 2;
	}
	else {
		float temp = width;
		if (height > width) temp = height;
		return getY_center() - temp / 2;
	}
}

float Actor::getW() const {
	if (roundf(matrix[4]) == 0) { // sin = 0 -> no rotation or 180
		return width;
	}
	else if (roundf(matrix[0]) == 0) { // cos = 0 -> 90 or 270 rotation
		return height;
	}
	else {
		float temp = width;
		if (height > width) temp = height;
		return temp;
	}
}
float Actor::getH() const {
	if (roundf(matrix[4]) == 0) { // sin = 0 -> no rotation or 180
		return height;
	}
	else if (roundf(matrix[0]) == 0) { // cos = 0 -> 90 or 270 rotation
		return width;
	}
	else {
		float temp = width;
		if (height > width) temp = height;
		return temp;
	}
}


 bool Actor::isOnScreen()
 {
	 float xmin = InfoPanel::getCam_xpos();
	 float xmax = InfoPanel::getCam_xpos() + InfoPanel::getCam_width();
	 float ymin = InfoPanel::getCam_ypos();
	 float ymax = InfoPanel::getCam_ypos() + InfoPanel::getCam_height();
	 
	 if (getX() > xmax || getX() + getW() < xmin || getY() > ymax || getY() + getH() < ymin) {
		 return false;
	 }
	 else { 
		 return true; 
	 }
 }

Actor::~Actor()
{
}

GLuint loadPNG(char* name)
{
	// Texture loading object
	nv::Image img;

	GLuint myTextureID;

	// Return true on success
	if (img.loadImageFromFile(name))
	{
		glGenTextures(1, &myTextureID);
		glBindTexture(GL_TEXTURE_2D, myTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(GL_TEXTURE_2D, 0, img.getInternalFormat(), img.getWidth(), img.getHeight(), 0, img.getFormat(), img.getType(), img.getLevel(0));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
	}

	else {
		MessageBox(NULL, name, "End of the world", MB_OK | MB_ICONINFORMATION);
	}
		

	return myTextureID;
}





