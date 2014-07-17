// show ripple example
// tex coord > [0,1] ^ 2
// add texture animation

#include "gluit.h"
#include "gl/glpng.h"

int mainwin, viewer;
void display(){}
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}

// texture objects
GLuint pebbles, water;
float ds = 0;

void content()
{
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	
	// ocean floor
	glBindTexture (GL_TEXTURE_2D, pebbles);	
	glBegin (GL_QUADS);
	glTexCoord2f (0,0); glVertex3i (-5,0,5);
	glTexCoord2f (1,0); glVertex3i (5,0,5);
	glTexCoord2f (1,1); glVertex3i (5,0,-5);
	glTexCoord2f (0,1); glVertex3i (-5,0,-5);
	glEnd();
	
	// ocean surface, blended with ocean floor
	// texture transform
	glBindTexture (GL_TEXTURE_2D, water);
	glEnable (GL_BLEND);
	glPushMatrix();
	glTranslatef (0,1.5,0);
	glMatrixMode (GL_TEXTURE);
	glPushMatrix ();
	glTranslatef (ds, 0,0);
	glBegin (GL_QUADS);
	glTexCoord2f (0+ds,0); glVertex3i (-5,0,5);
	glTexCoord2f (1+ds,0); glVertex3i (5,0,5);
	glTexCoord2f (1+ds,1); glVertex3i (5,0,-5);
	glTexCoord2f (0+ds,1); glVertex3i (-5,0,-5);
	glEnd();
	glPopMatrix();
	glMatrixMode (GL_MODELVIEW);
	glPopMatrix();
	glDisable (GL_BLEND);
	
	glutSwapBuffers();
}

void timer( int dummy )
{
	ds += 0.005;

	glutTimerFunc(100, timer, 0);
	glutPostRedisplay();
}

void init()
{
	glClearColor (.4,.4,.4,1);
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_TEXTURE_2D);

	pebbles = pngBind("pebbles.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_REPEAT, GL_LINEAR, GL_LINEAR);
	water = pngBind("water.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_REPEAT, GL_LINEAR, GL_LINEAR);

	glBlendFunc (GL_ZERO, GL_SRC_COLOR);  // multiplicative blend
}

void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (400,400);
	mainwin = glutCreateWindow ("tst");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	glutTimerFunc(100, timer, 0);

	viewer = ViewerNew (mainwin, 0,0,400,400, content);
	init();

	glutMainLoop();
}