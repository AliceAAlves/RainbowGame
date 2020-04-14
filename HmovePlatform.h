#pragma once
#include "Platform.h"
class HmovePlatform : public Platform
{
public:
	HmovePlatform(int width_units, float x, float y, float distance);
	float getIncrement();
	void display(float dt) override;
	void update(float dt) override;
	~HmovePlatform();
private:
	float distance;
	float xpos_start, xpos_next;
	float xSpeed = 0.1f;
	static GLuint tx_blue;
};

