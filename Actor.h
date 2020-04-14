#pragma once

#include "Actor.h"
#include <windows.h>		// Header file for Windows
#include <iostream>
#include "Image_Loading/nvImage.h"
#include <gl\gl.h>			// Header file for the OpenGL32 Library
#include <gl\glu.h>			// Header file for the GLu32 Library

class Actor
{
public:
	Actor(float width, float height);
	virtual void display(float dt);
	virtual void update(float dt);
	float getX() const;
	float getY() const;
	float getH() const;
	float getW() const;
	float getX_center() const;
	float getY_center() const;
	void setPos(float x, float y);
	bool isOnScreen();
	~Actor();

protected:
	float xpos; //Bottom left corner
	float ypos; //Bottom left corner
	float height;
	float width;
	GLuint texture;
	float matrix[16];
};


inline float Actor::getX_center() const { return  matrix[12]; }
inline float Actor::getY_center() const { return matrix[13]; }

inline void Actor::setPos(float x, float y) { xpos = x; ypos = y; }
GLuint loadPNG(char* name);


