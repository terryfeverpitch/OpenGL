#include "gluit.h"

int mainwin, viewer;
void display(){}
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}

GLMmodel *m1;
GLuint treetexture;

void content()
{
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	
	// scene: grid, car, tree
	
	glutSwapBuffers();
}
void init()
{
	//
	// default setting: light on (and nothing else)
	//
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);
	
// reading OBJ model
m1 = glmReadOBJ ("pickup.obj");
glmUnitize (m1);
glmFacetNormals (m1);
glmVertexNormals (m1, 90);

pngSetStandardOrientation(1); 
treetexture = pngBind("shrub1.png", PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	
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
