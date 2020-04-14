#pragma once
#include "RotatingPlatform.h"
class RotatingDoor :
	public RotatingPlatform
{
public:
	RotatingDoor(int width_units, float x, float y, bool leftDoor);
	void display(float dt) override;
	void update(float dt) override;
	~RotatingDoor();
private:
	bool leftDoor;
	static GLuint tx_red;
};

