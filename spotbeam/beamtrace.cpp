#include <gl/glut.h>
#include <svl/svl.h>
extern GLuint cyl;
#include <iostream>
using namespace std;

void beamtrace (const Vec3 base, const Vec3 character)
{
	Vec3 p = norm (character - base);
	Vec3 y(0,1,0);

	y = norm (y - dot(y,p)*p);
	Vec3 x = cross (y,p);

	Mat4 mv(x[0], y[0], p[0], base[0], 
			x[1], y[1], p[1], base[1], 
			x[2], y[2], p[2], base[2], 
			0,       0,    0, 1); 
	Mat4 mvt = trans(mv);  // OpenGL matrix is column major; while C matrix is row major!
	
	glPushMatrix();
	glMultMatrixd (mvt.Ref());
	
	glScalef (1.5,1.5,len(base-character));
	glCallList (cyl);
	glPopMatrix();
}

