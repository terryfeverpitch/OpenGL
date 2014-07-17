#include <gl/glut.h>

// cylinder of radius 0.5 and height 1
// ends at Z=0 and Z=1 
int init_cyl()
{
	GLUquadricObj *qobj;
	qobj = gluNewQuadric();

	// default GLU cylinder: along z-axis [0,height]
	// default GLU disk: on XY plane
	//gluQuadricDrawStyle(qobj, GLU_LINE); /* all polygons wireframe */

	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluQuadricNormals(qobj, GLU_SMOOTH);  // DEFAULT
	gluQuadricOrientation (qobj, GLU_OUTSIDE);

	GLuint cyl = glGenLists (1);
	glNewList(cyl, GL_COMPILE);
	   gluCylinder(qobj, 0.25, 1.5, 1.0, 15, 1);
	glEndList();

	return cyl;
}
