#include <windows.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

extern void ProcessCapture(int,int=10);

static int ww=512,wh=512;

#include "gluit.h"
#include "gl/glpng.h"

unsigned int FloorTexture;

bool open = false;
bool close = false;
GLuint graph[20];
float zside,xside;
float Plane[4] = {0,1,0,0};		
float FloorColor[4] = { 1.0f, 1.0f, 1.0f, 0.6f };	
float LightPosition[] = {0.0f, 30.0f, 20.0f, 1.0f};

float fShadowMatrix[16];

float angle = 0;
float fRot = 0;

void DrawFloor(float fCenterX, float fCenterY, float fCenterZ);

int viewer, mainwin;


void SetShadowMatrix(float fDestMat[16],float fLightPos[4],float fPlane[4])
{
	float dot;

	dot =	fPlane[0] * fLightPos[0] + 
			fPlane[1] * fLightPos[1] + 
			fPlane[2] * fLightPos[2] + 
			fPlane[3] * fLightPos[3];

	fDestMat[0] = dot - fLightPos[0] * fPlane[0];
	fDestMat[4] = 0.0f - fLightPos[0] * fPlane[1];
	fDestMat[8] = 0.0f - fLightPos[0] * fPlane[2];
	fDestMat[12] = 0.0f - fLightPos[0] * fPlane[3];

	fDestMat[1] = 0.0f - fLightPos[1] * fPlane[0];
	fDestMat[5] = dot - fLightPos[1] * fPlane[1];
	fDestMat[9] = 0.0f - fLightPos[1] * fPlane[2];
	fDestMat[13] = 0.0f - fLightPos[1] * fPlane[3];

	fDestMat[2] = 0.0f - fLightPos[2] * fPlane[0];
	fDestMat[6] = 0.0f - fLightPos[2] * fPlane[1];
	fDestMat[10] = dot - fLightPos[2] * fPlane[2];
	fDestMat[14] = 0.0f - fLightPos[2] * fPlane[3];

	fDestMat[3] = 0.0f - fLightPos[3] * fPlane[0];
	fDestMat[7] = 0.0f - fLightPos[3] * fPlane[1];
	fDestMat[11] = 0.0f - fLightPos[3] * fPlane[2];
	fDestMat[15] = dot - fLightPos[3] * fPlane[3];
}

