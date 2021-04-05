#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma warning(disable:4505)
#endif
#include <GL/gl.h>
#include <GL/glut.h>
#include <Eigen/Core>
#include <iostream>
#include "matrixUtils.h"
#include "RotationModel.h"
#include "RgbaImageTexture.h"
#include "BookRenderer.h"

#include "ParticleBasedRotation.h"
#include "EulerRotation.h"
#include "QuaternionRotation.h"

using std::shared_ptr;
using std::cout;
#undef max
BookRenderer::BookRenderer(shared_ptr<RotationModel> initstate, double timestep, int timesteps_per_frame) : state(initstate), timestep(timestep), timesteps_per_frame(timesteps_per_frame)
{
	texture = shared_ptr<RgbaImageTexture>(new RgbaImageTexture("textures/cover.jpg"));
}

void BookRenderer::display() {
	for(int i=0;i<timesteps_per_frame;i++)
		state->updateStep(timestep/timesteps_per_frame);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-8.0f);
	renderBook();
	glutSwapBuffers();
}

void BookRenderer::idle() { display(); }

void BookRenderer::reshape(int width, int height)	{
	cout<<width<<"x"<<height<<"\n";
	if(height==0) height=1;

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(30.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();
}

void BookRenderer::keyboard(unsigned char key, int /*x*/, int /*y*/) {
	switch(key) {
	case 27:
		exit(0);
		break;
	case '+':
		timestep*=1.2;
		cout<<"Timestep per frame size: "<< timestep<<"\n";
		break;
	case '-':
		timestep/=1.2;
		cout<<"Timestep per frame size: "<< timestep<<"\n";
		break;
	case '/':
		timesteps_per_frame = std::max(1,(int)(timesteps_per_frame/1.1));
		cout<<"Timestep subdivisions per frame now: "<< timesteps_per_frame<<"\n";
		break;
	case '*':
		timesteps_per_frame = (int)(timesteps_per_frame*1.1+0.999);
		cout<<"Timestep subdivisions per frame now: "<< timesteps_per_frame<<"\n";
		break;
	case 'r':
		state->jiggle();
		break;
	case 'n':
		state->normalize = !state->normalize;
		cout <<"Normalize: "<<state->normalize <<"\n";
		break;
	case 'd':
		state->toggleDamping();
		break;
	case 'e':
		cout <<"Energy: "<<state->energy() <<"\n";
		break;
	case '1':
		state->resetWithAngularMomentum(Vector3d(1.0,0.0,0.0));
		break;
	case '2':
		state->resetWithAngularMomentum(Vector3d(0.0,0.5,0.0));
		break;
	case '3':
		state->resetWithAngularMomentum(Vector3d(0.0,0.0,1.0));
		break;
	case '7':
		state = shared_ptr<RotationModel>(new ParticleBasedRotation(state->getDimensions()));
		state->resetWithAngularMomentum(Vector3d(1.0,0.0,0.0));
		cout <<"ParticleBasedRotation\n";
		break;
	case '8':
		state = shared_ptr<RotationModel>(new EulerRotation(state->getDimensions()));
		state->resetWithAngularMomentum(Vector3d(1.0,0.0,0.0));
		cout <<"EulerRotation\n";
		break;
	case '9':
		state = shared_ptr<RotationModel>(new QuaternionRotation(state->getDimensions()));
		state->resetWithAngularMomentum(Vector3d(1.0,0.0,0.0));
		cout <<"QuaternionRotation\n";
		break;
	default:
		break;
	}
}

void BookRenderer::arrow_keys(int a_keys, int /*x*/, int /*y*/) {
	switch(a_keys) {
	case GLUT_KEY_UP:
		glutFullScreen();
		break;
	case GLUT_KEY_DOWN:
		glutReshapeWindow(720, 720);
		break;
	default:
		break;
	}
}

void BookRenderer::renderBook() {
	glPushMatrix();
	glRotated(180,1.0,0.0,0.0);
	glRotated(180,0.0,0.0,1.0);
	auto positions  = state->getPositions();
	texture->BindTexture();
	glBegin(GL_QUADS);
	glColor3f(1.0f,1.0f,1.0f);//front
	glTexCoord2f(0.444f, 0.0f); glVertex3d(positions(0,7), positions(1,7), positions(2,7));
	glTexCoord2f(0.0f,   0.0f); glVertex3d(positions(0,6), positions(1,6), positions(2,6));
	glTexCoord2f(0.0f,   1.0f); glVertex3d(positions(0,4), positions(1,4), positions(2,4));
	glTexCoord2f(0.444f, 1.0f); glVertex3d(positions(0,5), positions(1,5), positions(2,5));
	glColor3f(1.0f,1.0f,1.0f); //top
	glVertex3d(positions(0,3), positions(1,3), positions(2,3));
	glVertex3d(positions(0,2), positions(1,2), positions(2,2));
	glVertex3d(positions(0,6), positions(1,6), positions(2,6));
	glVertex3d(positions(0,7), positions(1,7), positions(2,7));
	glColor3f(1.0f,1.0f,1.0f);//bottom
	glVertex3d(positions(0,5), positions(1,5), positions(2,5));
	glVertex3d(positions(0,4), positions(1,4), positions(2,4));
	glVertex3d(positions(0,0), positions(1,0), positions(2,0));
	glVertex3d(positions(0,1), positions(1,1), positions(2,1));
	glColor3f(1.0f,1.0f,1.0f);//back
	glTexCoord2f(0.562f, 1.0f); glVertex3d(positions(0,1), positions(1,1), positions(2,1));
	glTexCoord2f(1.0f,   1.0f); glVertex3d(positions(0,0), positions(1,0), positions(2,0));
	glTexCoord2f(1.0f,   0.0f); glVertex3d(positions(0,2), positions(1,2), positions(2,2));
	glTexCoord2f(0.562f, 0.0f); glVertex3d(positions(0,3), positions(1,3), positions(2,3));
	glColor3f(1.0f,1.0f,1.0f);//left
	glVertex3d(positions(0,6), positions(1,6), positions(2,6));
	glVertex3d(positions(0,2), positions(1,2), positions(2,2));
	glVertex3d(positions(0,0), positions(1,0), positions(2,0));
	glVertex3d(positions(0,4), positions(1,4), positions(2,4));
	glColor3f(1.0f,1.0f,1.0f);//right (spine)
	glTexCoord2f(0.562f, 0.0f); glVertex3d(positions(0,3), positions(1,3), positions(2,3));
	glTexCoord2f(0.444f, 0.0f); glVertex3d(positions(0,7), positions(1,7), positions(2,7));
	glTexCoord2f(0.444f, 1.0f); glVertex3d(positions(0,5), positions(1,5), positions(2,5));
	glTexCoord2f(0.562f, 1.0f); glVertex3d(positions(0,1), positions(1,1), positions(2,1));
	glEnd();
	glPopMatrix();
}

/*
void BookRenderer::renderBook() {
	glPushMatrix();
	glMultMatrixd(asAffine(state->getOrientation()).data());
	auto dimensions = state->getDimensions();
	glScaled(dimensions(0),dimensions(1),dimensions(2));
	texture->BindTexture();
	glBegin(GL_QUADS);
	glColor3f(1.0f,1.0f,1.0f);//front
	glTexCoord2f(0.444f, 0.0f); glVertex3f( 1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f,   0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f,   1.0f); glVertex3f(-1.0f,-1.0f, 1.0f);
	glTexCoord2f(0.444f, 1.0f); glVertex3f( 1.0f,-1.0f, 1.0f);
	glColor3f(1.0f,1.0f,1.0f); //top
	glVertex3f( 1.0f, 1.0f,-1.0f);
	glVertex3f(-1.0f, 1.0f,-1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f( 1.0f, 1.0f, 1.0f);
	glColor3f(1.0f,1.0f,1.0f);//bottom
	glVertex3f( 1.0f,-1.0f, 1.0f);
	glVertex3f(-1.0f,-1.0f, 1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glVertex3f( 1.0f,-1.0f,-1.0f);
	glColor3f(1.0f,1.0f,1.0f);//back
	glTexCoord2f(0.562f, 1.0f); glVertex3f( 1.0f,-1.0f,-1.0f);
	glTexCoord2f(1.0f,   1.0f); glVertex3f(-1.0f,-1.0f,-1.0f);
	glTexCoord2f(1.0f,   0.0f); glVertex3f(-1.0f, 1.0f,-1.0f);
	glTexCoord2f(0.562f, 0.0f); glVertex3f( 1.0f, 1.0f,-1.0f);
	glColor3f(1.0f,1.0f,1.0f);//left
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f, 1.0f);
	glColor3f(1.0f,1.0f,1.0f);//right (spine)
	glTexCoord2f(0.562f, 0.0f); glVertex3f( 1.0f, 1.0f,-1.0f);
	glTexCoord2f(0.444f, 0.0f); glVertex3f( 1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.444f, 1.0f); glVertex3f( 1.0f,-1.0f, 1.0f);
	glTexCoord2f(0.562f, 1.0f); glVertex3f( 1.0f,-1.0f,-1.0f);
	glEnd();
	glPopMatrix();
}
*/
