#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

#include "gluit.h"
#include "gl/glpng.h"
// textures
unsigned int FloorTexture;
unsigned int CubeTexture;

float Plane[4] = {0,1,0,0};			// the plane is simple here, this is the normal for the plane, 0,1,0
float FloorColor[4] = { 1.0f, 1.0f, 1.0f, 0.6f };	// set up the floor's color
float CubeColor[4] = { 1.0f, 1.0f, 1.0f, 1.0 };		// set the color of the cube's surface

// light
float LightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
float LightDiffuse[] = {1.0, 1.0, 1.0, 1.0}; 
float LightSpecular[] = {1.0, 1.0, 1.0, 1.0};
float LightPosition[] = {2.0f, 3.1f, 2.0f, 1.0f};

// shadow matrix
float fShadowMatrix[16];

// for rotating the cube
float fRot = 0;

void LoadTexture(char *filename, GLuint &texture);
void DrawCube(float x, float y, float z);
void DrawFloor(float fCenterX, float fCenterY, float fCenterZ);

int viewer, mainwin;

void SetShadowMatrix(float fDestMat[16],float fLightPos[4],float fPlane[4])
{
	float dot;

	// dot product of plane and light position
	dot =	fPlane[0] * fLightPos[0] + 
			fPlane[1] * fLightPos[1] + 
			fPlane[2] * fLightPos[2] + 
			fPlane[3] * fLightPos[3];

	// first column
	fDestMat[0] = dot - fLightPos[0] * fPlane[0];
	fDestMat[4] = 0.0f - fLightPos[0] * fPlane[1];
	fDestMat[8] = 0.0f - fLightPos[0] * fPlane[2];
	fDestMat[12] = 0.0f - fLightPos[0] * fPlane[3];

	// second column
	fDestMat[1] = 0.0f - fLightPos[1] * fPlane[0];
	fDestMat[5] = dot - fLightPos[1] * fPlane[1];
	fDestMat[9] = 0.0f - fLightPos[1] * fPlane[2];
	fDestMat[13] = 0.0f - fLightPos[1] * fPlane[3];

	// third column
	fDestMat[2] = 0.0f - fLightPos[2] * fPlane[0];
	fDestMat[6] = 0.0f - fLightPos[2] * fPlane[1];
	fDestMat[10] = dot - fLightPos[2] * fPlane[2];
	fDestMat[14] = 0.0f - fLightPos[2] * fPlane[3];

	// fourth column
	fDestMat[3] = 0.0f - fLightPos[3] * fPlane[0];
	fDestMat[7] = 0.0f - fLightPos[3] * fPlane[1];
	fDestMat[11] = 0.0f - fLightPos[3] * fPlane[2];
	fDestMat[15] = dot - fLightPos[3] * fPlane[3];
}

static void DrawCube(float x, float y, float z)
{
	// some material properties for the cube
	glMaterialfv(GL_FRONT, GL_SPECULAR, CubeColor);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, CubeColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0);

	// set up the cube's texture
	glBindTexture(GL_TEXTURE_2D, CubeTexture);

	glBegin(GL_QUADS);
		// front
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f+x, -1.0f+y, 1.0f+z);
		glTexCoord2f(1.0, 0.0); glVertex3f(1.0f+x, -1.0f+y, 1.0f+z);
		glTexCoord2f(1.0, 1.0); glVertex3f(1.0f+x, 1.0f+y, 1.0f+z);
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0f+x, 1.0f+y, 1.0f+z);

		// back
		glNormal3f(0.0, 0.0, -1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(1.0f+x, -1.0f+y, -1.0f+z);
		glTexCoord2f(1.0, 0.0); glVertex3f(-1.0f+x, -1.0f+y, -1.0f+z);
		glTexCoord2f(1.0, 1.0); glVertex3f(-1.0f+x, 1.0f+y, -1.0f+z);
		glTexCoord2f(0.0, 1.0); glVertex3f(1.0f+x, 1.0f+y, -1.0f+z);

		// top
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f+x, 1.0f+y, 1.0f+z);
		glTexCoord2f(1.0, 0.0); glVertex3f(1.0f+x, 1.0f+y, 1.0f+z);
		glTexCoord2f(1.0, 1.0); glVertex3f(1.0f+x, 1.0f+y, -1.0f+z);
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0f+x, 1.0f+y, -1.0f+z);

		// bottom
		glNormal3f(0.0, -1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f+x, -1.0f+y, -1.0f+z);
		glTexCoord2f(1.0, 0.0); glVertex3f(1.0f+x, -1.0f+y, -1.0f+z);
		glTexCoord2f(1.0, 1.0); glVertex3f(1.0f+x, -1.0f+y, 1.0f+z);
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0f+x, -1.0f+y, 1.0f+z);

		// left
		glNormal3f(-1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f+x, -1.0f+y, -1.0f+z);
		glTexCoord2f(1.0, 0.0); glVertex3f(-1.0f+x, -1.0f+y, 1.0f+z);
		glTexCoord2f(1.0, 1.0); glVertex3f(-1.0f+x, 1.0f+y, 1.0f+z);
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0f+x, 1.0f+y, -1.0f+z);

		// right
		glNormal3f(1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(1.0f+x, -1.0f+y, 1.0f+z);
		glTexCoord2f(1.0, 0.0); glVertex3f(1.0f+x, -1.0f+y, -1.0f+z);
		glTexCoord2f(1.0, 1.0); glVertex3f(1.0f+x, 1.0f+y, -1.0f+z);
		glTexCoord2f(0.0, 1.0); glVertex3f(1.0f+x, 1.0f+y, 1.0f+z);

	glEnd();
}

