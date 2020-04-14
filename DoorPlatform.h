#pragma once
#include "Platform.h"
class DoorPlatform :
	public Platform
{
public:
	DoorPlatform(int height_units, float x, float y);
	void display(float dt) override;
	void update(float dt) override;
	~DoorPlatform();
private:
	float distance = 150;
	float ySpeed = 0.1f;
	float ypos_start, ypos_next;
	static GLuint tx_yellow;
};

