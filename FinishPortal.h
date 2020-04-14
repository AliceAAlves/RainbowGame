#pragma once
#include "Actor.h"
#include <chrono>
class FinishPortal :
	public Actor
{
public:
	FinishPortal(float x, float y);
	void display(float dt) override;
	void updateFrame();
	void activate();
	bool isActive() const;
	~FinishPortal();
private:
	int currentFrame;
	bool active = false;
	std::chrono::time_point<std::chrono::high_resolution_clock> currentTime,lastFrameTime;
	float frames[18][3] = { { 1,0.0f,0.75f },{ 2,0.2f,0.75f },{ 3,0.4f,0.75f },{ 4,0.6f,0.75f },{ 5,0.8f,0.75f },
							{ 6,0.0f,0.50f },{ 7,0.2f,0.50f },{ 8,0.4f,0.50f },{ 9,0.6f,0.50f },{ 10,0.8f,0.50f },
							{ 11,0.0f,0.25f },{ 12,0.2f,0.25f },{ 13,0.4f,0.25f },{ 14,0.6f,0.25f },{ 15,0.8f,0.25f }, 
							{ 16,0.0f,0.0f },{ 17,0.2f,0.0f },{ 18,0.4f,0.0f } };
};

inline bool FinishPortal::isActive() const { return active; }
