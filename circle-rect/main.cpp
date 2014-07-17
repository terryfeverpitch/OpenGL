
#include <glut.h>
#include <math.h>

#pragma comment(lib, "gifcapture-vc9.lib") 

extern void ProcessCapture(int,int=10);

void special (int key, int x, int y)
{
	if (key == GLUT_KEY_UP) {
		ProcessCapture(glutGetWindow ());
	}
}


void circle()
{	
	glColor3f (1,0,0);
	glBegin (GL_POLYGON);
	for (int i = 0; i < 20; i++) 
		glVertex2d (cos(i*6.28/20), sin(i*6.28/20));
	glEnd();
}

double cx,cy;
void display()
{
	glClear (GL_COLOR_BUFFER_BIT);
	
	glColor3f (1,1,0);
	glLineWidth (4.0);
	glBegin (GL_LINE_LOOP);
	glVertex2i (-6,6); glVertex2i (-6,-6); glVertex2i (6,-6); glVertex2i (6,6); 
	glEnd();

	glPushMatrix();
		glTranslated (cx,cy,0);
		circle();
	glPopMatrix();
	
	glutSwapBuffers();
}

extern bool CircleRectCD (double, double, double, double[2], double[2]);

void mouse (int button, int status, int mx, int my)
{
	
	if (button == GLUT_LEFT_BUTTON && status == GLUT_DOWN) {
		cx = -10+mx/20.;
		cy = 10 - my/20.;
		
		double min[2] = {-6,-6};
		double max[2] = {6,6};
		if (CircleRectCD (cx, cy, 1, min, max)) {//沒有撞到
			glPolygonMode (GL_FRONT, GL_FILL);
		}
		else {//有撞到
			glPolygonMode (GL_FRONT, GL_LINE);
		}

		glutPostRedisplay();
	}

}


void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize (400,400);
	glutCreateWindow ("circ-rect");
	glutDisplayFunc (display);
	glutMouseFunc (mouse);

	glutSpecialFunc(special);

	glViewport (0,0,400,400);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity(); gluOrtho2D (-10,10,-10,10);
	glMatrixMode (GL_MODELVIEW); glLoadIdentity();
	glutMainLoop();
}
