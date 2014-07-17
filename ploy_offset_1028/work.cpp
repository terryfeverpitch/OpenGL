// changing polygon order
//

#include "gluit.h"
#include "gl/glpng.h"

int mainwin, viewer;
void display(){}
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}

void content()
{
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	// part1: two overlapping rectangles
	glColor3f (1,0,0);
	glRecti (-2,-2, 1,1);
	
	glEnable (GL_POLYGON_OFFSET_FILL);
	glPolygonOffset (.2,.2);

	glColor3f (0,0,1);
	glPushMatrix();
	glTranslatef (1,1,0);
	glRecti (-2,-2, 1,1);
	glPopMatrix();
	glDisable (GL_POLYGON_OFFSET_FILL);

	// part2: outlining icosahedron
	
	glPushAttrib (GL_ALL_ATTRIB_BITS);
	glColor3f (1,1,1);
	glPushMatrix();
	glTranslatef (2,2,2);
	glutSolidIcosahedron ();
	glPopMatrix();

	glPopAttrib();
	glPushAttrib (GL_ALL_ATTRIB_BITS);
		glColor3f (0,0,0);
		glEnable (GL_POLYGON_OFFSET_LINE);
		glPolygonOffset (-.4,-.4);
		glPolygonMode (GL_FRONT, GL_LINE);//polygon mode
		glPushMatrix();
		glTranslatef (2,2,2);
		glutSolidIcosahedron ();
		glPopMatrix();
	glPopAttrib();

	glutSwapBuffers();
}

void init()
{
	glClearColor (.4,.4,.4,1);
	glEnable (GL_DEPTH_TEST);
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
	ViewerExtent (viewer, 8);
	glutMainLoop();
}