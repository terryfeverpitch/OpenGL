#include <gl/glut.h>
#include "svl/svl.h"
#include <iostream>
using namespace std;

#pragma comment(lib, "gifcapture.lib") 

extern void ProcessCapture(int,int=10);

void special (int key, int x, int y)
{
	if (key == GLUT_KEY_UP) {
		ProcessCapture(glutGetWindow ());
	}
}




//
// version 0: projectile
//
Vec2 force(0,-1);  // gravity
Vec2 pos, vel;

int now;

void display()
{
	glClear (GL_COLOR_BUFFER_BIT);
	
	glColor3f(1,1,1);
	glBegin (GL_LINES);
	glVertex2i (-10,0); glVertex2i (10,0);
	glEnd();
	
	glBegin (GL_POINTS);
		glVertex2dv (pos.Ref());	
	glEnd();

	glutSwapBuffers();
}

void init_particle (unsigned char key, int x, int y)
{
	pos = Vec2 (-2,0);
	vel = Vec2 (.3,5);
}

void timer(int dummy)
{
	double dt = 0.1;  // seconds

	pos += vel * dt;
	vel += force *dt;

	glutTimerFunc (100, timer, 0);
	glutPostRedisplay();
}

void init()
{
	// pipeline
    glViewport (0,0,400,400);
	glMatrixMode (GL_PROJECTION); glLoadIdentity(); gluOrtho2D (-10,10,-4,16);
	glMatrixMode (GL_MODELVIEW);

	// graphics setting
    glEnable (GL_POINT_SMOOTH);
    glPointSize (10);
}

void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize (400,400);
	glutCreateWindow ("firework");
	
	glutDisplayFunc (display);
	glutTimerFunc (100, timer, 0);
	glutKeyboardFunc (init_particle);  // press any key to start
	
	init();
	glutSpecialFunc(special);

	glutMainLoop();
}
