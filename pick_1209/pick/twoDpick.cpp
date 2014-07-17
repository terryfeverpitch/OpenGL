#include <gl/glut.h>

int picked[9] = {0,0,0,0,0,0,0,0,0};
int llx[3] = {1,4,7};
int lly[3] = {1,4,7};

void display()
{
	glClear (GL_COLOR_BUFFER_BIT);
	
	for (int ii = 0; ii < 3; ii++) {
		for (int jj = 0; jj < 3; jj++) {
			if (picked[3*ii+jj])
				glColor3f (1,1,0);
			else
				glColor3f (1,1,1);
			glRecti (llx[ii],lly[jj], llx[ii]+3, lly[jj]+3);
		}
	}
	
	glColor3f (1,0,0);
	glBegin (GL_LINES);
	for (int i = 1; i <= 10; i+= 3) {  // 1, 4, 7, 10
		glVertex2i (i,1); glVertex2i (i,10);
		glVertex2i (1,i); glVertex2i (10,i);
	}
	glEnd();
	glutSwapBuffers();
}

void init()
{
	glClearColor (.8,.8,.8,1);
	glLineWidth (5);
}

void mouse (int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		float x = mx*11/400.;
		float y = (400-my)*11/400.;
		int ii, jj;
		ii = (x-1)/3;
		jj = (y-1)/3;
		picked[3*ii+jj] ^= 1;
		glutPostRedisplay();
	}
}

void main (int argc, char** argv)
{
	glutInit (&argc,argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize (400,400);
	glutCreateWindow ("2D Pick: left button to toggle pick");
	
	glutDisplayFunc (display);
	init();
	glutMouseFunc (mouse);

	// pipeline
	glViewport (0,0,400,400);
	glMatrixMode (GL_PROJECTION); glLoadIdentity(); gluOrtho2D (0,11,0,11);
	glMatrixMode (GL_MODELVIEW); glLoadIdentity();
	
	glutMainLoop();
}
