#include "gluit.h"
#include <stdio.h>
#include <math.h>

int mainwin, viewer;

void display(){}
void reshape(int w, int h)
{
	glViewport (0,0,w,h);
	ViewerReshape(viewer);
}

GLuint dl;

void wavycircle (float t)
{	
	const int ndiv = 140;
	const double MIX2 = 6.2832;
#if 1	
	if (!dl) {
		dl = glGenLists (1);
		glNewList (dl, GL_COMPILE_AND_EXECUTE);
#endif		
		glBegin (GL_LINE_LOOP);
		for (int i = 0; i < ndiv; i++) {
			double r = 2+0.3*(sin(t)+1.5)*cos(8*MIX2*i/ndiv);
			glVertex2d (r*cos(MIX2*i/ndiv), r*sin(MIX2*i/ndiv));
		}
		glEnd();
#if 1		
		glEndList();
	}

	glCallList (dl);
#endif

}

static GLfloat vertices[] = {
	0,0,0,
	1,0,0,
	1,0.5,1,  // 0.5 ... vertices[7] to be modified
	0,0,1};
static GLfloat colors[] = {
	1, 0.2, 0.2,
	0.2, 0.2, 1.0,
	0.8, 1.0, 0.2,
	0.3, 0.3, 0.5};

double t;

void content (void)
{
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glColor3f (1,1,1); 
	
	wavycircle (t);

    GLuint allIndices[] = {0,2,1, 0,3,2};
	glPushMatrix();
	// the most elegant way of using vertex array
	glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, allIndices);
	glRotatef (90., 0,1,0);glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, allIndices);
	glRotatef (90., 0,1,0);glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, allIndices);
	glRotatef (90., 0,1,0);glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, allIndices);
	glPopMatrix();

	glutSwapBuffers();
}

void init()
{
	glEnable (GL_DEPTH_TEST);

	glEnableClientState (GL_VERTEX_ARRAY);
	glEnableClientState (GL_COLOR_ARRAY);
	glVertexPointer (3, GL_FLOAT, 0, vertices);
	glColorPointer (3, GL_FLOAT, 0, colors);
}

void timer (int dummy)
{
	static float dx = 0.1;
	float x = vertices[7];

	t+= 0.1;

	if (x > 0.6 || x < -0.6) 
		dx *= -1;
	vertices[7] += dx;


	glutTimerFunc (10, timer, 0);
	glutPostRedisplay();
}

void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitWindowSize (400,400);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	mainwin = glutCreateWindow ("vertex array");

	glutDisplayFunc(display);
	glutReshapeFunc (reshape);

	viewer = ViewerNew (mainwin, 0,0,400,400, content);
	glutTimerFunc (10, timer, 0);
	init();

	glutMainLoop();
}
