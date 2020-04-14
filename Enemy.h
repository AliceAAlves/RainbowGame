#pragma once
#include "Actor.h"
#include <chrono>

class Enemy : public Actor
{
public:
	Enemy(float x, float y);
	void display(float dt) override;
	void update(float dt) override;
	void updateFrame();
	void defeat();
	bool toDelete() const;
	bool isDefeated() const;
	~Enemy();
protected:
	float distance;
	float xpos_start, xpos_next;
	float xSpeed = 0.07f;
	int currentFrame;
	bool defeated;
	bool to_delete = false;
	int defeat_points = 400;
	std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime, currentTime;
	static GLuint tx_enemy1, tx_cloud;
};

inline bool Enemy::toDelete() const {
	return to_delete;
}
inline bool Enemy::isDefeated() const { return defeated; }
