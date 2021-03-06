#pragma once
#include <memory>
#include "InitOpenGL.h"

class RgbaImageTexture;
class RotationModel;

class BookRenderer : public DisplayManager {
	void renderBook(void);
	std::shared_ptr<RotationModel> state;
	double timestep;
	int timesteps_per_frame;
	std::shared_ptr<RgbaImageTexture> texture;
public:
	BookRenderer(std::shared_ptr<RotationModel> initstate, double timestep, int timesteps_per_frame);
	virtual void display() ;
	virtual void idle();
	virtual void reshape(int width, int height)	;
	virtual void keyboard(unsigned char key, int x, int y);
	virtual void arrow_keys(int a_keys, int x, int y);
};
