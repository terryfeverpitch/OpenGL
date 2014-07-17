#include "gl/gluit.h"

int mainwin, viewer;
void display(){}
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}

// scene related
#define NSPHERES 120
float pos[NSPHERES][3];
float radius[NSPHERES];

static int content_count;  // for FPS computation
static int cull;

void content()
{
	extern bool SphereInFrustum (float,float,float,float);

	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	// extract frustum parameter of current camera
	extern void ExtractFrustum();
	ExtractFrustum();

	int nshow = 0;
	for (int i = 0; i < NSPHERES; i++) {
		if (! cull || SphereInFrustum (pos[i][0], pos[i][1],pos[i][2], radius[i])) {
			++nshow;
			glPushMatrix();
				glTranslatef (pos[i][0], pos[i][1], pos[i][2]);
				glutSolidSphere (radius[i], 120,120); 
			glPopMatrix();
		}
	}


	BEGIN_2D_OVERLAY (10,10);
	glDisable(GL_LIGHTING);
	glColor3f (1,1,0);
	if (cull) 	
		drawstr (1,1, "cull [%d|%d]", NSPHERES-nshow, NSPHERES);
	else
		drawstr (1,1, "no cull");
	END_2D_OVERLAY();

	content_count++;
	glutSwapBuffers();
}

float frand (float lo, float hi)
{
	int i =	rand()%131; // [0,130]
	return lo + (float)i/130*(hi-lo); 
}

#define RANGE 5
void init()
{
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);

	glClearColor (.4,.4,.4,1);

	// initialize sphere positions: [-RANGE,RANGE]^3
	
	for (int i = 0; i < NSPHERES; i++) {
		pos[i][0] = frand(-RANGE,RANGE);
		pos[i][1] = frand(-RANGE,RANGE);
		pos[i][2] = frand(-RANGE,RANGE);
		radius[i] = frand(.2,1);
	}
}

void timer(int dummy)
{
	char msg[50];
	sprintf (msg, "FPS: %d\n", content_count);

	glutSetWindow (mainwin);
	glutSetWindowTitle (msg);
	//glutSetWindow (viewer);

	content_count = 0;
	glutTimerFunc (1000, timer, 0);
}

void keyboard (unsigned char key, int x, int y)
{
	if (key == 'c' || key == 'C') {
		cull ^= 1;
	}
	glutPostRedisplay();
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

	glutTimerFunc (1000, timer, 0);
	glutIdleFunc (content);
	glutKeyboardFunc (keyboard);

	glutMainLoop();
}
