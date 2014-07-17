//
// covered transparent floor
// mutiple texture (duck, 2x); 2x toggle; turn
// and single texture (aim)
// left for you ... motion, integration
//
#include <GL/glew.h>
#include <GL/gluit.h>
#include <GL/glpng.h>
#include <iostream>
using namespace std;
#pragma comment (lib, "glew32.lib")

int mainwin, viewer;
float pos = 0;
float shift = 0.05;
int flip = 1;
int angle = 0;
int delta = 0;
bool hit = false;
float timing;

void display(){}
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}
void grid()
{
	glPushAttrib (GL_ALL_ATTRIB_BITS);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glActiveTexture(GL_TEXTURE0); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1); glDisable(GL_TEXTURE_2D);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.3,1.3);

	glColor4f(1,0,1,.5);

	glBegin(GL_QUADS);
	glVertex3i(-10,0,10), glVertex3i(10,0,10), glVertex3i(10,0,-10), glVertex3i(-10,0,-10);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(1,1,1);
	for(int i = -10 ; i <= 10 ; i++)
	{
		glVertex3i(i,0,10), glVertex3i(i,0,-10);
		glVertex3i(10,0,i), glVertex3i(-10,0,i);
	}
	glEnd();

	glPopAttrib();
}

GLuint duckid, doubleid;

void content()
{
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);	

	glPushAttrib (GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0);
	glTranslatef(pos,0,0);
	glRotatef(angle, 1,0,0);
	
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.5, 2.5);

	glBegin(GL_QUADS);
	glMultiTexCoord2f(GL_TEXTURE0, 0 * flip,0); glMultiTexCoord2f(GL_TEXTURE1, 0,0); glVertex2i(-1,0);
	glMultiTexCoord2f(GL_TEXTURE0, 1 * flip,0); glMultiTexCoord2f(GL_TEXTURE1, 1,0); glVertex2i(1,0);
	glMultiTexCoord2f(GL_TEXTURE0, 1 * flip,1); glMultiTexCoord2f(GL_TEXTURE1, 1,1); glVertex2i(1,2);
	glMultiTexCoord2f(GL_TEXTURE0, 0 * flip,1); glMultiTexCoord2f(GL_TEXTURE1, 0,1); glVertex2i(-1,2);
	glEnd();
	glPopMatrix();
	glPopAttrib();
	/*glPushAttrib (GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glTranslatef(0,1,0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glutSolidTeapot(1.0);
	glPopMatrix();
	glPopAttrib();*/	//draw teapot
	grid();

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case ' ':
			if( hit == false )
			{
				hit = true;
				delta = 10;
			}
			break;
	}

}

void count_down (int dummy) 
{
	delta = -10;
}

void timer (int dummy)
{
	
	pos	  += shift;
	angle -= delta;

	if( pos >= 6 || pos <= -6 ) 
	{
		shift *= -1;
		flip  *= -1;
	}

	if( angle <= -90)
	{
		angle = -90;
		delta = 0;
		glutTimerFunc (800, count_down, 0);
	}
	
	if( angle >= 0 )
	{
		angle = 0;
		delta = 0;
		hit = false;
	}

	glutTimerFunc (50, timer, 0);
	glutPostRedisplay();
}

void init()
{
	glEnable (GL_DEPTH_TEST);

	glewInit();

	pngSetStandardOrientation(1);

	duckid = pngBind("duck.png", PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_REPEAT, GL_LINEAR, GL_LINEAR); // rgba
	doubleid = pngBind("2x.png", PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, duckid);

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, doubleid);
}

void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (400,400);
	mainwin = glutCreateWindow ("duck");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc (50, timer, 0);
	viewer = ViewerNew (mainwin, 0,0,400,400, content);
	
	init();
	glutKeyboardFunc(keyboard);

	glutMainLoop();
}
