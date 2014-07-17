#include "GL/gluit.h"

int mainwin, viewer;
void display(){}
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}

void drawscene()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	float red[] = {1,0,0,1};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	glutSolidTeapot(1.0);
	glPopAttrib();
	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	float green[] = {0,1,0,1};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glTranslatef(1,3,1);
	glutSolidTeapot(1.0);
	glPopMatrix();
	glPopAttrib();
}

void content()
{
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	drawscene();

	glutSwapBuffers();
}
void init()
{
	glEnable (GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void processHits(GLint hits, GLuint buffer[])
{
   unsigned int i, j;
   GLuint names, *ptr;

   printf("hits = %d\n", hits);
   ptr = (GLuint *) buffer;
   for (i = 0; i < hits; i++) {  /* for each hit  */
      names = *ptr;
      printf(" number of names for hit = %d\n", names); ptr++;
      printf("  z1 is %g;", (float) *ptr/0xffffffff); ptr++;
      printf(" z2 is %g\n", (float) *ptr/0xffffffff); ptr++;
      printf("   the name is ");
      for (j = 0; j < names; j++) {  /* for each name */
         printf("%d ", *ptr); ptr++;
      }
      printf("\n");
   }
}

void clicker(int button, )
{

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
	ViewerAddClicker(viewer, clicker);

	glutMainLoop();
}
