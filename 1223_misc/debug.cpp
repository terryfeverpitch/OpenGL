#include <gl/glut.h>

void display()
{
	glViewport (0,0,400,400);
	glMatrixMode (GL_PROJECTION); glLoadIdentity(); gluOrtho2D (0,10,0,10);
	glMatrixMode (GL_MODELVIEW); glLoadIdentity();
	glClear (GL_COLOR_BUFFER_BIT);
	
	glColor3f (1,1,0);
	glBegin (GL_LINE);
	glVertex2i (0,3); glVertex2i (2,3);
	glEnd();
	
	glFlush();
}

void main(int argc, char** argv)
{
	glutInit (&argc,argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_SINGLE);
	glutInitWindowSize (400,400);
	glutCreateWindow ("bug");
	
	glutDisplayFunc (display);
	glutMainLoop();
}
