#pragma once
#include "Platform.h"
class VmovePlatform : public Platform
{
public:
	VmovePlatform(int width_units, float x, float y, float distance);
	float getYnext();
	void display(float dt) override;
	void update(float dt) override;
	~VmovePlatform();
private:
	float distance;
	float ypos_start, ypos_next;
	float ySpeed = 0.1f;
	static GLuint tx_green;
};

