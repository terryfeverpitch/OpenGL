#include <gl/glut.h>
#include <math.h>

#define	beamImageWidth 32
GLubyte beamImage[beamImageWidth][4];
static GLfloat zequalzero[] = {0.0, 0.0, 1.0, 0.0};
static GLuint texName;

//
// 1D texture [0..beamImageWidth-1] = [1,1,1,alpha]
// alpha: exponentially decay
//
static void makeBeamImage(void)
{
   for (int j = 0; j < beamImageWidth; j++) {
      beamImage[j][0] = (GLubyte) 255;
      beamImage[j][1] = (GLubyte) 255;
      beamImage[j][2] = (GLubyte) 255;
//      beamImage[j][3] = (GLubyte) (255.*(beamImageWidth-j)/beamImageWidth);
	  beamImage[j][3] = (GLubyte) 255*exp(-(float)4*j/beamImageWidth);
   }
}

void inittex (void)
{
	int i, j;

	makeBeamImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_1D, texName);

	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, beamImageWidth, 0,
		            GL_RGBA, GL_UNSIGNED_BYTE, beamImage);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, zequalzero);   // the height from XZ plane

	glEnable(GL_TEXTURE_GEN_S);
}
