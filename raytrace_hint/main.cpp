#include "GL/gluit.h"
#include <svl/svl.h>
#include <math.h>

#define RADIUS 4.0

extern void DoRayTrace(void);  //ray.cpp

static int mainwin, viewer;
static int raycast;
static int FPScomputation = 0;
static GLdouble w_zoomFactor = 1.0;
static GLdouble h_zoomFactor = 1.0;

double before;
double after;
double omega = 0;
bool track = false;
bool _auto = false;
int mode = 1;
//
// global data
//

int maxlevel   = 3;   // max level of reflection
int numobjects = 3;  // id: 1,2,3
int numlights  = 1;
Vec3 lightpos[1]; 
Vec3 camera;

int gw =256;
int gh =256;

double theta = 0.0;

void display()
{
}

void reshape (int w, int h)
{
	w_zoomFactor = (double)w/256.0;
	h_zoomFactor = (double)h/256.0;
	//cout << w_zoomFactor << "  " << h_zoomFactor << endl;
	gw = w, gh = h;
	glViewport (0,0,w,h);
	ViewerReshape(viewer);
}

void DisplayImage(void)
{
	glMatrixMode (GL_PROJECTION); glPushMatrix(); glLoadIdentity();
	gluOrtho2D (0,20,0,20);  // this does not really matter: we draw an image
	glMatrixMode (GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
	glRasterPos2i (0,0);

	extern GLubyte image[256][256][3];
	//
	// use pixel zoom to account for reshape 	
	//
	//
	glPixelZoom(w_zoomFactor, h_zoomFactor);
	glDrawPixels (256,256, GL_RGB, GL_UNSIGNED_BYTE, image);
		
	glMatrixMode (GL_PROJECTION); glPopMatrix();
	glMatrixMode (GL_MODELVIEW);  glPopMatrix(); 
}

static void hint(void)
{
	BEGIN_2D_OVERLAY (gw, gh);
	
	glPushAttrib (GL_ENABLE_BIT);
	glDisable (GL_LIGHTING);
	glColor3f (0.88, 0.02, 0.89);
	setfont ("9x15",0);
	drawstr (10,gh-20, "r : switch raytrace");
	drawstr (10,gh-40, "t : show the trace");
	drawstr (10,gh-80, "Press '1' or '2' to change mode ");
	drawstr (10,gh-100, "a : AUTO ON/OFF    MODE %d", mode);
	if( _auto ) 
	{
		drawstr (10,gh-60, "+ : SPEED++ / - : SPEED--");
		glColor3f (1, 0.02, 0.02); 
		drawstr (10, gh-120, "AUTO : ON / SPEED = %.3f", omega);
	}
	else
	{
		drawstr (10,gh-60, "+ : clockwise  - : counter clockwise");
		glColor3f (.5, 0.5, 0.5); 
		drawstr (10, gh-120, "AUTO : OFF");
	}
	glPopAttrib();

	END_2D_OVERLAY();
}

void draw3Dscene()
{
	float yellow[] = {1,1,0,1};
	float white[] = {1,1,1,1};
	glMaterialfv (GL_FRONT, GL_DIFFUSE, yellow);
	glMaterialfv (GL_FRONT, GL_SPECULAR, white);
	glMateriali (GL_FRONT, GL_SHININESS, 40);
	float lpos[4] = {lightpos[0][0], lightpos[0][1], lightpos[0][2], 1.0};
	glLightfv (GL_LIGHT0, GL_POSITION, lpos);    // point light

	glEnable (GL_NORMALIZE);
	glPushMatrix ();
		glScalef (2., 1., 1.);
		glutSolidSphere (1.0, 20,20);
	glPopMatrix();

	float grey[] = {.2,.2,.2};
	float purple[] = {.5,0,.5};
	float green[] = {0,.7,0};
	glMaterialfv (GL_FRONT, GL_AMBIENT, grey); 
	glMaterialfv (GL_FRONT, GL_DIFFUSE, purple);
	glMaterialfv (GL_FRONT, GL_SPECULAR, green);
	glMateriali (GL_FRONT, GL_SHININESS, 10);

	glBegin (GL_QUADS);
		glNormal3f(0,1,0);
		glVertex3i (-4,-2,4);
		glVertex3i (4,-2,4);
		glVertex3i (4,-2,-4);
		glVertex3i (-4,-2,-4);
	glEnd();
}

void drawTrace()
{
	glColor3f (1,0,0);
	glBegin(GL_LINES);
	glVertex3f(lightpos[0][0], lightpos[0][1], lightpos[0][2]); glVertex3f(0,0,0);
	glEnd();

	if( mode == 1 )
	{
		glBegin(GL_LINE_LOOP);
		double delta;
		for (delta = 0.0 ; delta < 2 * 3.14159 ; delta += 0.1)
			glVertex3d(RADIUS*cos(delta), 5,RADIUS*sin(delta));
		glEnd();
	}
	else if( mode == 2)
	{
		glBegin(GL_LINE_LOOP);
		double delta;
		for (delta = 0.0 ; delta < 2 * 3.14159 ; delta += 0.1)
			glVertex3d(2*RADIUS*(cos(delta)/sqrt(11)+sin(delta)/sqrt(2)), 
					   2*RADIUS*(cos(delta)/sqrt(11)-sin(delta)/sqrt(2)),
					   -4*RADIUS*cos(delta)/sqrt(11));
		glEnd();
	}
}

void drawLight()
{
	glDisable (GL_LIGHTING);
	glColor3f (1,1,0);
	glPushMatrix();
	glTranslated (lightpos[0][0], lightpos[0][1], lightpos[0][2]);
	glutWireSphere (.1,12,12);
	glPopMatrix();
}

void content ()
{
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	
	if (raycast) 
	{
		DoRayTrace ();
		DisplayImage ();
	} 
	else 
	{
		draw3Dscene();
		
		glPushAttrib(GL_ENABLE_BIT);
		drawLight();
		
		if( track )
			drawTrace();

		hint();
		glPopAttrib();
	}

	//
	// FPS calculation and output
	///////////////////////////////////////////////////////////
	FPScomputation += 1;
	glutSwapBuffers();
}

void init (void)
{
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);
	glEnable (GL_NORMALIZE);

	lightpos[0] = Vec3 (0,5,RADIUS); // xz plane
}
void keyboard (unsigned char key, int x, int y)
{
	switch (key) 
	{
		case '1':
			mode = 1;
			lightpos[0] = Vec3 (0,5,RADIUS);
			break;
		case '2':
			mode = 2;
			lightpos[0][0] = 2*RADIUS*(cos(45)/sqrt(11) + sin(45)/sqrt(2));
			lightpos[0][1] = 2*RADIUS*(cos(45)/sqrt(11) - sin(45)/sqrt(2));
			lightpos[0][2] = -4*RADIUS*cos(45)/sqrt(11);
			break;
		case 'r': case 'R': 
			raycast ^= 1; //XOR:1 -> 0, 0 -> 1
			ViewerRedisplay(viewer);
			break;
		case 't':case 'T':
			track ^= 1;
			break;
		case '+':
			omega = (omega >= 0.15) ? (0.15):(omega+0.005);
			break;
		case '-':
			omega = (omega <= -0.15) ? (-0.15):(omega-0.005);
			break;
		case 'a':case 'A':
			_auto ^= 1;
			if( _auto ) omega = 0.025;
			break;
	}
}

