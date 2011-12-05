#include "InitOpenGL.h"
#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glut.h>

static void InitGL() {

	glShadeModel(GL_SMOOTH);

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable ( GL_COLOR_MATERIAL );
	//glDisable ( GL_COLOR_MATERIAL );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}


DisplayManager *globalMan;
static void arrow_keys(int a_keys, int x, int y) { globalMan->arrow_keys(a_keys,x,y); }
static void keyboard(unsigned char key, int x, int y) { globalMan->keyboard(key,x,y); }
static void reshape(int width, int height) { globalMan->reshape(width,height); }
static void display() { globalMan->display(); }
static void idle() { globalMan->idle(); }

void SetupMainLoop(DisplayManager* (* factory)(), int argc, char **argv) {

	glutInit            ( &argc, argv );
	glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize  ( 500, 500 );
	glutCreateWindow    ( "Unstable Rotation" );
	//glutFullScreen      ( );
	InitGL ();
	glutReshapeFunc     ( reshape );
	glutKeyboardFunc    ( keyboard );
	glutSpecialFunc     ( arrow_keys );
	glutDisplayFunc     ( display );
	glutIdleFunc		( idle );

	globalMan = factory();
	glutMainLoop        ( );
}