void milk(){
	
	float a0[3]={-3.5,0,3.5};
	float a1[3]={3.5,0,3.5};
	float a2[3]={3.5,0,-3.5};
	float a3[3]={-3.5,0,-3.5};
	float a4[3]={-3.5,7,3.5};
	float a5[3]={3.5,7,3.5};
	float a6[3]={3.5,7,-3.5};
	float a7[3]={-3.5,7,-3.5};
	float a8[3]={-3.5,9.5,0.1};
	float a9[3]={3.5,9.5,0.1+zside};
	float a10[3]={-3.5,11,0.1};
	float a11[3]={3.5,11,0.1+zside};
	float a12[3]={-3.5,9.5,-0.1};
	float a13[3]={3.5,9.5,-0.1-zside};
	float a14[3]={-3.5,11,-0.1};
	float a15[3]={3.5,11,-0.1-zside};
	float a16[3]={0,9.5,0.1};
	float a17[3]={0+xside,9.5,0.1};
	float a18[3]={0,11,0.1};
	float a19[3]={0+xside,11,0.1};
	float a20[3]={0,9.5,-0.1};
	float a21[3]={0+xside,9.5,-0.1};
	float a22[3]={0,11,-0.1};
	float a23[3]={0+xside,11,-0.1};

	//out

	glBindTexture(GL_TEXTURE_2D, graph[0]);
	glBegin(GL_QUADS);
	glNormal3f(0,-1,0);
	glTexCoord2f(0,0);glVertex3fv(a0);
	glTexCoord2f(1,0);glVertex3fv(a3);
	glTexCoord2f(1,1);glVertex3fv(a2);
	glTexCoord2f(0,1);glVertex3fv(a1);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, graph[1]); 
	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	glTexCoord2f(1,0);glVertex3fv(a0);
	glTexCoord2f(1,1);glVertex3fv(a1);
	glTexCoord2f(0,1);glVertex3fv(a5);
	glTexCoord2f(0,0);glVertex3fv(a4);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, graph[1]);
	glBegin(GL_QUADS);
	glNormal3f(0,0,-1);
	glTexCoord2f(1,0);glVertex3fv(a2);
	glTexCoord2f(1,1);glVertex3fv(a3);
	glTexCoord2f(0,1);glVertex3fv(a7);
	glTexCoord2f(0,0);glVertex3fv(a6);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, graph[3]);
	glBegin(GL_POLYGON);
	glNormal3f(0,0.707,0.707);
	glTexCoord2f(1,0);glVertex3fv(a4);
	glTexCoord2f(1,1);glVertex3fv(a5);
	glTexCoord2f(0,1);glVertex3fv(a9);
	glTexCoord2f(0,0.5);glVertex3fv(a16);
	glTexCoord2f(0,0);glVertex3fv(a8);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, graph[3]);
	glBegin(GL_POLYGON);
	glNormal3f(0,0.707,-0.707);
	glTexCoord2f(1,0);glVertex3fv(a6);
	glTexCoord2f(1,1);glVertex3fv(a7);
	glTexCoord2f(0,1);glVertex3fv(a12);
	glTexCoord2f(0,0.5);glVertex3fv(a20);
	glTexCoord2f(0,0);glVertex3fv(a13);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, graph[6]);
	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	glTexCoord2f(1,0);glVertex3fv(a20);
	glTexCoord2f(1,1);glVertex3fv(a9);
	glTexCoord2f(0,1);glVertex3fv(a11);
	glTexCoord2f(0,0);glVertex3fv(a22);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, graph[4]);
	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	glTexCoord2f(1,0);glVertex3fv(a8);
	glTexCoord2f(1,1);glVertex3fv(a20);
	glTexCoord2f(0,1);glVertex3fv(a22);
	glTexCoord2f(0,0);glVertex3fv(a10);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, graph[4]);
	glBegin(GL_QUADS);
	glNormal3f(0,0,-1);
	glTexCoord2f(1,0);glVertex3fv(a9);
	glTexCoord2f(1,1);glVertex3fv(a20);
	glTexCoord2f(0,1);glVertex3fv(a22);
	glTexCoord2f(0,0);glVertex3fv(a11);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, graph[6]);
	glBegin(GL_QUADS);
	glNormal3f(0,0,-1);
	glTexCoord2f(1,0);glVertex3fv(a16);
	glTexCoord2f(1,1);glVertex3fv(a12);
	glTexCoord2f(0,1);glVertex3fv(a14);
	glTexCoord2f(0,0);glVertex3fv(a18);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, graph[4]);
	glBegin(GL_QUADS);
	glNormal3f(0,0,-1);
	glTexCoord2f(1,0);glVertex3fv(a13);
	glTexCoord2f(1,1);glVertex3fv(a16);
	glTexCoord2f(0,1);glVertex3fv(a18);
	glTexCoord2f(0,0);glVertex3fv(a15);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, graph[6]);
	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	glTexCoord2f(1,0);glVertex3fv(a20);
	glTexCoord2f(1,1);glVertex3fv(a13);
	glTexCoord2f(0,1);glVertex3fv(a15);
	glTexCoord2f(0,0);glVertex3fv(a22);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, graph[4]);
	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	glTexCoord2f(1,0);glVertex3fv(a9);
	glTexCoord2f(1,1);glVertex3fv(a21);
	glTexCoord2f(0,1);glVertex3fv(a23);
	glTexCoord2f(0,0);glVertex3fv(a11);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, graph[4]);
	glBegin(GL_QUADS);
	glNormal3f(0,0,-1);
	glTexCoord2f(1,0);glVertex3fv(a11);
	glTexCoord2f(1,1);glVertex3fv(a23);
	glTexCoord2f(0,1);glVertex3fv(a21);
	glTexCoord2f(0,0);glVertex3fv(a9);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, graph[6]);
	glBegin(GL_QUADS);
	glNormal3f(0,0,-1);
	glTexCoord2f(1,0);glVertex3fv(a21);
	glTexCoord2f(1,1);glVertex3fv(a13);
	glTexCoord2f(0,1);glVertex3fv(a15);
	glTexCoord2f(0,0);glVertex3fv(a23);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, graph[6]);
	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	glTexCoord2f(1,0);glVertex3fv(a19);
	glTexCoord2f(1,1);glVertex3fv(a15);
	glTexCoord2f(0,1);glVertex3fv(a13);
	glTexCoord2f(0,0);glVertex3fv(a17);
	glEnd();

	glNormal3f(1,0,0);
	glVertex3fv(a9);
	glVertex3fv(a13);
	glVertex3fv(a15);
	glVertex3fv(a11);

	glNormal3f(-1,0,0);
	glVertex3fv(a8);
	glVertex3fv(a10);
	glVertex3fv(a14);
	glVertex3fv(a12);

	glNormal3f(0,1,0);
	glVertex3fv(a10);
	glVertex3fv(a11);
	glVertex3fv(a15);
	glVertex3fv(a14);

	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, graph[9]);
	glBegin(GL_QUADS);
	glNormal3f(1,0,0);
	glTexCoord2f(1,0);glVertex3fv(a1);
	glTexCoord2f(1,1);glVertex3fv(a2);
	glTexCoord2f(0,1);glVertex3fv(a6);
	glTexCoord2f(0,0);glVertex3fv(a5);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, graph[10]);
	glBegin(GL_QUADS);
	glNormal3f(-1,0,0);
	glTexCoord2f(1,0);glVertex3fv(a3);
	glTexCoord2f(1,1);glVertex3fv(a0);
	glTexCoord2f(0,1);glVertex3fv(a4);
	glTexCoord2f(0,0);glVertex3fv(a7);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, graph[7]);
	glBegin(GL_QUADS);
	glNormal3f(0.707,0.707,0);
	glTexCoord2f(1,0);glVertex3fv(a5);
	glTexCoord2f(1,1);glVertex3fv(a6);
	glTexCoord2f(0,0.5);glVertex3fv(a21);
	glTexCoord2f(0,0.5);glVertex3fv(a17);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, graph[8]);
	glBegin(GL_QUADS);
	glNormal3f(-0.707,0.707,0);
	glTexCoord2f(1,0);glVertex3fv(a7);
	glTexCoord2f(1,1);glVertex3fv(a4);
	glTexCoord2f(0,0.5);glVertex3fv(a16);
	glTexCoord2f(0,0.5);glVertex3fv(a20);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, graph[7]);
	glBegin(GL_TRIANGLES);
	glNormal3f(0.707,0.707,0);
	glTexCoord2f(1,0);glVertex3fv(a5);
	glTexCoord2f(0,0.5);glVertex3fv(a17);
	glTexCoord2f(0,0);glVertex3fv(a9);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, graph[7]);
	glBegin(GL_TRIANGLES);
	glNormal3f(0,1,0);
	glTexCoord2f(1,1);glVertex3fv(a6);
	glTexCoord2f(0,1);glVertex3fv(a13);
	glTexCoord2f(0,0.5);glVertex3fv(a21);
	glEnd();

	glutPostRedisplay();
}


