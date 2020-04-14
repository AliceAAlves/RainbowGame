#pragma once
#include "Actor.h"
class Platform : public Actor
{
public:
	Platform(int width_units, float x, float y);
	void display(float dt) override;
	bool isActive();
	void activate();
	~Platform();

protected:
	bool active;
	GLuint active_texture;
	static GLuint tx_purple;
	static GLuint tx_grey;
	static GLuint tx_grey_2;
};

inline bool Platform::isActive() { return active; }