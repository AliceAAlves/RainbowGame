#include "InfoPanel.h"
#include "Actor.h"
#include "Collectable.h"
#include <chrono>

int InfoPanel::lives = 3;
float InfoPanel::cam_xpos = 0;
float InfoPanel::cam_ypos = 0;
float InfoPanel::cam_dx = 0;
float InfoPanel::cam_dy = 0;
float InfoPanel::cam_height = 720;
float InfoPanel::cam_width = 1280;
bool InfoPanel::paused = true;
bool InfoPanel::started = false;
bool InfoPanel::gameOver = false;
bool InfoPanel::gameFinished = false;
float InfoPanel::checkpoints[CP_SIZE][2] = { { 200,400 },{ 1850,400 },{ 4100,500 },{ 6600,400 },{ 7700,1200 },
{ 8700,1200 },{ 10100,1000 },{ 11100,400 },{ 13700,1200 } };
int InfoPanel::score = 0;

std::chrono::time_point<std::chrono::high_resolution_clock> InfoPanel::finishTime = std::chrono::high_resolution_clock::now();
std::chrono::time_point<std::chrono::high_resolution_clock> InfoPanel::currentTime = std::chrono::high_resolution_clock::now();

GLuint InfoPanel::tx_lives = NULL;
GLuint InfoPanel::tx_begin_panel = NULL;
GLuint InfoPanel::tx_gameover_panel = NULL;
GLuint InfoPanel::tx_finish_panel = NULL;

InfoPanel::InfoPanel()
{
}

void InfoPanel::display()
{
	currentTime = std::chrono::high_resolution_clock::now();
	
	if (tx_lives == NULL) {
		tx_lives = loadPNG("Resource/heart_life.png");
	}
	
	if (tx_begin_panel == NULL) {
		tx_begin_panel = loadPNG("Resource/start-screen.png");
	}
	if (tx_gameover_panel == NULL) {
		tx_gameover_panel = loadPNG("Resource/gameover-screen.png");
	}
	if (tx_finish_panel == NULL) {
		tx_finish_panel = loadPNG("Resource/finish-screen.png");
	}

	if (gameFinished && !paused) {
		float dt_finish = (float)((currentTime - finishTime).count()) * 1000 * std::chrono::high_resolution_clock::period::num /
			std::chrono::high_resolution_clock::period::den; // milliseconds
		if (dt_finish > 1000) {
			paused = true;
			PlaySound("Resource/game-win.wav", NULL, SND_ASYNC | SND_FILENAME);
		}
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	int scoreSquare_h = 50;
	int scoreSquare_w = 250;
	glPushMatrix();
		glTranslatef(cam_xpos + cam_width - scoreSquare_w - 20, cam_ypos + cam_height - scoreSquare_h - 20, 0.0);
		glColor4f(1.0, 1.0, 1.0, 0.6);
		glBegin(GL_POLYGON);
			glVertex2f(0, 0);
			glVertex2f(0, scoreSquare_h);
			glVertex2f(scoreSquare_w, scoreSquare_h);
			glVertex2f(scoreSquare_w, 0);
		glEnd();
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);

	for (int i = 0; i < lives; i++) {
		glPushMatrix();
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glBindTexture(GL_TEXTURE_2D, tx_lives);
			glTranslatef(cam_xpos+20+50*i, cam_ypos+cam_height-70, 0.0);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
				glTexCoord2f(0.0, 1.0); glVertex2f(0, 50);
				glTexCoord2f(1.0, 1.0); glVertex2f(50, 50);
				glTexCoord2f(1.0, 0.0); glVertex2f(50, 0);
			glEnd();
		glPopMatrix();
	}

	glDisable(GL_TEXTURE_2D);
	if (paused) {
		glPushMatrix();
			glTranslatef(cam_xpos, cam_ypos, 0.0);
			glColor4f(0.0, 0.0, 0.0,0.6);
			glBegin(GL_POLYGON);
				glVertex2f(0, 0);
				glVertex2f(0, cam_height);
				glVertex2f(cam_width, cam_height);
				glVertex2f(cam_width, 0);
			glEnd();
		glPopMatrix();
	}
	glEnable(GL_TEXTURE_2D);

	if (!started) {
		glPushMatrix();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, tx_begin_panel);
		glTranslatef(cam_xpos+(cam_width-926)/2, cam_ypos + (cam_height - 530) / 2, 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
			glTexCoord2f(0.0, 1.0); glVertex2f(0, 530);
			glTexCoord2f(1.0, 1.0); glVertex2f(926, 530);
			glTexCoord2f(1.0, 0.0); glVertex2f(926, 0);
		glEnd();
		glPopMatrix();
	}

	if (gameOver) {
		glPushMatrix();
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glBindTexture(GL_TEXTURE_2D, tx_gameover_panel);
			glTranslatef(cam_xpos + (cam_width - 926) / 2, cam_ypos + (cam_height - 530) / 2, 0.0);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
				glTexCoord2f(0.0, 1.0); glVertex2f(0, 530);
				glTexCoord2f(1.0, 1.0); glVertex2f(926, 530);
				glTexCoord2f(1.0, 0.0); glVertex2f(926, 0);
			glEnd();
		glPopMatrix();
	}

	if (gameFinished && paused) {
		glPushMatrix();
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glBindTexture(GL_TEXTURE_2D, tx_finish_panel);
			glTranslatef(cam_xpos + (cam_width - 926) / 2, cam_ypos + (cam_height - 530) / 2, 0.0);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
				glTexCoord2f(0.0, 1.0); glVertex2f(0, 530);
				glTexCoord2f(1.0, 1.0); glVertex2f(926, 530);
				glTexCoord2f(1.0, 0.0); glVertex2f(926, 0);
			glEnd();
		glPopMatrix();
	}
	

	glDisable(GL_TEXTURE_2D);
}

bool InfoPanel::loseLife()
{
	if (lives > 0) { 
		lives--;
		PlaySound("Resource/hurt.wav", NULL, SND_ASYNC | SND_FILENAME);
		return true;
	}
	else if (lives == 0) {
		setGameOver();
		PlaySound("Resource/game-over.wav", NULL, SND_ASYNC | SND_FILENAME);
	}
	return false;
}

void InfoPanel::reset() {
	lives = 3;
	score = 0;
	started = false;
	gameOver = false;
	gameFinished = false;
	Collectable::reset();
}

void InfoPanel::finishGame() { 
	gameFinished = true;
	finishTime = std::chrono::high_resolution_clock::now();
}

void InfoPanel::setCamPos(float x, float y, float h, float w) {
	cam_dx = x - cam_xpos;
	cam_dy = y - cam_ypos;
	cam_xpos = x;
	cam_ypos = y;
	cam_height = h;
	cam_width = w;
}

InfoPanel::~InfoPanel()
{
}
