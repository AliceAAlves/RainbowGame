#pragma once
#include "Actor.h"
#include <vector>
#include <chrono>

class Character : public Actor
{
public:
	Character();
	void display(float dt);
	float getX_next() const;
	float getY_next() const;
	float getYspeed() const;
	void setPos(float x, float y);
	void setIsWalking(bool isWalking);
	void xmove(bool left, bool right, float const dt);
	void ymove(bool jump, float const dt);
	void checkCollision(std::shared_ptr<Actor>);
	void checkAllCollision(std::vector<std::shared_ptr<Actor>> allObj);
	void updateFrame();
	void setHurt();
	void setFalling();
	void setFalling(float checkpoints[][2], int size);
	void reset();
	bool isFacingLeft();
	~Character();
private:
	float xpos_next, ypos_next;
	float yspeed = 0.0f;
	float const gravity = 0.003f;
	float const jumpSpeed = 1.4f;
	float const xSpeed = 0.5f;
	bool facingLeft;
	bool c_bottom, c_top, c_left, c_right; //collisions
	bool isWalking;
	bool hurt;
	bool falling;
	int currentFrame, currentFrame_t;
	float respawn_position[2] = {200,400};
	std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime, lastFrameTime_t, hurtTime, fallTime, currentTime;
};

inline void Character::setPos(float x, float y) { xpos = xpos_next = x; ypos = ypos_next = y; yspeed = 0; }
inline void Character::setIsWalking(bool isWalking) { this->isWalking = isWalking; }
inline bool Character::isFacingLeft() { return facingLeft; }
inline float Character::getX_next() const { return xpos_next; }
inline float Character::getY_next() const { return ypos_next; }
inline float Character::getYspeed() const { return yspeed; }