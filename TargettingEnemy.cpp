#include "TargettingEnemy.h"
#include "InfoPanel.h"
#include <math.h>

GLuint TargettingEnemy::tx_enemy2 = NULL;

TargettingEnemy::TargettingEnemy(float x, float y, std::shared_ptr<Actor> target) : Enemy(x, y)
{
	height = width = 98;
	this->target = target;
	ypos_next = y;
	matrix[12] = xpos;
	matrix[13] = ypos;
	defeat_points = 600;
}

void TargettingEnemy::display(float dt)
{
	if (!InfoPanel::paused) updateFrame();

	//display is only called when obj is on screen, so only then pursuit is activate
	if (!pursuit_active) pursuit_active = true;

	if (tx_enemy2 == NULL) { tx_enemy2 = loadPNG("Resource/enemy2_spreadsheet.png"); }
	if (tx_cloud == NULL) { tx_cloud = loadPNG("Resource/cloud_spreadsheet.png"); }
	if (texture == NULL) { texture = tx_enemy2; }

	float smin = currentFrame * 0.5, smax = (currentFrame + 1)* 0.5, tmin = 0.0, tmax = 1.0;
	if (defeated) {
		smin = currentFrame * 0.1, smax = (currentFrame + 1)* 0.1;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTranslatef(xpos, ypos, 0.0);
		glRotatef(rotation, 0.0, 0.0, 1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		glBegin(GL_QUADS);
			glTexCoord2f(smin, tmin); glVertex2f(-width/2, -height/2);
			glTexCoord2f(smin, tmax); glVertex2f(-width/2, height/2);
			glTexCoord2f(smax, tmax); glVertex2f(width/2, height/2);
			glTexCoord2f(smax, tmin); glVertex2f(width/2, -height/2);
		glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void TargettingEnemy::update(float dt)
{
	
	if (!defeated) {
		// ROTATING
		// A - TARGET , C - OBJECT
		// B - FACING DIRECTION (width/2,0) if target is on the right,  (-width/2,0) [d -> direction (-1 or 1)]
		float Ax = target->getX_center(), Ay = target->getY_center();
		float Cx = xpos, Cy = ypos;
		// If it's very distant deactivate pursuit
		if (abs(Cx - Ax) > 3000 || abs(Cy - Ay) > 2000) pursuit_active = false;
		if (pursuit_active) {
			int d;
			if (Ax > Cx) d = 1; //A is to the right of C
			else d = -1;
			float Bx = matrix[0] * (d*width / 2) + matrix[4] * 0 + matrix[12];
			float By = matrix[1] * (d*width / 2) + matrix[5] * 0 + matrix[13];
			// V = (B - C) , W = (A - C)
			float Vx = Bx - Cx, Vy = By - Cy;
			float Wx = Ax - Cx, Wy = Ay - Cy;
			// cross = V x W = VxWy - WxVy
			float cross = Vx * Wy - Wx * Vy;
			if (cross > 0) { rotation += spin_speed; }
			else if (cross < 0) { rotation -= spin_speed; }
			if (rotation>360) rotation = 0;

			// MOVING
			// Moving from C to A
			xpos = Cx = xpos_next;
			ypos = Cy = ypos_next;
			// D = (A-C) -> direction
			float Dx = Ax - Cx, Dy = Ay - Cy;
			// |t*D| = speed*dt -> distance to travel
			// |t*D|^2 = t^2*Dx^2 + t^2*Dy^2
			// t = sqrt((|t*D|^2)/(Dx^2+Dy^2))
			// t = sqrt((speed*dt)^2/(Dx^2+Dy^2))
			float t = sqrt((speed*dt)*(speed*dt) / (Dx*Dx + Dy * Dy));
			xpos_next += t * Dx;
			ypos_next += t * Dy;
			matrix[12] = xpos;
			matrix[13] = ypos;
		}
	}
	else rotation = 0;
}

TargettingEnemy::~TargettingEnemy()
{

}
