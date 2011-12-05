#include "InitOpenGL.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include <Eigen/Core>
#include <Eigen/SVD>
#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
extern "C" {
#include "stb_image.c"
}


using namespace Eigen;
using namespace std;

Matrix4d asAffine(Matrix3d mat) {
	Matrix4d retval = Matrix4d::Identity();
	retval.topLeftCorner(3,3) = mat;
	return retval;
}

Matrix3d asTensor(Vector3d tensor) {
	Matrix3d retval = Matrix3d::Zero();
	retval(1,0) = tensor(2);
	retval(2,0) = -tensor(1);
	retval(0,1) = -tensor(2);
	retval(2,1) = tensor(0);
	retval(0,2) = tensor(1);
	retval(1,2) = -tensor(0);
	return retval;
}

struct RotationState {
	Vector3d const dimensions;
	Vector3d angular_momentum;
	Matrix3d MoI_body, MoI_body_inv;
	Matrix3d orientation;
	double timestep;

	RotationState(Vector3d dimensions, Vector3d angular_momentum)
		: dimensions(dimensions)
		, angular_momentum(angular_momentum)
		, orientation(Matrix3d::Identity())
		, timestep(0.01)
	{
		Vector3d diagOf_MoI=
			Vector3d(
				dimensions(1)*dimensions(1) + dimensions(2)*dimensions(2),
				dimensions(0)*dimensions(0) + dimensions(2)*dimensions(2),
				dimensions(1)*dimensions(1) + dimensions(0)*dimensions(0)
			);
		MoI_body = diagOf_MoI.asDiagonal(); //TODO:we're ignoring density
		MoI_body_inv = diagOf_MoI.array().inverse().matrix().asDiagonal();
	}

	void updateStepEuler() {
		Matrix3d I_inv = orientation * MoI_body_inv * orientation.transpose();
		Vector3d omega = I_inv * angular_momentum;
		orientation += timestep * asTensor(omega) * orientation;

		auto svd = orientation.jacobiSvd(ComputeFullU | ComputeFullV);
		orientation = svd.matrixU() * svd.matrixV().transpose();
	}
};

struct RGBImageTexture {
	//vector<unsigned char> data;
	int width, height;
	GLuint textureID;
	RGBImageTexture(char const * filename) {
		int n;
		unsigned char *rawdata = stbi_load(filename, &width, &height, &n, 3);
		cout<<"width: "<<width<<"; height: "<<height<<"; n: "<<n<<"; ptr:"<<(void*)rawdata<<"; val:"<<rawdata[10000]<<"\n";

		//data = vector(rawdata, rawdata+width*height*3);
	    // Create Texture
		cout<<"init "<<textureID<<"; err:"<<glGetError() << "\n";

		glGenTextures(1, &textureID);
	    cout<<"created "<<textureID<<"; err:"<<glGetError() << "\n";
	    glGenTextures(1, &textureID);
	    cout<<"created(again) "<<textureID<<"; err:"<<glGetError() << "\n";
	    glBindTexture(GL_TEXTURE_2D, textureID);   // 2d texture (x and y size)

	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

	    // 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image,
	    // border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
	    glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rawdata);
		stbi_image_free(rawdata);
		glEnable(GL_TEXTURE_2D);
	}
	void BindTexture() {
		glBindTexture(GL_TEXTURE_2D, textureID);
		//cout<<"bound "<<textureID<<"\n";
	}
	~RGBImageTexture() {
		glDeleteTextures(1, &textureID);
	}

	RGBImageTexture& operator=(const RGBImageTexture&) = delete;	// Disallow copying
	RGBImageTexture(const RGBImageTexture&) = delete;
};


class BookRenderer : public DisplayManager {
	void renderBook(void);
	RotationState state;
	shared_ptr<RGBImageTexture> texture;
public:
	BookRenderer()
		: state(Vector3d(1.0, 2.0, 0.3), Vector3d(0.0, 0.0, 1.0) )
	{
		texture = shared_ptr<RGBImageTexture>(new RGBImageTexture("../texture.png"));
	}
	virtual void display() ;
	virtual void idle();

	virtual void reshape(int width, int height)	;
	virtual void keyboard(unsigned char key, int x, int y);
	virtual void arrow_keys(int a_keys, int x, int y);
};


void BookRenderer::display() {
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
		//state.updateStepEuler();
		cout << state.orientation << "\n\n";
		break;

	case '1':
		state.orientation = Matrix3d::Identity();
		state.angular_momentum = Vector3d(1.0,0.0,0.0);
	break;
	case '2':
		state.orientation = Matrix3d::Identity();
		state.angular_momentum= Vector3d(0.0,1.0,0.0);
	break;
	case '3':
		state.orientation = Matrix3d::Identity();
		state.angular_momentum= Vector3d(0.0,0.0,1.0);
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
	glTranslated(0.0,0.0,-3.0);
	glMultMatrixd(asAffine(state.orientation).data());
	glScaled(state.dimensions.coeff(0),state.dimensions.coeff(1),state.dimensions.coeff(2));
	texture->BindTexture();
	glBegin(GL_QUADS);
	//glColor3f(1.0f,0.0f,0.0f);//front
	 glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	 glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	 glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,-1.0f, 1.0f);
	 glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f,-1.0f, 1.0f);
	glColor3f(0.0f,1.0f,0.0f); //top
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


int main(int argc, char** argv) {
	SetupMainLoop(new BookRenderer(), argc, argv);
}

