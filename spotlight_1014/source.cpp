// spot light stuff:
//  spot_cutoff, spot_exponent, spot_direction
// fixed vs. animating
//
// make the color of the teapots vary in hue angles
//
#include "gluit.h"

int mainwin, viewer;
void display(){}
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}

void grid()
{
	glPushAttrib (GL_ALL_ATTRIB_BITS);
	glDisable (GL_LIGHTING);
	glLineWidth (1.0);
	glColor3f (1,1,1);
	glBegin (GL_LINES);
	for (int i = -5; i <= 5; i++) {
		glVertex3i (-5,0,i); glVertex3i (5,0,i);
		glVertex3i (i,0,-5); glVertex3i (i,0,5);
	}
	glEnd();
	glPopAttrib();
}

void scene()
{
	glPushMatrix();
	glTranslatef (-4,0,0); glutSolidTeapot(0.6);
	glPopMatrix();
	glPushMatrix();
	glTranslatef (-2,0,0); glutSolidTeapot(0.6);
	glPopMatrix();
	glPushMatrix();
	glTranslatef (0,0,0); glutSolidTeapot(0.6);
	glPopMatrix();
	glPushMatrix();
	glTranslatef (2,0,0); glutSolidTeapot(0.6);
	glPopMatrix();
	glPushMatrix();
	glTranslatef (4,0,0); glutSolidTeapot(0.6);
	glPopMatrix();
}

void content()
{
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	// stay here to observe teapots of different distance
	// attenuation: 1/(a + bd + cd^2)
	
	glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.1);  // a constant attenuation, 0.1=整比較亮, 1=整體比較暗 
	glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);   // b linear attenuation
	glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.1); // c quadratic attenuation*/

	float pos[4] = {0,0, 2,1};//燈的位置
	glLightfv (GL_LIGHT0, GL_POSITION, pos);
	glLightf (GL_LIGHT0, GL_SPOT_CUTOFF, 60);
	glLightf (GL_LIGHT0, GL_SPOT_EXPONENT, 5);
	glPopMatrix();
	
	glEnable (GL_LIGHTING);

	grid();
	scene();
	
	glutSwapBuffers();
}

void init()
{
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);

	glClearColor (.4,.4,.4,1);
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
	ViewerExtent (viewer, 10.0);  // adjust the viewer radius

	init();
	glutMainLoop();
}