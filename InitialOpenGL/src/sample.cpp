#include "InitOpenGL.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include <Eigen/Core>
#include <Eigen/SVD>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include "matrixUtils.h"
#include "RotationState.h"
#include "RgbaImageTexture.h"

using namespace Eigen;
using namespace std;


class BookRenderer : public DisplayManager {
	void renderBook(void);
	RotationState state;
	shared_ptr<RgbaImageTexture> texture;
public:
	BookRenderer()
		: state(Vector3d(1.0, 2.0, 0.3), Vector3d(1.0,0.0,0.0) )
	{
		texture = shared_ptr<RgbaImageTexture>(new RgbaImageTexture("../texture.png"));
	}
	virtual void display() ;
	virtual void idle();
	virtual void reshape(int width, int height)	;
	virtual void keyboard(unsigned char key, int x, int y);
	virtual void arrow_keys(int a_keys, int x, int y);
};


void BookRenderer::display() {
	for(int i=0;i<100;i++)
		state.updateStepEuler();
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
		cout << state.orientation << "\n\n";
		break;
	case '1':
		state.orientation = Matrix3d::Identity();
		state.angular_momentum = Vector3d(1.0,0.0,0.0);
	break;
	case '2':
		state.orientation = Matrix3d::Identity();
		state.angular_momentum= Vector3d(0.0,0.5,0.0);
	break;
	case '3':
		state.orientation = Matrix3d::Identity();
		state.angular_momentum= Vector3d(0.0,0.0,1.0);
	break;
	case '+':
		state.timestep *= 1.2;
	break;
	case '-':
		state.timestep /= 1.2;
	break;
	case 'r':
		state.angular_momentum += Vector3d::Random() * 0.01;
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
	glMultMatrixd(asAffine(state.orientation).data());
	glScaled(state.dimensions.coeff(0),state.dimensions.coeff(1),state.dimensions.coeff(2));
	texture->BindTexture();
	glBegin(GL_QUADS);
	glColor3f(1.0f,1.0f,1.0f);//front
	 glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	 glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	 glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,-1.0f, 1.0f);
	 glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f,-1.0f, 1.0f);
	//glColor3f(0.0f,1.0f,0.0f); //top
    glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f,-1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f,-1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glColor3f(1.0f,0.5f,0.0f);//bottom
	glVertex3f(1.0f,-1.0f, 1.0f);
	glVertex3f(-1.0f,-1.0f, 1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);
	glColor3f(1.0f,1.0f,0.0f);//back
	glVertex3f(1.0f,-1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glVertex3f(-1.0f, 1.0f,-1.0f);
	glVertex3f(1.0f, 1.0f,-1.0f);
	glColor3f(0.0f,0.0f,1.0f);//left
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f, 1.0f);
	glColor3f(1.0f,0.0f,1.0f);//right
	glVertex3f(1.0f, 1.0f,-1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f,-1.0f, 1.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);
	glEnd();
	glPopMatrix();
}

DisplayManager * RendererFactory() {return new BookRenderer(); }

int main(int argc, char** argv) {
	SetupMainLoop(RendererFactory, argc, argv);
}

