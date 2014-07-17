#include "gluit.h"
#include <gl/glpng.h>
#include <stdio.h>

pngRawInfo rawinfo;
void init()
{
	pngSetStandardOrientation(1);
	pngLoadRaw("chiling-grey347.png", &rawinfo);
}

void display()
{
	glMatrixMode (GL_PROJECTION); glLoadIdentity(); gluOrtho2D (0,10,0,10);
	glMatrixMode (GL_MODELVIEW); glLoadIdentity(); 
	glRasterPos2i(0,0);
	glDrawPixels (rawinfo.Width, rawinfo.Height, GL_LUMINANCE, GL_UNSIGNED_BYTE, rawinfo.Data);

	glColor3f(1,1,0);
	drawstr (1,1, "UnpackAlignment Mode [1,2,4,8]");
	glFlush();
}

void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
	case '2':
	case '4':
	case '8':
		glPixelStorei (GL_UNPACK_ALIGNMENT, key-'0');
		glutPostRedisplay();
		char msg[50];
		sprintf (msg, "Unpack Mode: %c", key);
		glutSetWindowTitle (msg);
		break;
	}
}

void main (int argc, char** argv)
{
	glutInit (&argc,argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_SINGLE);
	init();
	glutInitWindowSize (rawinfo.Width, rawinfo.Height);

	glutCreateWindow ("image");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
}
