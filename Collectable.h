#pragma once
#include "Actor.h"
#include "Character.h"
class Collectable : public Actor
{
public:
	Collectable(float x, float y, int colour, std::shared_ptr<Actor> following);
	void display(float dt);
	void update(float dt);
	int getColour() const;
	void collect();
	static void reset();
	static GLuint getTexture(int colour);
	static bool isCollected(int colour);
	float getX() const;
	float getY() const;
	float getX_center() const;
	float getY_center() const;
	float getSpeed();
	~Collectable();

private:
	int colour; // 0-6 ---> 0:purple, 1:blue, 2:green, 3:yellow, 4:orange, 5:red, 6:pink
	std::shared_ptr<Collectable> following;
	float speed = 0.0f;
	static std::shared_ptr<Character> head_follow;
	static GLuint tx_purple, tx_blue, tx_green, tx_yellow, tx_orange, tx_red, tx_pink;
	static bool collected_purple, collected_blue, collected_green, collected_yellow, collected_orange, collected_red, collected_pink;
	static const float MAX_SPEED;
	int collect_points = 500;
};

inline int Collectable::getColour() const { return colour; }

inline float Collectable::getX() const { return xpos - width / 2; } // bottom left
inline float Collectable::getY() const { return ypos - height / 2; } // bottom left
inline float Collectable::getX_center() const { return xpos; }
inline float Collectable::getY_center() const { return ypos; }
inline float Collectable::getSpeed() { return speed; }
