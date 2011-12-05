#pragma once
#include <memory>
#include "InitOpenGL.h"

using namespace Eigen;
using namespace std;

class RgbaImageTexture;
class RotationModel;

class BookRenderer : public DisplayManager {
	void renderBook(void);
	shared_ptr<RotationModel> state;
	shared_ptr<RgbaImageTexture> texture;
public:
	BookRenderer(shared_ptr<RotationModel> initstate);
	virtual void display() ;
	virtual void idle();
	virtual void reshape(int width, int height)	;
	virtual void keyboard(unsigned char key, int x, int y);
	virtual void arrow_keys(int a_keys, int x, int y);
};
