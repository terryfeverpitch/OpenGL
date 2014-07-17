#include "gluit.h"
#include <svl/svl.h>

extern int init_cyl();
extern void beamtrace (const Vec3, const Vec3);
extern void inittex();
extern void screen2object(int,int,float*);

int mainwin, viewer;

void display(){}
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}

GLuint cyl;

void grid()
{
	glPushAttrib (GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glBegin (GL_LINES);
	for (int i = -5; i <= 5; i++) {
		glVertex3i (-5,0,i); glVertex3i(5,0,i);
		glVertex3i (i,0,-5); glVertex3i(i,0,5);
	}
	glEnd();
	glPopAttrib();
}

float target[3];

void content()
{
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	glColor3f(1,1,1);
	grid();

	glPushAttrib (GL_ENABLE_BIT);
//	glColor3f(1,0,0);
//	glPointSize (5);
//	glBegin (GL_POINTS);
//		glVertex3fv(target);
//	glEnd();
	glDisable (GL_BLEND);
	glEnable (GL_LIGHTING); 
	glPushMatrix();
	glTranslatef(target[0], target[1], target[2]);
	glutSolidSphere(0.5,20,20);
	glPopMatrix();

	glPopAttrib();

	// draw transparent spot light
	glDepthMask (GL_FALSE);
	glEnable (GL_TEXTURE_1D);
	glColor3f (1,1,0);
	Vec3 base (-5,5,1);
	Vec3 character(target[0], target[1], target[2]);
	beamtrace (base, character);
	glDisable (GL_TEXTURE_1D);
	glDepthMask (GL_TRUE);
	
	glutSwapBuffers();
}

void init()
{
	glClearColor (.4,.4,.4,1);

	glEnable (GL_LIGHT0);
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLightModeli (GL_LIGHT_MODEL_TWO_SIDE, 1);
	
	cyl = init_cyl();
	inittex();
}

void passive (int x, int y)
{
	screen2object (x, y, target);
	ViewerRedisplay (viewer);
}

void keyboard (unsigned char key, int x, int y)
{
	if (key == 27)
		exit(0);
}

void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (400,400);
	mainwin = glutCreateWindow ("fake spotlight");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	viewer = ViewerNew (mainwin, 0,0,400,400, content);
	init();
	ViewerExtent (viewer, 10);
	glutPassiveMotionFunc (passive);
	glutKeyboardFunc (keyboard);

	glutMainLoop();
}
