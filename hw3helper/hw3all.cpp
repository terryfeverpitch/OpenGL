#include "gl/gluit.h"
#include "gl/glpng.h"
#include "svl/svl.h"

int mainwin, viewer;
float x = 0, y = 0, z = 0;

void display()
{
	
}

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
	
	extern void grid(), drawcar(float ,float ,float ), drawXtree(const Vec3&, const Vec3&);
	
	grid();
	drawcar(0, 0, 0);
	drawXtree(Vec3(1,0,1), Vec3(1,1,1));
	
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
	glClearColor (.4,.4,.4,1);
	
	// reading OBJ model
	
	m1 = glmReadOBJ ("pickup.obj");
	glmUnitize (m1);
	glmFacetNormals (m1);
	glmVertexNormals (m1, 90);

	

	pngSetStandardOrientation(1); 
	treetexture = pngBind("shrub1.png", PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
}
/*void timer (int dummy)
{
	theta += 6;

	static int sign = 1;
	x += sign*2;
	if (x > 50 || x < 0)
		sign *= -1;

	glutTimerFunc (100, timer, 0);
	glutPostRedisplay();
}*/

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
	//glutTimerFunc (100, timer, 0);
	glutMainLoop();
}
