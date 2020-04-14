#include "Ground.h"
#include "Platform.h"

GLuint Ground::tx_tl = NULL;
GLuint Ground::tx_t = NULL;
GLuint Ground::tx_tr = NULL;
GLuint Ground::tx_l = NULL;
GLuint Ground::tx_c = NULL;
GLuint Ground::tx_r = NULL;
GLuint Ground::tx_bl = NULL;
GLuint Ground::tx_b = NULL;
GLuint Ground::tx_br = NULL;
GLuint Ground::tx_ltr = NULL;
GLuint Ground::tx_lr = NULL;
GLuint Ground::tx_lbr = NULL;
GLuint Ground::tx_tlb = NULL;
GLuint Ground::tx_tb = NULL;
GLuint Ground::tx_trb = NULL;

Ground::Ground(int width_units, int height_units, float x, float y, int texture_type) : Platform(width_units,x,y)
{
	xpos = x;
	ypos = y;
	active = true;
	height = height_units * 50;
	tx_type = texture_type;
}

void Ground::display(float dt)
{
	if (texture == NULL) {
		texture = loadPNG("Resource/ground-5-center.png");
	}
	if (tx_c == NULL) {
		tx_tl = loadPNG("Resource/ground-1-left-top.png");
		tx_t = loadPNG("Resource/ground-2-top.png");
		tx_tr = loadPNG("Resource/ground-3-right-top.png");
		tx_l = loadPNG("Resource/ground-4-left.png");
		tx_c = loadPNG("Resource/ground-5-center.png");
		tx_r = loadPNG("Resource/ground-6-right.png");
		tx_bl = loadPNG("Resource/ground-7-left-bottom.png");
		tx_b = loadPNG("Resource/ground-8-bottom.png");
		tx_br = loadPNG("Resource/ground-9-right-bottom.png");
		tx_ltr = loadPNG("Resource/ground-10-left-top-right.png");
		tx_lr = loadPNG("Resource/ground-11-left-right.png");
		tx_lbr = loadPNG("Resource/ground-12-left-bottom-right.png");
		tx_tlb = loadPNG("Resource/ground-13-top-left-bottom.png");
		tx_tb = loadPNG("Resource/ground-14-top-bottom.png");
		tx_trb = loadPNG("Resource/ground-15-top-right-bottom.png");
	}

	GLuint extremity1, extremity2, middle;

	if (tx_type == 1) {
		extremity1 = tx_tl;
		extremity2 = tx_tr;
		middle = tx_t;
	}
	else if (tx_type == 2) {
		extremity1 = tx_tlb;
		extremity2 = tx_trb;
		middle = tx_tb;
	}
	else if (tx_type == 3) {
		extremity1 = tx_tl;
		extremity2 = tx_tr;
		middle = tx_c;
	}
	else if (tx_type == 4) {
		extremity1 = tx_t;
		extremity2 = tx_tr;
		middle = tx_c;
	}
	else if (tx_type == 5) {
		extremity1 = tx_tl;
		extremity2 = tx_t;
		middle = tx_t;
	}
	else if (tx_type == 6) {
		extremity1 = tx_tl;
		extremity2 = tx_r;
		middle = tx_c;
	}
	else if (tx_type == 7) {
		extremity1 = tx_c;
		extremity2 = tx_r;
		middle = tx_c;
	}
	else if (tx_type == 8) {
		extremity1 = tx_l;
		extremity2 = tx_tr;
		middle = tx_c;
	}
	else if (tx_type == 9) {
		extremity1 = tx_l;
		extremity2 = tx_c;
		middle = tx_c;
	}
	else if (tx_type == 10) {
		extremity1 = tx_t;
		extremity2 = tx_tr;
		middle = tx_t;
	}
	else if (tx_type == 11) {
		extremity1 = tx_tlb;
		extremity2 = tx_tb;
		middle = tx_tb;
	}
	else if (tx_type == 12) {
		extremity1 = tx_tb;
		extremity2 = tx_trb;
		middle = tx_tb;
	}
	else if (tx_type == 20) {
		extremity1 = tx_c;
		extremity2 = tx_ltr;
		middle = tx_lr;
	}
	else if (tx_type == 21) {
		extremity1 = tx_r;
		extremity2 = tx_r;
		middle = tx_r;
	}
	else if (tx_type == 22) {
		extremity1 = tx_c;
		extremity2 = tx_tl;
		middle = tx_l;
	}
	else if (tx_type == 23) {
		extremity1 = tx_c;
		extremity2 = tx_tr;
		middle = tx_r;
	}
	else if (tx_type == 24) {
		extremity1 = tx_br;
		extremity2 = tx_ltr;
		middle = tx_lr;
	}
	else if (tx_type == 25) {
		extremity1 = tx_bl;
		extremity2 = tx_tr;
		middle = tx_lr;
	}
	else if (tx_type == 26) {
		extremity1 = tx_br;
		extremity2 = tx_tr;
		middle = tx_lr;
	}
	else if (tx_type == 27) {
		extremity1 = tx_lbr;
		extremity2 = tx_ltr;
		middle = tx_lr;
	}
	else { extremity1 = extremity2 = middle = tx_c; }

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	
	// horizontal
	if (width>=50 && height==50) {
		float xrepeat = (width - height * 2) / height;
		//EXTREMITY 1
		glPushMatrix();
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glBindTexture(GL_TEXTURE_2D, extremity1);
			glTranslatef(xpos + width / 2, ypos + height / 2, 0.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
			glTranslatef(-width / 2, -height / 2, 0.0);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
				glTexCoord2f(0.0, 1.0); glVertex2f(0, height);
				glTexCoord2f(1.0, 1.0); glVertex2f(height, height);
				glTexCoord2f(1.0, 0.0); glVertex2f(height, 0);
			glEnd();
		glPopMatrix();

		if (width > 100) {
			//MIDDLE
			glPushMatrix();
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glBindTexture(GL_TEXTURE_2D, middle);
				glTranslatef(xpos + height, ypos, 0.0);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
					glTexCoord2f(0.0, 1.0); glVertex2f(0, height);
					glTexCoord2f(xrepeat, 1.0); glVertex2f((width - height * 2), height);
					glTexCoord2f(xrepeat, 0.0); glVertex2f((width - height * 2), 0);
				glEnd();
			glPopMatrix();
		}

		//EXTREMITY 2
		if (width > 50) {
			glPushMatrix();
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glBindTexture(GL_TEXTURE_2D, extremity2);
			glTranslatef(xpos + width - height, ypos, 0.0);
			glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
			glTexCoord2f(0.0, 1.0); glVertex2f(0, height);
			glTexCoord2f(1.0, 1.0); glVertex2f(height, height);
			glTexCoord2f(1.0, 0.0); glVertex2f(height, 0);
			glEnd();
			glPopMatrix();
		}
	}

	// vertical
	else if (height>50 && width==50) {
		float yrepeat = (height - width * 2) / width;
		//EXTREMITY 1
		glPushMatrix();
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glBindTexture(GL_TEXTURE_2D, extremity1);
			glTranslatef(xpos + width / 2, ypos + height / 2, 0.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
			glTranslatef(-width / 2, -height / 2, 0.0);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
				glTexCoord2f(0.0, 1.0); glVertex2f(0, width);
				glTexCoord2f(1.0, 1.0); glVertex2f(width, width);
				glTexCoord2f(1.0, 0.0); glVertex2f(width, 0);
			glEnd();
		glPopMatrix();

		if (height > 100) {
			//MIDDLE
			glPushMatrix();
				glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glBindTexture(GL_TEXTURE_2D, middle);
				glTranslatef(xpos, ypos + width, 0.0);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
					glTexCoord2f(0.0, yrepeat); glVertex2f(0, (height - width * 2));
					glTexCoord2f(1.0, yrepeat); glVertex2f(width, (height - width * 2));
					glTexCoord2f(1.0, 0.0); glVertex2f(width, 0);
				glEnd();
			glPopMatrix();
		}
	
		//EXTREMITY 2
		glPushMatrix();
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glBindTexture(GL_TEXTURE_2D, extremity2);
			glTranslatef(xpos, ypos + height - width, 0.0);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
				glTexCoord2f(0.0, 1.0); glVertex2f(0, width);
				glTexCoord2f(1.0, 1.0); glVertex2f(width, width);
				glTexCoord2f(1.0, 0.0); glVertex2f(width, 0);
			glEnd();
		glPopMatrix();
	}
	else if (width > 50 && height > 50) {
		float xrepeat = width / 50;
		float yrepeat = height / 50;

		glPushMatrix();
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, tx_c);
		glTranslatef(xpos + width / 2, ypos + height / 2, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		glTranslatef(-width / 2, -height / 2, 0.0);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
				glTexCoord2f(0.0, yrepeat); glVertex2f(0, height);
				glTexCoord2f(xrepeat, yrepeat); glVertex2f(width, height);
				glTexCoord2f(xrepeat, 0.0); glVertex2f(width, 0);
			glEnd();
		glPopMatrix();
	}
}

Ground::~Ground()
{
}
