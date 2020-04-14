#include "Platform.h"

#pragma once
class Ground : public Platform
{
public:
	Ground(int width_units, int height_units, float x, float y, int texture_type);
	void display(float dt) override;
	~Ground();
private:
	static GLuint tx_tl, tx_t, tx_tr, tx_l, tx_c, tx_r, tx_bl, tx_b, tx_br, tx_tlb, tx_tb, tx_trb, tx_ltr, tx_lr, tx_lbr;
	int tx_type;
};

