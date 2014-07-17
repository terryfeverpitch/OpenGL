#include <gl/glut.h>
#include "svl/svl.h"
#include <iostream>
using namespace std;

//
// Version 2: struct implementation, random initial pos & vel
//   
// ready to go to more particles
//


typedef struct _particle {
	Vec2 pos, vel;
	int birth_time;  // birth_timestamp
} PARTICLE;

Vec2 force(0,-1);  // gravity

PARTICLE p;
int now;

float frand(float lo, float hi)
{
	int r = rand()%143;
	return lo + (hi-lo)*r/142.;
}

void display()
{
	glClear (GL_COLOR_BUFFER_BIT);
	
	glColor3f(1,1,1);
	glBegin (GL_LINES);
	glVertex2i (-10,0); glVertex2i (10,0);
	glEnd();
	
#define max(x,y) ((x)>(y)?(x):(y))
		float life = (now - p.birth_time)/1000.;
		float intensity = max(0, (10 - life)/10);

		glColor3f (intensity,intensity,intensity);
		glBegin (GL_POINTS);
		glVertex2dv (p.pos.Ref());	
		glEnd();

	glutSwapBuffers();
}

void init_particle (unsigned char key, int x, int y)
{
	p.birth_time = glutGet (GLUT_ELAPSED_TIME);
	float start = frand(-3,-1);
	p.pos = Vec2 (start,0);
	start = frand (-.3,.3);
	p.vel = Vec2 (start,5);
}

float life (PARTICLE p) 
{
	return	(now - p.birth_time)/1000.;
}

void timer(int dummy)
{
	double dt = 0.1;  // seconds

	now = glutGet (GLUT_ELAPSED_TIME);

		p.pos += p.vel * dt;
		p.vel += force *dt;

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
	glLineWidth (5.0);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);
}

void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize (400,400);
	glutCreateWindow ("firework");
	
	glutDisplayFunc (display);
	glutTimerFunc (100, timer, 0);
	glutKeyboardFunc (init_particle);

	init();
	
	glutMainLoop();
}
