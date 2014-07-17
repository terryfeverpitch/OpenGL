#include <glut.h>
#include <stdio.h>

void display()
{
	glViewport (0,0,256,256);
	glMatrixMode (GL_PROJECTION); glLoadIdentity(); gluOrtho2D (-10,10,-10,10);
	glMatrixMode (GL_MODELVIEW); glLoadIdentity();
	
	glClear (GL_COLOR_BUFFER_BIT);
	
	glColor3f (1,0,0);
	glRecti (-10,-10,0,0);
	glColor3f (0,1,0);
	glRecti (0,0,10,10);

	// BEFORE swapbuffers, every thing is drawn to the "back" buffer
	glReadBuffer (GL_BACK);
	GLubyte data[3];

	// [x,y] read the image location
	// [0,0] (origin) is at the lower left corner
	// (1,1) means read only one pixel
	glReadPixels (0,100,1,1, GL_RGB,GL_UNSIGNED_BYTE, data);   // lower left
	printf ("%x %x %x\n", data[0], data[1], data[2]);
	glReadPixels (155,5,1,1, GL_RGB,GL_UNSIGNED_BYTE, data); // lower right
	printf ("%x %x %x\n", data[0], data[1], data[2]);
	glReadPixels (155,155,1,1, GL_RGB,GL_UNSIGNED_BYTE, data);  // upper right
	printf ("%x %x %x\n", data[0], data[1], data[2]);
	glReadPixels (5,155,1,1, GL_RGB,GL_UNSIGNED_BYTE, data);  // upper left
	printf ("%x %x %x\n", data[0], data[1], data[2]);

	glutSwapBuffers();
}

void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize (256,256);
	glutCreateWindow ("tst");
	
	glutDisplayFunc (display);
	glClearColor (1,1,1,1);
	
	glutMainLoop();
}