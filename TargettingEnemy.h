#pragma once
#include "Enemy.h"
#include "Actor.h"
class TargettingEnemy :
	public Enemy
{
public:
	TargettingEnemy(float x, float y, std::shared_ptr<Actor> target);
	void display(float dt) override;
	void update(float dt) override;
	float getH_actual() const;
	float getW_actual() const;
	~TargettingEnemy();
private:
	float ypos_next;
	float speed = 0.08f;
	float spin_speed = 0.03f;
	float rotation = 0;
	static GLuint tx_enemy2;
	bool pursuit_active = false;
	std::shared_ptr<Actor> target;
};

inline float TargettingEnemy::getH_actual() const { return height*0.7; }
inline float TargettingEnemy::getW_actual() const { return width * 0.7; }
