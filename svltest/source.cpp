#include <glut.h>
#include <iostream>

using namespace std;

int pattern = 0x00ff;

void display()
{
	glViewport (0,0,400,400);
	glMatrixMode (GL_PROJECTION); glLoadIdentity(); gluOrtho2D (-10,10,-10,10);
	glMatrixMode (GL_MODELVIEW); glLoadIdentity();
	glClear (GL_COLOR_BUFFER_BIT);
	
	glColor3f (1,1,0);
	glLineStipple(2, pattern);
	glBegin (GL_LINE_LOOP);
	glVertex2i (3,3); glVertex2i (-3,3); glVertex2i (-3,-3); glVertex2i (3,-3);
	glEnd();
	
	glFlush();
}

void timer(int dummy)
{
	pattern = pattern << 1;
	if(pattern & 0x8000 && pattern != 0xff00)
	{
		pattern += 1;
	}
	/*
	if(pattern & 0x0001 && pattern != 0xff00)
	{
		//cout << pattern << endl;
		printf("%.8x\n", pattern);
		pattern = pattern >> 1;
		pattern += 0x1000;
	}
	else
		pattern = pattern >> 1;*/
	
	glutTimerFunc(100,timer,0);
	glutPostRedisplay();
}

void main(int argc, char** argv)
{
	glutInit (&argc,argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_SINGLE);
	glutInitWindowSize (400,400);
	glutCreateWindow ("bug");
	
	glutDisplayFunc (display);

	glEnable (GL_LINE_STIPPLE);
	glutTimerFunc(100,timer,0);
	//glLineStipple (2, 0xff08);//數字越大，線條越疏

	glutMainLoop();
}