void time (int dummy)
{
	if( _auto == false )
		omega *= 0.93;

	theta += omega;

	if( mode == 1)
	{
		lightpos[0][0] = RADIUS*cos(theta);
		lightpos[0][2] = RADIUS*sin(theta);
	}
	else if(mode ==2)
	{
		lightpos[0][0] = 2*RADIUS*(cos(theta)/sqrt(11) + sin(theta)/sqrt(2));
		lightpos[0][1] = 2*RADIUS*(cos(theta)/sqrt(11) - sin(theta)/sqrt(2));
		lightpos[0][2] = -4*RADIUS*cos(theta)/sqrt(11);
	}

	glutTimerFunc(10, time, 0);
	glutPostRedisplay();
}

char msg[] = "";

void FPStimer (int dummy)
{
	sprintf (msg, "FPS : %d\n", FPScomputation);
	glutSetWindow (mainwin);
	glutSetWindowTitle (msg);
	glutSetWindow (viewer);
	FPScomputation = 0;
	glutTimerFunc(1000, FPStimer, 0);
}

int main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (256,256);

	sprintf (msg, "FPS : %d\n", FPScomputation);
	mainwin = glutCreateWindow (msg);
	glutDisplayFunc (display);
	glutReshapeFunc (reshape);

	viewer = ViewerNew (mainwin,0,0,256,256, content);
	glutKeyboardFunc (keyboard);
	glutTimerFunc(10, time, 0);
	glutTimerFunc(1000, FPStimer, 0);
	glutIdleFunc (content);
	init();
	glutMainLoop();
	return 0;
}