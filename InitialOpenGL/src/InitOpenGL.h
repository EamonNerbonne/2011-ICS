#pragma once

class DisplayManager {
public:
	DisplayManager(){}
	virtual void arrow_keys(int a_keys, int x, int y)=0;
	virtual void keyboard(unsigned char key, int x, int y)=0;
	virtual void reshape(int width, int height)=0;
	virtual void display()=0;
	virtual void idle()=0;
	virtual ~DisplayManager() {}
};

void SetupMainLoop(DisplayManager* (* factory)(), int argc, char **argv);
