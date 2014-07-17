//
//  0. download/install/test glew
//  1. tune the parameter for unlit wall
//  2. add a yellow teapot (no texture) on the right side
//



#include <gl/glew.h> 
#include "gl/gluit.h"
#include "gl/glpng.h"
#pragma comment (lib, "glew32.lib")
int mainwin, viewer;
void display(){}
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}

GLuint brickid, spotid;
static int light;

void content()
{
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	if (light)
		glColor3f(1, 1, 1);
	else
		glColor3f(.4, .4, .4);

	glBegin (GL_QUADS);
	glMultiTexCoord2f (GL_TEXTURE0, 0,0); glMultiTexCoord2f (GL_TEXTURE1, 0,0); glVertex2i (-1,0);
	glMultiTexCoord2f (GL_TEXTURE0, 1,0); glMultiTexCoord2f (GL_TEXTURE1, 1,0); glVertex2i (1,0);
	glMultiTexCoord2f (GL_TEXTURE0, 1,1); glMultiTexCoord2f (GL_TEXTURE1, 1,1); glVertex2i (1,2);
	glMultiTexCoord2f (GL_TEXTURE0, 0,1); glMultiTexCoord2f (GL_TEXTURE1, 0,1); glVertex2i (-1,2);
	glEnd();
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		//glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0); glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE1); glDisable(GL_TEXTURE_2D);
		float red[] ={1, 0, 0, 1};
		glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
		glPushMatrix();
			glTranslatef(2,0,0);
			glutSolidTeapot(1.0);
		glPopMatrix();
	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode (GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, 10, 0, 10);
	glMatrixMode (GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

	glActiveTexture(GL_TEXTURE0); glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE1); glDisable(GL_TEXTURE_2D);

	glDisable(GL_DEPTH_TEST);
	
	glColor3f(1,1,1);
	if(light)
		drawstr (1,1, "LIGHT ON");
	else
		drawstr (1,1, "LIGHT OFF");
	
	glMatrixMode (GL_PROJECTION); glPopMatrix();
	glMatrixMode (GL_MODELVIEW); glPopMatrix();

	/*glPushAttrib (GL_ALL_ATTRIB_BITS);
	glDisable (GL_LIGHTING);
	glColor3f(1,1,1);
	if(light)
		drawstr (1,1, "LIGHT ON");
	else
		drawstr (1,1, "LIGHT OFF");
	BEGIN_2D_OVERLAY(15,15);
	END_2D_OVERLAY();*/
	glPopAttrib();

	glutSwapBuffers();
}

void init()
{
	glClearColor (.4,.4,.4,1);
	glEnable (GL_DEPTH_TEST);
	//glEnable();
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	pngSetStandardOrientation (1);

	glActiveTexture (GL_TEXTURE0);
	glEnable (GL_TEXTURE_2D);
	brickid = pngBind("brick.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);

	glActiveTexture (GL_TEXTURE1);
	glDisable (GL_TEXTURE_2D);
	spotid = pngBind("spot.png", PNG_NOMIPMAP, PNG_SOLID, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);

}

void keyboard (unsigned char key, int x, int y)
{

	if (key == 'l' || key == 'L') {
		light ^= 1;
	}

	glActiveTexture (GL_TEXTURE1);
	if (light) 
		glEnable (GL_TEXTURE_2D);
	else
		glDisable (GL_TEXTURE_2D);

	glutPostRedisplay();
}

void main (int argc, char** argv)
{
	glutInit (&argc, argv);

	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (400,400);
	mainwin = glutCreateWindow ("LightMap (l)");
	
	glewInit();  // call glewInit after the first window has been created
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	viewer = ViewerNew (mainwin, 0,0,400,400, content);
	glutKeyboardFunc (keyboard);

	init();
	glutMainLoop();
}