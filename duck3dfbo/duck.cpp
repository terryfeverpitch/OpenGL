#include <gl/glew.h>
#include "gluit.h"

int mainwin, viewer;
void display(){}
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}
GLMmodel *duck;

float turnangle;

void sceneDuck()
{
	glPushMatrix(); glLoadIdentity(); gluLookAt (0,0,3, 0,0,0, 0,1,0);
	glPushMatrix();
	glRotatef (turnangle, 0,1,0);
	glRotatef (90, 0,1,0);
	//glScalef (2,2,2);
	glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glmDraw (duck, GLM_TEXTURE|GLM_SMOOTH|GLM_MATERIAL);
	glPopMatrix();
	glPopMatrix();
}

float center, theta;

void duck_quad()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable (GL_ALPHA_TEST);
	glAlphaFunc (GL_GREATER, 0.0);

	glEnable (GL_TEXTURE_2D);
	glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	extern GLuint img;
	glBindTexture (GL_TEXTURE_2D, img);
	glBegin (GL_QUADS);
	glTexCoord2f (0,0); glVertex2i (-1,0);
	glTexCoord2f (1,0); glVertex2i ( 1,0);
	glTexCoord2f (1,1); glVertex2i ( 1,2);
	glTexCoord2f (0,1); glVertex2i (-1,2);
	glEnd();
	glPopAttrib();
}

void frame()
{
	glColor3f (1,0,0);
	glBegin (GL_LINE_LOOP);
	glVertex2i (-1,0);glVertex2i (1,0);glVertex2i (1,2);glVertex2i (-1,2);
	glEnd();	
}

void grid()
{
	glColor3f (1,1,1);
	glBegin (GL_LINES);
	for (int i = -8; i <= 8; i++) {
		glVertex3i (-8,0,i); glVertex3i (8,0,i);
		glVertex3i (i,0,-8); glVertex3i (i,0,8);
	}
	glEnd();
}

void content()
{
	extern void RTT();
	RTT();

	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	grid();

	glPushMatrix();
	glTranslatef (center, 0,0);
	glRotatef (theta, -1,0,0);

	duck_quad();
	frame();

	glPopMatrix();


	glutSwapBuffers();
}

void init()
{
	glClearColor (.4,.4,.4,1);
//	glEnable (GL_LIGHT0); glEnable (GL_LIGHTING);
	glEnable (GL_DEPTH_TEST);

	duck = glmReadOBJ ("rubberduck.obj");
	glmUnitize (duck);
	glmFacetNormals (duck);
	glmVertexNormals (duck, 90);

	glewInit();
	extern void initFBO();
	initFBO();

}

static float nowturn;

void turn(int dummy)
{
	turnangle += 5;
	if (turnangle - nowturn > 180) {
		turnangle = nowturn + 180;
	} else { 
		glutTimerFunc (10, turn, 0);
	}

	glutPostRedisplay();
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	case 't':
		nowturn = turnangle;
		glutTimerFunc (0, turn,0);
		break;
	}
	glutPostRedisplay();
}

void move (int dummy)
{
	static int sign = 1;

	center += sign*0.1;
	if (center > 3 || center < -3) {
		sign *= -1;

		nowturn = turnangle;
		glutTimerFunc (0, turn,0);
	}

	glutTimerFunc (100, move, 0);
	glutPostRedisplay();
}

void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (400,400);
	mainwin = glutCreateWindow ("tst");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	viewer = ViewerNew (mainwin, 0,0,400,400, content);
	init();
	glutKeyboardFunc (keyboard);
	glutTimerFunc (10, move, 0);

	glutMainLoop();
}
