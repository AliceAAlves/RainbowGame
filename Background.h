#pragma once

#include "Actor.h"

class Background : public Actor
{
public:
	Background(float width, float height, float x, float y);
	void display(float dt);
	void setRBtexture();
	~Background();
protected:
	GLuint rb_texture;
	static GLuint rb0, rb1, rb2, rb3, rb4, rb5, rb6, rb_none;
	float s = 0.0, t = 0.0, s_rb = 0.0, t_rb = 0.0;
}
;

