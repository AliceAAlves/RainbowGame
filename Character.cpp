#include "Actor.h"
#include "Character.h"
#include "Ground.h"
#include "Platform.h"
#include "HmovePlatform.h"
#include "VmovePlatform.h"
#include "DoorPlatform.h"
#include "RotatingPlatform.h"
#include "RotatingDoor.h"
#include "Collectable.h"
#include "Enemy.h"
#include "TargettingEnemy.h"
#include "InfoPanel.h"
#include "FinishPortal.h"
#include <vector>
#include <chrono>
#include <math.h>


Character::Character() : Actor(70,70)
{
	currentFrame = 0;
	currentFrame_t = 0;
	lastFrameTime = lastFrameTime_t = std::chrono::high_resolution_clock::now();
}

void Character::display(float dt)
{
	if (!InfoPanel::paused) {
		updateFrame();
		xpos = xpos_next;
	}

	if (texture == NULL) { texture = loadPNG("Resource/char_spritesheet.png"); }
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	//Texture coordinates
	float smin = currentFrame*0.04, smax = (currentFrame + 1)* 0.04, tmin = currentFrame_t*0.5, tmax = (currentFrame_t+1)*0.5;
	if (facingLeft) {
		float temp = smin;
		smin = smax;
		smax = temp;
	}

	glPushMatrix();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTranslatef(xpos + width / 2, ypos + height / 2, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		glBegin(GL_QUADS);
			glTexCoord2f(smin, tmin); glVertex2f(-width / 2, -height / 2);
			glTexCoord2f(smin, tmax); glVertex2f(-width / 2, height / 2);
			glTexCoord2f(smax, tmax); glVertex2f(width / 2, height / 2);
			glTexCoord2f(smax, tmin); glVertex2f(width / 2, -height / 2);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void Character::xmove(bool left, bool right, float const dt) {
	if (right) {
		if(!c_right) xpos_next += xSpeed * dt;
		facingLeft = false;
	}
	else if (left) {
		if (!c_left) xpos_next -= xSpeed * dt;
		facingLeft = true;
	}
}

void Character::ymove(bool jump, float const dt) {
	if (!InfoPanel::paused) {
		ypos = ypos_next;
		if (jump && yspeed == 0) {
			yspeed = jumpSpeed;
		}
		else if (c_bottom) {
			yspeed = 0;
		}
		else if(c_top){ 
			yspeed = -yspeed; 
		}
		else {
			yspeed -= (gravity*dt);
		}
		ypos_next += yspeed * dt;
	}
}

void Character::checkCollision(std::shared_ptr<Actor> obj)
{
	float obj_xmin = obj->getX();
	float obj_xmax = obj->getX() + obj->getW();
	float obj_ymin = obj->getY();
	float obj_ymax = obj->getY() + obj->getH();
	
	//Circle test for COLLECTABLES
	if (std::shared_ptr<Collectable> coll = std::dynamic_pointer_cast<Collectable>(obj)) {
		if (!Collectable::isCollected(coll->getColour())) {
			float charRadius = height / 2;
			float collRadius = coll->getH() / 2 * 0.9;
			float objX_center = coll->getX_center();
			float objY_center = coll->getY_center();
			if ((objX_center - getX_center())*(objX_center - getX_center()) + (objY_center - getY_center())*(objY_center - getY_center()) < (charRadius + collRadius)*(charRadius + collRadius))
			{
				coll->collect();
			}
		}
		return;
	}
	// Tagetting enemies
	else if (std::shared_ptr<TargettingEnemy> enemy = std::dynamic_pointer_cast<TargettingEnemy>(obj)) {
		obj_xmin = enemy->getX_center() - enemy->getW_actual() / 2;
		obj_xmax = enemy->getX_center() + enemy->getW_actual() / 2;
		obj_ymin = enemy->getY_center() - enemy->getH_actual() / 2;
		obj_ymax = enemy->getY_center() + enemy->getH_actual() / 2;

		float charRadius = height / 2;
		float collRadius = enemy->getH_actual() / 2;
		float objX_center = enemy->getX_center();
		float objY_center = enemy->getY_center();
		
		//std::cout << obj_xmin << "," << obj_xmax << std::endl << charRadius << " rad " << collRadius << std::endl;
		// If on top defeat
		if ((ypos > obj_ymax && ypos_next <= obj_ymax || ypos == obj_ymax && ypos_next == obj_ymax) && xpos<obj_xmax && xpos + width>obj_xmin) {
			if (!enemy->isDefeated()) {
				yspeed = 1.0f;
				enemy->defeat();
			}
		}
		// Else use circle test to see if they are colliding
		else if ((objX_center - getX_center())*(objX_center - getX_center()) + (objY_center - getY_center())*(objY_center - getY_center()) < (charRadius + collRadius)*(charRadius + collRadius))
		{
			if (!hurt && !enemy->isDefeated()) {
				setHurt();
				InfoPanel::loseLife();
			}
		}
		return;
	}

	// Colliding with finish portal
	else if (std::shared_ptr<FinishPortal> portal = std::dynamic_pointer_cast<FinishPortal>(obj)) {
		if (portal->isActive() && !InfoPanel::isGameFinished()) {
			if (xpos<obj_xmax && xpos + width>obj_xmin && ypos<obj_ymax && ypos + height>obj_ymin) {
				InfoPanel::finishGame();
			}
		}
	}
	
	//Colliding at bottom of char (char is on top of obj)
	else if ((ypos > obj_ymax && ypos_next <= obj_ymax || ypos == obj_ymax && ypos_next == obj_ymax) && xpos<obj_xmax && xpos+width>obj_xmin) {
		// ACTIVE PLATFORM
		if ( std::shared_ptr<Platform> plat = std::dynamic_pointer_cast<Platform>(obj)){
			if (plat->isActive()) {
				c_bottom = true;
				ypos_next = obj_ymax;
				//If it's colliding with horizontal moving Platform, update xpos
				if (std::shared_ptr<HmovePlatform> h = std::dynamic_pointer_cast<HmovePlatform>(obj)) {
					xpos_next += h->getIncrement();
				}
				//If it's colliding with vertical moving Platform, update ypos
				if (std::shared_ptr<VmovePlatform> v = std::dynamic_pointer_cast<VmovePlatform>(obj)) {
					ypos_next = v->getYnext() + v->getH();
				}
			}
			else if (std::dynamic_pointer_cast<RotatingDoor>(obj)) {
				c_bottom = true;
				ypos_next = obj_ymax;
			}
			
		}
		// ENEMY
		if (std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(obj)) {
			if (!enemy->isDefeated()) {
				yspeed = 1.0f;
				enemy->defeat();
			}
		}
	}
	//Colliding with an enemy from another direction
	else if (std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(obj)) {
		if (!hurt && !enemy->isDefeated()) {
			if (xpos<obj_xmax && xpos + width>obj_xmin && ypos<obj_ymax && ypos + height>obj_ymin) {
				setHurt();
				InfoPanel::loseLife();
			}		
		}
	}
	//Colliding at top of char (char hit the bottom of obj)
	else if ((ypos + height < obj_ymin && ypos_next + height >= obj_ymin || ypos + height == obj_ymin && ypos_next + height == obj_ymin) && xpos<obj_xmax && xpos + width>obj_xmin) {
		// GROUND
		if (std::dynamic_pointer_cast<Ground>(obj)) {
			c_top = true;
		}
	}
	//Colliding at right of char (char hit the left of obj)
	else if ((xpos + width < obj_xmin && xpos_next + width >= obj_xmin || xpos + width == obj_xmin && xpos_next + width == obj_xmin) && ypos<obj_ymax && ypos + height>obj_ymin) {
		// GROUND
		if (std::dynamic_pointer_cast<Ground>(obj)) {
			c_right = true;
			xpos_next = obj_xmin-width;
		}
		// DOOR
		if (std::dynamic_pointer_cast<DoorPlatform>(obj)) {
			c_right = true;
			xpos_next = obj_xmin - width;
		}
		// ROTATING PLATFORM
		if (std::shared_ptr<RotatingPlatform> rotPlat = std::dynamic_pointer_cast<RotatingPlatform>(obj)) {
			if (!rotPlat->isRotating()) {
				c_right = true;
				xpos_next = obj_xmin - width;
			}
		}
	}
	//Colliding at left of char (char hit the right of obj)
	else if ((xpos > obj_xmax && xpos_next <= obj_xmax || xpos == obj_xmax && xpos_next == obj_xmax) && ypos<obj_ymax && ypos + height>obj_ymin) {
		// GROUND
		if (std::dynamic_pointer_cast<Ground>(obj)) {
			c_left = true;
			xpos_next = obj_xmax;
		}
		// DOOR
		if (std::dynamic_pointer_cast<DoorPlatform>(obj)) {
			c_left = true;
			xpos_next = obj_xmax;
		}
		// ROTATING PLATFORM
		if (std::shared_ptr<RotatingPlatform> rotPlat = std::dynamic_pointer_cast<RotatingPlatform>(obj)) {
			if (!rotPlat->isRotating()) {
				c_left = true;
				xpos_next = obj_xmax;
			}
		}
	}
}

void Character::checkAllCollision(std::vector<std::shared_ptr<Actor>> allObj) {
	c_bottom = c_top = c_left = c_right = false;
	float xmin = InfoPanel::getCam_xpos();
	float xmax = InfoPanel::getCam_xpos() + InfoPanel::getCam_width();
	// Making sure the character doesn't run off screen
	if (xpos > xmin && xpos_next <= xmin || xpos == xmin && xpos_next == xmin) {
		c_left = true;
		xpos_next = xmin;
	}
	else if (xpos + width < xmax && xpos_next + width >= xmax || xpos + width == xmax && xpos_next + width == xmax) {
		c_right = true;
		xpos_next = xmax - width;
	}
	// If character is off screen, it's because it has fallen off screen
	else if (!isOnScreen()) {
		if (!falling) {
			InfoPanel::loseLife();
			setFalling(InfoPanel::checkpoints, InfoPanel::CP_SIZE);
		}
	}
	for (int i = 0; i < allObj.size(); i++) {
		// Only needs to check for collisions if the actor is on screen (since the character is always on screen)
		if (allObj[i]->isOnScreen()) {
			checkCollision(allObj[i]);
		}
	}
}

void Character::updateFrame()
{
	currentTime = std::chrono::high_resolution_clock::now();
	float dt = (float)((currentTime - lastFrameTime).count()) * 1000 * std::chrono::high_resolution_clock::period::num /
		std::chrono::high_resolution_clock::period::den; // milliseconds

	if (hurt) {
		float dt_t = (float)((currentTime - lastFrameTime_t).count()) * 1000 * std::chrono::high_resolution_clock::period::num /
			std::chrono::high_resolution_clock::period::den; // milliseconds
		if (dt_t > 100) {
			if (currentFrame_t == 0) currentFrame_t++;
			else currentFrame_t--;
			lastFrameTime_t = currentTime;
		}

		float dt_hurt = (float)((currentTime - hurtTime).count()) * 1000 * std::chrono::high_resolution_clock::period::num /
			std::chrono::high_resolution_clock::period::den; // milliseconds
		if (dt_hurt > 1000) {
			hurt = false;
			currentFrame_t = 0;
		}
	}

	if (falling) {
		float dt_fall = (float)((currentTime - fallTime).count()) * 1000 * std::chrono::high_resolution_clock::period::num /
			std::chrono::high_resolution_clock::period::den; // milliseconds
		if (dt_fall > 2000) {
			falling = false;
			setPos(respawn_position[0], respawn_position[1]);
		}
	}


	if (yspeed > 0 && dt>80) { //going up frames 4-9
		if (currentFrame >= 4 && currentFrame <= 8) {
			currentFrame++;
		}
		else if(currentFrame != 9){
			currentFrame = 4; 
		}
		lastFrameTime = currentTime;
	}
	else if (yspeed < 0 && dt>80) { //going down frames 10-15
		if (currentFrame >= 10 && currentFrame <= 14) {
			currentFrame++;
		}
		else if (currentFrame != 15) {
			currentFrame = 10;
		}
		lastFrameTime = currentTime;
	}
	else if (currentFrame >= 10 && currentFrame <= 14 && dt > 80) { //it's no longer jumping but animation is still in the middle of jump
		currentFrame = 16;
		lastFrameTime = currentTime;
	}
	else if (isWalking && dt>100) { // walking frames 0 and 18-20 (use frames 16-17 if it was mid jump)
		if (currentFrame == 0) {
			currentFrame = 18;
		}
		else if (currentFrame >= 15 && currentFrame <= 19) {
			currentFrame++;
		}
		else {
			currentFrame = 0;
		}
		lastFrameTime = currentTime;
	}
	else if(dt>100){ // still frames 0-3 (use frames 16-17 if it was mid jump)
		if (currentFrame >= 0 && currentFrame <= 2) {
			currentFrame++;
		}
		else if (currentFrame >= 15 && currentFrame <= 16) {
			currentFrame++;
		}
		else {
			currentFrame = 0;
		}
		lastFrameTime = currentTime;
	}
}

void Character::setHurt()
{
	hurtTime = lastFrameTime_t = std::chrono::high_resolution_clock::now();
	hurt = true;
	currentFrame_t = 1;
}

void Character::setFalling()
{
	fallTime = std::chrono::high_resolution_clock::now();
	falling = true;
}

void Character::setFalling(float checkpoints[][2], int size)
{
	fallTime = std::chrono::high_resolution_clock::now();
	falling = true;
	for (int i = 0; i < size; i++) {
		if (xpos < checkpoints[i][0]) {
			if (i == 0) {
				respawn_position[0] = checkpoints[i][0];
				respawn_position[1] = checkpoints[i][1];
			}
			else {
				respawn_position[0] = checkpoints[i - 1][0];
				respawn_position[1] = checkpoints[i - 1][1];
			}
			break;
		}
	}
}

void Character::reset() {
	setPos(200, 400);
	facingLeft = false;
	c_bottom = c_top = c_left = c_right = false;
	isWalking = hurt = falling = false;
	respawn_position[0] = 200;
	respawn_position[1] = 400;
	currentFrame_t = 0;
}

Character::~Character()
{
}
