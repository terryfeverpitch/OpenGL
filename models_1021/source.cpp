// OBJ (Wavefront) & STL (Standard Tesselation Language)
// two parts: model loading (read), and model displaying
// should only read once (do so in "init()")
// display in display callback
//
// three models:
// yoda.obj (no material info)
// toyplane.obj & toyplane.mtl (model and material)
// james.stl (no material)
//
#include "GL/gluit.h"
#include <stdlib.h>


#pragma comment(lib, "gluit.lib")

int mainwin, viewer;
void display(){}
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}

void grid_axes()
{
	glPushAttrib (GL_ALL_ATTRIB_BITS);
	glDisable (GL_LIGHTING);
	glBegin (GL_LINES);
	for (int i = -5; i <= 5; i++) {
		glVertex3i (-5,0,i); glVertex3i (5,0,i);
		glVertex3i (i,0,-5); glVertex3i (i,0,5);
	}
	glEnd();
	
	glLineWidth (5.0);
	glBegin (GL_LINES);
	glColor3f (1,0,0); glVertex3i (0,0,0); glVertex3i (1,0,0);
	glColor3f (0,1,0); glVertex3i (0,0,0); glVertex3i (0,1,0);
	glColor3f (0,0,1); glVertex3i (0,0,0); glVertex3i (0,0,1);
	glEnd();

	glPopAttrib();
}


GLMmodel *m1;
GLuint james_dl;

void content()
{
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	
	grid_axes();

	glPushMatrix();
	glTranslatef (-1,0,0);
	glmDraw (m1, GLM_MATERIAL | GLM_SMOOTH);
//	glmDraw (m1, GLM_SMOOTH);
	glPopMatrix();

	glPushMatrix();
	glTranslatef (1,1,0);
	glCallList (james_dl);
	glPopMatrix();
	
	glutSwapBuffers();
}

void init()
{
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);

	glClearColor (.4,.4,.4,1);
	
	// reading OBJ model
	//m1 = glmReadOBJ ("toyplane.obj");
	m1 = glmReadOBJ ("source.obj");

	glmUnitize (m1);
	glmFacetNormals (m1);
	glmVertexNormals (m1, 90);

	// reading STL model
	STLmodel james = stlRead ("james.stl");
	stlUnitize (james);
	james_dl = stlRender2DL (james);
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
	glutMainLoop();
}