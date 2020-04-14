#pragma once
#include "Platform.h"
class RotatingPlatform :
	public Platform
{
public:
	RotatingPlatform(int width_units, float x, float y);
	void display(float dt) override;
	void update(float dt) override;
	bool isRotating() const;
	bool isHorizontal() const;
	~RotatingPlatform();
protected:
	bool rotating = false;
	bool horizontal = false;
	float rotationSpeed = 0.1f;
	float rotation = 90;
	static GLuint tx_orange;
};

inline bool RotatingPlatform::isRotating() const { return rotating; }
inline bool RotatingPlatform::isHorizontal() const { return horizontal; }