void DrawFloor(float fCenterX, float fCenterY, float fCenterZ)
{
	glMaterialfv(GL_FRONT, GL_SPECULAR, FloorColor);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, FloorColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 200.0);

	// set up blending so we can see the reflected cube through the
	// surface, and thus create the illusion of reflection

	glBindTexture(GL_TEXTURE_2D, FloorTexture);

	glBegin(GL_QUADS);
		glNormal3f(0.0, 1.0, 0.0);

		// to have lighting effects show up at all, we need to draw the
		// surface as a lot of quads, not just one
		float x = fCenterX-5.0f, z = fCenterZ-7.0f;

		for (GLint i = 0; i < 10; i++, x += 1.0)
		{
			for (GLint j = 0; j < 14; j++, z += 1.0)
			{
			  // draw the plane slightly offset so the shadow shows up
			  glTexCoord2f(0.0, 0.0);
			  glVertex3f(x,			fCenterY, z);
			  glTexCoord2f(1.0, 0.0);
			  glVertex3f(x + 1.0f,	fCenterY, z);
			  glTexCoord2f(1.0, 1.0);
			  glVertex3f(x + 1.0f,	fCenterY, z + 1.0f);
			  glTexCoord2f(0.0, 1.0);
			  glVertex3f(x,			fCenterY, z + 1.0f);
			}

			z = fCenterZ-7.0f;
		}

	glEnd();
}

void LoadTexture(char *filename, GLuint &texture)
{
  GLuint id;

  pngSetStandardOrientation (1);
  id = pngBind(filename, PNG_BUILDMIPMAPS, PNG_SOLID, NULL, GL_REPEAT, GL_LINEAR, GL_LINEAR);

  if (id != 0)
	  texture = id;
} // end LoadTexture()

void content (void)
{
	glClear (GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

	// step 1
	// turning off writing to the color buffer and depth buffer so we only 
	// write to stencil buffer
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);

	// enable stencil buffer
	glEnable(GL_STENCIL_TEST);
	// write a one to the stencil buffer everywhere we are about to draw
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	// this is to always pass a one to the stencil buffer where we draw
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
				
	// render the plane which the shadow will be on
	// color and depth buffer are disabled, only the stencil buffer
	// will be modified
	DrawFloor(0,0,0);

	// turn the color and depth buffers back on
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);


	// until stencil test is diabled, only write to areas where the
	// stencil buffer has a one. This is to draw the shadow only on
	// the floor.
	glStencilFunc(GL_EQUAL, 1, 0xFF);

	// don't modify the contents of the stencil buffer
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	DrawFloor(0,0,0);
	// end step 1

	// draw the shadow
	glPushMatrix();
	// draw the shadow as black, blended with the surface, with no lighting, and not
	// preforming the depth test
		// step 3
		glColor4f(0.0, 0.0, 0.0, 0.5f);
		// step 3
		glDisable(GL_TEXTURE_2D);
		// step 3
		glDisable(GL_LIGHTING);
		// step 4
		glDisable(GL_DEPTH_TEST);
		// step 5
		glEnable(GL_BLEND);
		// make sure that we don't draw at any raster position more than once. Using GL_INCR
		// here is smart because the value of the area in stencil buffer wich will be drawn to
		// increases to 2, and earlier we have set the stencil buffer function to only write to
		// areas with a one. This means that a shadow will not be draw 2 times on the same area!
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);


		// project the cube through the shadow matrix
		// step 6
		glMultMatrixf(fShadowMatrix);
		glPushMatrix();
			glRotatef(fRot,0,1,0);
			DrawCube(0,2,0);
		glPopMatrix();
		// end step 6

		// step 7
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);
		// end step 7
	glPopMatrix();

	glDisable(GL_STENCIL_TEST);
	// 
	// draw the cube normally
	glPushMatrix();
		glRotatef(fRot,0,1,0);
		DrawCube(0,2,0);
	glPopMatrix();


	glutSwapBuffers();
}
void init (void)
{
	// what color to clear to in color buffer
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Enable a single OpenGL light.
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	pngSetStandardOrientation (1);
	FloorTexture = pngBind("marble.png", PNG_BUILDMIPMAPS, PNG_SOLID, NULL, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	CubeTexture = pngBind("cubetexture.png", PNG_BUILDMIPMAPS, PNG_SOLID, NULL, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// step 2
	SetShadowMatrix(fShadowMatrix, LightPosition, Plane);
}

void display (void) {}
void reshape (int w, int h) 
{
    glViewport (0,0,w,h);
    ViewerReshape (viewer);
}
void idle (void)
{
	fRot += 0.8f;
	if (fRot>=360)
	 	fRot = 0;
	ViewerRedisplay (viewer);
}

int main (int argc, char** argv)
{
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE |GLUT_RGB|GLUT_DEPTH|GLUT_STENCIL);
    glutInitWindowSize (400,400);
    
    mainwin = glutCreateWindow (argv[0]);
    glutDisplayFunc (display);
    glutReshapeFunc (reshape);
    viewer = ViewerNew (mainwin, 0,0,400,400, content);
    init();
	glutIdleFunc (idle);
    
    int stencilbits;
	glGetIntegerv (GL_STENCIL_BITS, &stencilbits);
	printf ("stencil %d bits\n", stencilbits);

	glutMainLoop();
}

    