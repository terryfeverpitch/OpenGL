#include "GL/gluit.h"
#include <math.h>

int mainwin, viewer;
void display(){}
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}

void grid()
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable (GL_LIGHTING);
	glBegin (GL_LINES);
	for (int i = -10; i <= 10; i++) {
		glVertex3i (-10,0,i); glVertex3i (10,0,i);
		glVertex3i (i,0,-10); glVertex3i (i,0,10);
	}
	glEnd();
	glPopAttrib();
}

void drawscene()
{
	float red[] = {1,0,0,1};
	float yellow[] = {1,1,0,1};
	float green[] = {0,1,0,1};

glPushName (1);
	glPushMatrix();
	glTranslatef (1, 1.5, 2);
	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red);
	glutSolidTeapot (1.0);
	glPopMatrix();
glPopName();

glPushName (2);
	glPushMatrix();
	glTranslatef (-3, 0.5, 2);
	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, yellow);
	glutSolidTeapot (1.0);
	glPopMatrix();
glPopName();

glPushName (3);
	glPushMatrix();
	glTranslatef (-1, 2.5, -2);
	glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);
	glutSolidTeapot (1.0);
	glPopMatrix();
glPopName();
}

void aim()
{
	BEGIN_2D_OVERLAY (10,10);
	glDisable (GL_LIGHTING);
	glColor3f (1,0,0);
	glLineWidth (3.0);
	glPushMatrix();
	glTranslatef (5,5,0);
	glBegin (GL_LINE_LOOP);
		double theta;
		for (theta = 0.0; theta < 2*3.14159; theta += 0.1)
			glVertex2d (1*cos(theta), 1*sin(theta));
	glEnd();
	glBegin (GL_LINES);
		glVertex2i (-1,0); glVertex2i (1,0);
		glVertex2i (0,-1); glVertex2i (0,1);
	glEnd();
	glPopMatrix();
	END_2D_OVERLAY();
}

void content()
{
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	
	grid();

	drawscene();
	aim();
	glutSwapBuffers();
}
void init()
{
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING); glEnable (GL_LIGHT0);
	glClearColor (.4,.4,.4,1);
	ShowCursor (false);
}

void pick (int x, int y)
{
	// start picking
	GLint viewport[4];
	GLfloat project[16];
		
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetFloatv (GL_PROJECTION_MATRIX, project);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
		
	/*  create 5x5 pixel picking region near cursor location */
	gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3] - y), 		   5.0, 5.0, viewport);
	glMultMatrixf (project);
	
	glMatrixMode (GL_MODELVIEW);

	#define BUFSIZE 10
	GLuint selectBuf[BUFSIZE];
	glSelectBuffer(BUFSIZE, selectBuf);
	glRenderMode (GL_SELECT);
	glInitNames(); 
	drawscene();

	// restore PROJECTION matrix
	glMatrixMode (GL_PROJECTION); glPopMatrix();

	GLint hits;
		
	hits = glRenderMode(GL_RENDER);
	printf ("hit: %d\n", hits);
	extern void processHits (GLint, GLuint[]);
	processHits(hits, selectBuf);
}

void clicker (int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		pick (x, y);
	}
}

void keyboard (unsigned char key, int x, int y)
{
	if (key == ' ') { // space triggered pick
		GLint viewport[4];
		
		glGetIntegerv(GL_VIEWPORT, viewport);
		pick (viewport[2]/2,viewport[3]/2);  // assume aiming at center of window
	}


}

void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (400,400);
	mainwin = glutCreateWindow ("Pick (SPACE or Right Button)");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	viewer = ViewerNew (mainwin, 0,0,400,400, content);
	init();
	ViewerExtent (viewer, 8);

	ViewerAddClicker (viewer, clicker);
	glutKeyboardFunc (keyboard);

	glutMainLoop();
}
