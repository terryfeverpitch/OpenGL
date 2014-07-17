#include "gluit.h"

int picked[9] = {0,0,0,0,0,0,0,0,0};
int llx[3] = {1,4,7};
int lly[3] = {1,4,7};

int mainwin, viewer;
void display(){}
void reshape(int w, int h)
{
	glViewport (0,0,w,h);
	ViewerReshape(viewer);
}

void content()
{
	glClear (GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);
	
	glTranslatef (-5.5, 0, 5.5);
	glRotatef (-90, 1,0,0);
	
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

void clicker (int button, int state, int mx, int my)
{
extern void screen2object (const int x, const int y, float* xz);
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		float click[3];
		screen2object (mx, my, click);
		int ii, jj;
		
//		printf ("%lf %lf\n", click[0], click[1]);

		float x = click[0];
		float y = click[1];
		ii = (x-1)/3;
		jj = (y-1)/3;
		picked[3*ii+jj] ^= 1;

		glutPostRedisplay();
	}
}

void main (int argc, char** argv)
{
	glutInit (&argc,argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (400,400);
	mainwin = glutCreateWindow ("2-half D Pick: RIGHT button to toggle pick");
	
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);
	
	viewer = ViewerNew (mainwin, 0,0,400,400, content);
	
	init();
	ViewerAddClicker (viewer, clicker);
	ViewerExtent (viewer, 12.0);

	glutMainLoop();
}
