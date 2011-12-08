#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glut.h>
#include <Eigen/Core>
#include <iostream>
#include "matrixUtils.h"
#include "RotationModel.h"
#include "RgbaImageTexture.h"
#include "BookRenderer.h"

using std::shared_ptr;
using std::cout;

BookRenderer::BookRenderer(shared_ptr<RotationModel> initstate) : state(initstate)
{
	//texture = shared_ptr<RgbaImageTexture>(new RgbaImageTexture("../texture.png"));
	texturef = shared_ptr<RgbaImageTexture>(new RgbaImageTexture("../textures/dontpanic_front.png"));
	textureb = shared_ptr<RgbaImageTexture>(new RgbaImageTexture("../textures/dontpanic_back.png"));
	// front of book: "../textures/dontpanic_front.png"
	// back of book:  "../textures/dontpanic_back.png"
	// spine of book: "../textures/dontpanic_spine.png"
	// background:    "../textures/iss.jpg"
}

void BookRenderer::display() {
	for(int i=0;i<100;i++)
		state->updateStep();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-12.0f);
	renderBook();
	glutSwapBuffers();
}

void BookRenderer::idle() { display(); }

void BookRenderer::reshape(int width, int height)	{
	if(height==0) height=1;

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(30.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();
}

void BookRenderer::keyboard(unsigned char key, int x, int y) {
	switch(key) {
	case 27:
		exit(0);
		break;
	case 's':
		cout << state->getOrientation() << "\n\n";
		break;
	case '+':
		state->faster();
	break;
	case '-':
		state->slower();
	break;
	case 'r':
		state->jiggle();
	break;
	default:
		state->processInput(key);
	break;
	}
}

void BookRenderer::arrow_keys(int a_keys, int x, int y) {
	switch(a_keys) {
	case GLUT_KEY_UP:
		glutFullScreen();
		break;
	case GLUT_KEY_DOWN:
		glutReshapeWindow(500, 500);
		break;
	default:
		break;
	}
}


void BookRenderer::renderBook() {
	glPushMatrix();
	glMultMatrixd(asAffine(state->getOrientation()).data());
	auto dimensions = state->getDimensions();
	glScaled(dimensions(0),dimensions(1),dimensions(2));
	texturef->BindTexture();
	glBegin(GL_QUADS);
	glColor3f(1.0f,1.0f,1.0f);//front
	 glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	 glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	 glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,-1.0f, 1.0f);
	 glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f,-1.0f, 1.0f);
	glColor3f(1.0f,1.0f,1.0f); //top
	glVertex3f(1.0f, 1.0f,-1.0f);
	glVertex3f(-1.0f, 1.0f,-1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glColor3f(1.0f,1.0f,1.0f);//bottom
	glVertex3f(1.0f,-1.0f, 1.0f);
	glVertex3f(-1.0f,-1.0f, 1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);
	glColor3f(1.0f,1.0f,1.0f);//back
	textureb->BindTexture();
	  glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f,-1.0f,-1.0f);
	  glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,-1.0f,-1.0f);
	  glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f,-1.0f);
	  glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f,-1.0f);
	glColor3f(0.0f,0.5f,0.0f);//left
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f, 1.0f);
	glColor3f(1.0f,1.0f,1.0f);//right
	glVertex3f(1.0f, 1.0f,-1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f,-1.0f, 1.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);
	glEnd();
	glPopMatrix();
}
