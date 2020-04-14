#include "FinishPortal.h"
#include "InfoPanel.h"
#include "Collectable.h"



FinishPortal::FinishPortal(float x, float y) : Actor(150,150*385/354)
{
	xpos = x;
	ypos = y;
	currentFrame = 18;
	lastFrameTime = std::chrono::high_resolution_clock::now();
}

void FinishPortal::display(float dt)
{
	if (!InfoPanel::paused) {
		updateFrame();
	}

	if (Collectable::isCollected(6) && !active) activate();

	if (texture == NULL) { texture = loadPNG("Resource/final-door-spritesheet.png"); }
	
	//Texture coordinates
	float smin = 0.0, tmin = 0.0;
	for (int i = 0; i < 18; i++) {
		if (frames[i][0] == currentFrame) {
			smin = frames[i][1];
			tmin = frames[i][2];
			break;
		}
	}
	float smax = smin + 0.2;
	float tmax = tmin + 0.25;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

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

void FinishPortal::updateFrame()
{
	if (active) {
		currentTime = std::chrono::high_resolution_clock::now();
		float dt = (float)((currentTime - lastFrameTime).count()) * 1000 * std::chrono::high_resolution_clock::period::num /
			std::chrono::high_resolution_clock::period::den; // milliseconds

		if (dt>70) { 
			if (currentFrame == 17) {
				currentFrame = 1;
			}
			else {
				currentFrame++;
			}
			lastFrameTime = currentTime;
		}
	}
}

void FinishPortal::activate()
{
	active = true;
	currentFrame = 1;
}


FinishPortal::~FinishPortal()
{
}
