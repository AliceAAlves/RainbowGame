#pragma once
#include "Actor.h"
#include <chrono>

class InfoPanel
{
public:
	InfoPanel();
	static void display();
	static bool loseLife(); // returns true if can lose a life, false if it has no more lives to lose (game over)
	static float getCam_xpos();
	static float getCam_ypos();
	static float getCam_height();
	static float getCam_width();
	static float getCam_dx();
	static float getCam_dy();
	static int getScore();
	static void addScore(int toAdd);
	static void start();
	static bool hasGameStarted();
	static void setGameOver();
	static bool isGameOver();
	static void finishGame();
	static bool isGameFinished();
	static void reset();
	static void setCamPos(float x, float y, float h, float w);
	~InfoPanel();
	static bool paused;
	static int const CP_SIZE = 9;
	static float checkpoints[CP_SIZE][2];

private:
	static int lives;
	static float cam_xpos; //Bottom left corner
	static float cam_ypos; //Bottom left corner
	static float cam_dx;
	static float cam_dy;
	static float cam_height;
	static float cam_width;
	static GLuint tx_lives;
	static GLuint tx_begin_panel, tx_gameover_panel, tx_finish_panel;
	static bool started;
	static bool gameOver;
	static bool gameFinished;
	static int score;
	static std::chrono::time_point<std::chrono::high_resolution_clock> finishTime, currentTime;
};

inline float InfoPanel::getCam_xpos() { return cam_xpos; }
inline float InfoPanel::getCam_ypos() { return cam_ypos; }
inline float InfoPanel::getCam_height() { return cam_height; }
inline float InfoPanel::getCam_width() { return cam_width; }
inline float InfoPanel::getCam_dx() { return cam_dx; }
inline float InfoPanel::getCam_dy() { return cam_dy; }
inline int InfoPanel::getScore() { return score; }
inline void InfoPanel::addScore(int toAdd) { score += toAdd; }
inline void InfoPanel::start() { started = true; paused = false; }
inline bool InfoPanel::hasGameStarted() { return started; }
inline void InfoPanel::setGameOver() { gameOver = true; paused = true; }
inline bool InfoPanel::isGameOver() { return gameOver; }
inline bool InfoPanel::isGameFinished() { return gameFinished; }