void DrawFloor(float fCenterX, float fCenterY, float fCenterZ)
{
	glMaterialfv(GL_FRONT, GL_SPECULAR, FloorColor);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, FloorColor);
	glMaterialf(GL_FRONT, GL_SHININESS, 200.0);

	glBindTexture(GL_TEXTURE_2D, FloorTexture);

	glBegin(GL_QUADS);
		glNormal3f(0.0, 1.0, 0.0);

		float x = fCenterX-5.0f, z = fCenterZ-7.0f;

		for (GLint i = 0; i < 30; i++, x += 1.0)
		{
			for (GLint j = 0; j < 42; j++, z += 1.0)
			{
			  
			  glTexCoord2f(0.0, 0.0);
			  glVertex3f(x-10,			fCenterY, z-20);
			  glTexCoord2f(1.0, 0.0);
			  glVertex3f(x-10 + 1.0f,	fCenterY, z-20);
			  glTexCoord2f(1.0, 1.0);
			  glVertex3f(x-10 + 1.0f,	fCenterY, z -19.0f);
			  glTexCoord2f(0.0, 1.0);
			  glVertex3f(x-10,			fCenterY, z -19.0f);
			}

			z = fCenterZ-7.0f;
		}

	glEnd();
}

void content (void)
{
	glClear (GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
	
	glLightf (GL_LIGHT0, GL_SPOT_CUTOFF, 90);        
	glLightf (GL_LIGHT0, GL_SPOT_EXPONENT, 2); 

	glPushAttrib(GL_ENABLE_BIT);
	glDisable (GL_LIGHTING);
	
	float dir[3] = {0,-1,-1};
	
	glPushMatrix();
	glRotatef(angle,0,1,0);
	glTranslatef (0,15,10);
	glutSolidCone (1,1,20,20);

	glPopAttrib();
	glLightfv (GL_LIGHT0, GL_POSITION, LightPosition);
	glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, dir);

	glPopMatrix();
	glEnable (GL_LIGHTING);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);

	
	glEnable(GL_STENCIL_TEST);

	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);			
	
	DrawFloor(0,0,0);

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

	glStencilFunc(GL_EQUAL, 1, 0xFF);

	
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	DrawFloor(0,0,0);
	
	glPushMatrix();
	
		glColor4f(0.0, 0.0, 0.0, 0.5f);
		
		glDisable(GL_TEXTURE_2D);
		
		glDisable(GL_LIGHTING);
		
		glDisable(GL_DEPTH_TEST);
		
		glEnable(GL_BLEND);
		
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		
		glPushMatrix();
		glRotatef(angle,0,1,0);
		glMultMatrixf(fShadowMatrix);
		
		glPushMatrix();
			glRotatef(fRot,0,1,0);
			milk();
			
		glPopMatrix();
		glPopMatrix();
		
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);
	
	glPopMatrix();

	glDisable(GL_STENCIL_TEST);
	
	glPushMatrix();
		glRotatef(fRot,0,1,0);
		milk();
		
	glPopMatrix();

	float red[4] = {0,0,0,1};
	float yellow[4] = {0,0,0,1};
	glMaterialfv (GL_FRONT, GL_DIFFUSE, red);
	glMaterialfv (GL_FRONT, GL_SPECULAR, yellow);
	glMateriali (GL_FRONT, GL_SHININESS, 128);



	glutSwapBuffers();
}
void init (void)
{
	
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	pngSetStandardOrientation (1);
	FloorTexture = pngBind("marble.png", PNG_BUILDMIPMAPS, PNG_SOLID, NULL, GL_REPEAT, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	
	pngInfo info;
	graph[0] = pngBind("6.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
	graph[1] = pngBind("2.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
	graph[2] = pngBind("3.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
	graph[3] = pngBind("4.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
	graph[4] = pngBind("7.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
	graph[5] = pngBind("1.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
	graph[6] = pngBind("8.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
	graph[7] = pngBind("9.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
	graph[8] = pngBind("10.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
	graph[9] = pngBind("11.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
	graph[10] = pngBind("12.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SetShadowMatrix(fShadowMatrix, LightPosition, Plane);
}

void display (void) {}
void reshape (int w, int h) 
{
    ww=w; wh=h;
	glViewport(0,0,ww,wh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float fovy = 70;
	gluPerspective(fovy,1,1,50);
}

void time(int a){
	
	angle += 1.5;
	fRot += 0.8f;
	if (fRot>=360)
	 	fRot = 0;

	if(open == true && zside < 1.5)
		zside += 0.1;	
	
	if(open == true && xside < 6 && zside > 1.5)
		xside += 0.2;
		
	if(close == true && zside > 0.15 && xside < 0.15)
		zside -= 0.1;
	
	if(close == true && xside > 0.15)
		xside -= 0.2;
	
	glutTimerFunc(10,time,0);
}

void keyboard(unsigned char key, int x, int y){
	
	switch(key){
	case 115 :
		open = true;
		close = false;
		break;
	case 100 :
		close = true;
		open = false;
		break;
	}
	 
}

int main (int argc, char** argv)
{
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE |GLUT_RGB|GLUT_DEPTH|GLUT_STENCIL);
    glutInitWindowSize (400,400);
    
    mainwin = glutCreateWindow (argv[0]);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(10,time,0);
    glutDisplayFunc (display);
    glutReshapeFunc (reshape);
	
	glEnable (GL_COLOR_MATERIAL);
	glColorMaterial (GL_FRONT, GL_DIFFUSE);

    viewer = ViewerNew (mainwin, 0,0,400,400, content);
	ViewerCenter(viewer,0,5,0);
	ViewerExtent(viewer,40);
    init();
	
	glutMainLoop();
}

    