#include <stdio.h>
#include <stdlib.h>
#include "gluit.h"
#include "GL/glpng.h"

//declare the variables
int	  mainwin, viewer;
bool  frame = false;
bool  flag = true;		
bool  light = true;
float theta = 0;	//spotlight rotate degree
float drate = 5;	//rotate speed
int	  state = 0;	//0 = half opened, 1 = entire opened, 2 = half closed, 3 = entire closed

float LightAmbient[] = {0.1f, 0.1f, 0.1f, 1.0f};

float pos[4] = {0, 5, 10, 1};  //spotlight start position
float dir[3];				   //vector

float rate = 0.1;	   //open speed
float z_shift = rate;
float x_shift = rate;

GLuint image[10];

//functions
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}

void grid()
{
	glPushAttrib (GL_ENABLE_BIT);
	glDisable (GL_LIGHTING);
	glLineWidth (1.0);
    glColor3f (1,1,1);
	glBegin (GL_LINES);
	for (int i = -5; i <= 5; i++) 
	{
		//xz
		glVertex3i (i,0,-5); glVertex3i (i,0,5);
		glVertex3i (-5,0,i); glVertex3i (5,0,i);

		//xy
		/*glVertex3i (-5,i,0); glVertex3i (5,i,0);
		glVertex3i (i,-5,0); glVertex3i (i,5,0);*/
	}
	glEnd();
	glPopAttrib();
}

void drawLine(const float p1[],const  float p2[])
{
	glPushAttrib (GL_ENABLE_BIT);
	glLineWidth (3.0);
	glColor3f (1, 0, 0);
	glBegin(GL_LINES);
	glVertex3fv(p1);glVertex3fv(p2);
	glEnd();
	grid();
	glPopAttrib();
}

void drawTheOtherSide(const float p1[], const float p2[], const float p3[], const float p4[])
{
	glBegin(GL_QUADS);
	glVertex3fv(p1);
	glVertex3fv(p2);
	glVertex3fv(p3);
	glVertex3fv(p4);
	glEnd();
}

float* v_Normal(const float p1[], const  float p2[], const  float p3[])
{
	static float n[3], v1[3], v2[3];
	v1[0] = p2[0] - p1[0]; v1[1] = p2[1] - p1[1]; v1[2] = p2[2] - p1[2];
	v2[0] = p3[0] - p1[0]; v2[1] = p3[1] - p1[1]; v2[2] = p3[2] - p1[2];
	
	n[0] = v1[1] * v2[2] - v2[1] * v1[2];
	n[1] = v1[2] * v2[0] - v2[2] * v1[0];
	n[2] = v1[0] * v2[1] - v2[0] * v1[1];

	return n;
}

void drawTheOtherSide(const float p1[], const float p2[], const float p3[])
{
	glBegin(GL_TRIANGLES);
	glVertex3fv(p1);
	glVertex3fv(p2);
	glVertex3fv(p3);
	glEnd();
}

void drawRectangle(const float a1[], const float a2[], const float a3[], const float a4[], // a1 to a4 are texcoord
				   const float p1[], const float p2[], const float p3[], const float p4[]) // p1 to p4 are vertex
{
	glBegin(GL_QUADS);
	glNormal3fv( v_Normal(p1, p2, p3) );
	glTexCoord2fv(a1); 	glVertex3fv(p1);
	glTexCoord2fv(a2); 	glVertex3fv(p2);
	glTexCoord2fv(a3); 	glVertex3fv(p3);
	glTexCoord2fv(a4); 	glVertex3fv(p4);
	glEnd();
}

void drawTriangle(const float a1[], const float a2[], const float a3[],
				  const float p1[], const float p2[], const float p3[])
{
	glBegin(GL_TRIANGLES);
	glNormal3fv( v_Normal(p1, p2, p3) );
	glTexCoord2fv(a1);  glVertex3fv(p1);
	glTexCoord2fv(a2);  glVertex3fv(p2);
	glTexCoord2fv(a3);  glVertex3fv(p3);
	glEnd();
}

//the texture coordinates
float a1[2] = {0, 0}; 
float a2[2] = {1, 0};
float a3[2] = {1, 1};
float a4[2] = {0, 1};
float a5[2] = {0.5, 1};
float a6[2] = {0.5, 0};

//basic points
float p1[3]  = { 0, 0, 0 };
float p2[3]  = { 5, 0, 0 };
float p3[3]  = { 5, 7, 0 };
float p4[3]  = { 0, 7, 0 };
float p5[3]  = { 5, 0, -5 };
float p6[3]  = { 5, 7, -5 };
float p7[3]  = { 0, 0, -5 };
float p8[3]  = { 0, 7, -5 };
float p9[3]  = { 2.5, 9.5, -2.5 };
float p10[3] = { 0, 9.5, -2.5};
float p11[3] = { 5, 9.5, -2.5};		// the feature point
float p12[3] = { 0, 10, -2.5};
float p13[3] = { 5, 10, -2.5};
float p14[3] = { 2.5, 10, -2.5 };	// the feature point

//animate points
float fp11[3] = { 5, 9.5, -2.5};	// p11 is separated into fp11 and bp11.
float bp11[3] = { 5, 9.5, -2.5};
float fp13[3] = { 5, 10, -2.5};		// p13 is separated into fp13 and bp13.
float bp13[3] = { 5, 10, -2.5};
float ap9[3]  = { 2.5, 9.5, -2.5 }; // create ap9 to animate. 
float ap14[3] = { 2.5, 10, -2.5 };	// create ap14 to animate.

void restart()
{
	printf("You can press space to pause or play.\n");
	printf("You can press '+' or '-' to change the spotlight rotated speed.\n");
	printf("You can press enter to restart.\n");
	printf("You can press delete to show or hide the frame.\n");
	printf("You can press number 0 to enable or disable the light.\n ");

	fp11[0] = p11[0]; fp11[1] = p11[1]; fp11[2] = p11[2]; //= { 5, 9.5, -2.5};	// p11 is separated into fp11 and bp11.
	bp11[0] = p11[0]; bp11[1] = p11[1]; bp11[2] = p11[2]; //= { 5, 9.5, -2.5};
    
	fp13[0] = p13[0]; fp13[1] = p13[1]; fp13[2] = p13[2];
	bp13[0] = p13[0]; bp13[1] = p13[1]; bp13[2] = p13[2];

	ap9[0] = p9[0]; ap9[1] = p9[1]; ap9[2] = p9[2];
	
	ap14[0] = p14[0]; ap14[1] = p14[1]; ap14[2] = p14[2];

	light = true;
	frame = false;
    flag = true;		
    theta = 0;
    drate = 5;
    state = 0;
    pos[0] = 0; pos[1] =5; pos[2] = 10; pos[3] = 1;
    rate = 0.1;	
    z_shift = rate;
    x_shift = rate;
}
void showFrame()
{
	drawLine(p1, p2); drawLine(p2, p3); drawLine(p3, p4); drawLine(p4, p1);
	drawLine(p2, p5); drawLine(p5, p6); drawLine(p3, p6); drawLine(p3, p2);
	drawLine(p5, p7); drawLine(p7, p8); drawLine(p8, p6); drawLine(p6, p5);
	drawLine(p7, p1); drawLine(p1, p4); drawLine(p4, p8); drawLine(p8, p7);
	drawLine(p1, p2); drawLine(p2, p5); drawLine(p5, p7); drawLine(p7, p1);
	drawLine(p4, p3); drawLine(p9, p10); drawLine(p10, p4);
	drawLine(p4, p3); drawLine(p9, p10); drawLine(p10, p8);
	drawLine(p3, fp11); drawLine(fp11, p9);
	drawLine(p9, bp11); drawLine(bp11, p6);
	drawLine(p3, ap9); drawLine(p6, ap9); drawLine(p6, p3);
	drawLine(p4, p9); drawLine(p9, p8); drawLine(p8, p4);
	drawLine(p10, p9); drawLine(p12, p10); drawLine(p12, p14); drawLine(p9, p14);
	//half opened
	drawLine(fp13, p14); drawLine(bp13, p14);
	drawLine(fp11, fp13); drawLine(bp11, bp13);
	//totally open
	drawLine(ap9, ap14);
	drawLine(ap9, fp11); drawLine(ap9, bp11); drawLine(ap14, fp13); drawLine(ap14, bp13);
}

void content()
{
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	

	if( light )
	{
		if( frame )
		{
			glDisable (GL_LIGHTING);
			showFrame();
			glEnable (GL_LIGHTING);
		}
		glPushMatrix();
			glTranslatef(p9[0], p9[1], p9[2]);
			glRotatef(theta, 0, 1, 0);	
			dir[0] = 0 - pos[0]; dir[1] = 2 - pos[1]; dir[2] = 0 - pos[2];
			glLightfv (GL_LIGHT0, GL_POSITION, pos);
			glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, dir);		
			glTranslatef(pos[0], pos[1], pos[2]);
			
			glEnable (GL_LIGHTING);
			glEnable (GL_LIGHT0);
			
			glDisable (GL_LIGHTING);
			glutSolidCone (1,1,10,10);
			glEnable (GL_LIGHTING);
		glPopMatrix();
	}
	else
	{
		if( frame )
			showFrame();
	}

	glBindTexture(GL_TEXTURE_2D, image[0]);
	drawRectangle(a1, a2, a3, a4, p1, p2, p3, p4); 
	
	glBindTexture(GL_TEXTURE_2D, image[1]);
	drawRectangle(a1, a2, a3, a4, p2, p5, p6, p3); 

	glBindTexture(GL_TEXTURE_2D, image[2]);
	drawRectangle(a1, a2, a3, a4, p5, p7, p8, p6); 
	
	glBindTexture(GL_TEXTURE_2D, image[3]);
	drawRectangle(a1, a2, a3, a4, p7, p1, p4, p8); 

	glBindTexture(GL_TEXTURE_2D, image[4]);
	drawRectangle(a1, a2, a3, a4, p7, p5, p2, p1); 
	
	glBindTexture(GL_TEXTURE_2D, image[5]);
	drawRectangle(a1, a2, a5, a4, p4, p3, p9, p10); 
	drawTriangle(a2, a3, a5, p3, fp11, p9); 
	
	glBindTexture(GL_TEXTURE_2D, image[6]);
	drawTriangle(a5, a3, a2, ap9, fp11, p3); 
	drawTriangle(a1, a2, a5, p3, p6, ap9);	 
	drawTriangle(a2, a3, a5, p6, bp11, ap9); 

	glBindTexture(GL_TEXTURE_2D, image[7]);
	drawRectangle(a1, a2, a3, a5, p6, p8, p10, p9); 
	drawTriangle(a1, a5, a4, p6, p9, bp11); 

	glBindTexture(GL_TEXTURE_2D, image[8]);
	drawTriangle(a1, a2, a5, p8, p4, p9);
	drawTriangle(a1, a5, a4, p8, p9, p10);
	drawTriangle(a2, a3, a5, p4, p10, p9);

	glBindTexture(GL_TEXTURE_2D, image[9]);
	drawRectangle(a1, a6, a5, a4, p10, p9, p14, p12);
	drawRectangle(a6, a2, a3, a5, p9, fp11, fp13, p14);
	drawRectangle(a6, a2, a3, a5, fp11, p9, p14, fp13);
	drawRectangle(a6, a2, a3, a5, ap14, ap9, fp11, fp13);
	drawRectangle(a6, a2, a3, a5, fp11, ap9, ap14, fp13);
	drawRectangle(a6, a2, a3, a5, ap9, bp11, bp13, ap14);
	drawRectangle(a6, a2, a3, a5, bp11, ap9, ap14, bp13);
	drawRectangle(a6, a2, a3, a5, bp11, p9, p14, bp13);
	drawRectangle(a6, a2, a3, a5, p9, bp11, bp13, p14);
	drawRectangle(a1, a6, a5, a4, p9, p10, p12, p14);

	glutSwapBuffers();
}

void init()
{
	glEnable (GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LINE_SMOOTH);
    //glEnable(GL_CULL_FACE);
	glClearColor (.8,.8,.8,1);

	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightf (GL_LIGHT0, GL_SPOT_CUTOFF, 90);        // default 180
	glLightf (GL_LIGHT0, GL_SPOT_EXPONENT, 1);       // default 0
	glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION, .1);  // a
	glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION, .2);   // b
	glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.2); // c
	
	pngInfo info;
	pngSetStandardOrientation(1);

	image[0] = pngBind("0.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);//front side
	image[1] = pngBind("1.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);//right side
	image[2] = pngBind("2.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);//back side
	image[3] = pngBind("3.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);//left side
	image[4] = pngBind("4.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);//bottom
	image[5] = pngBind("5.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);//top front rectangle
	image[6] = pngBind("6.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);//right inner triangle
	image[7] = pngBind("7.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);//top back rectangle
	image[8] = pngBind("8.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);//left inner triangle
	image[9] = pngBind("9.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);//top
}

void timer (int dummy)
{
	if ( flag )
	{
		theta = theta + drate;

		if ( state == 0 )
		{
			fp11[2] += z_shift; bp11[2] -= z_shift;
			fp13[2] += z_shift; bp13[2] -= z_shift;

			fp11[0] -= x_shift;	bp11[0] -= x_shift;
			fp13[0] -= x_shift;	bp13[0] -= x_shift;

			if ( fp11[0] <= p9[0] )
			{
				state = 1;
				z_shift = rate * 0.3;
			}
		}
		else if( state == 1)
		{
			ap9[0] += x_shift; ap14[0] += x_shift;
			
			fp11[2] -= z_shift; bp11[2] += z_shift;
			fp13[2] -= z_shift; bp13[2] += z_shift;

			fp11[0] += z_shift; bp11[0] += z_shift;
			fp13[0] += z_shift; bp13[0] += z_shift;

			if ( ap9[0] >= p3[0] + 1 )
			{
				state = 2;
			}
		}
		else if( state == 2) 
		{
			ap9[0] -= x_shift; ap14[0] -= x_shift;
			
			fp11[2] += z_shift; bp11[2] -= z_shift;
			fp13[2] += z_shift; bp13[2] -= z_shift;

			fp11[0] -= z_shift; bp11[0] -= z_shift;
			fp13[0] -= z_shift; bp13[0] -= z_shift;

			if ( ap9[0] <= p9[0] )
			{
				ap9[0] = ap14[0] = p9[0];
				state = 3;
				z_shift = rate;
				x_shift = rate;
			}
		}
		else if( state == 3 )
		{

			fp11[2] -= z_shift; bp11[2] += z_shift;
			fp13[2] -= z_shift; bp13[2] += z_shift;

			fp11[0] += x_shift;	bp11[0] += x_shift;
			fp13[0] += x_shift;	bp13[0] += x_shift;

			if( fp11[2] <= p9[2] )
			{
				fp11[2] = fp13[2] = p9[2];
				bp11[2] = bp13[2] = p9[2];
				state = 0;
				z_shift = rate;
				x_shift = rate;			
			}
		}
	}

	glutTimerFunc (50, timer, 0);
	ViewerRedisplay(viewer);
}

void keyboard(unsigned char key, int x, int y)
{
	//printf("key = %d", key);
	if( key == 48)
	{
		if( light )
		{
			light = false;
			glDisable (GL_LIGHTING);
			printf("enable the light\n");
		}
		else
		{
			light = true;
			printf("enable the light\n");
		}
	}
	if( key == 127 )//del
	{
		if( frame )
		{
			frame = false;
			printf("hide the frame\n");
		}
		else
		{
			frame = true;
			printf("show the frame\n");
		}
	}
	if( key == 13 )//enter
	{
		printf("restart\n");
		system("CLS");
		restart();
	}
	if( key == 32 )//space
	{
		if( flag )
		{
			flag = false;
			printf("pause\n");
		}
		else
		{
			flag = true;
			printf("play\n");
		}
	}
	if( key == '+')
	{
		drate = drate + 1;

		if( drate >= 10 )
			drate = 10;

		printf("drate = %.2f\n", drate);
	}
	if( key == '-')
	{
		drate = drate - 1;

		if( drate <= 1 )
			drate = 1;

		printf("drate = %.2f\n", drate);
	}
}

void main (int argc, char** argv)
{
	printf("You can press space to pause or play.\n");
	printf("You can press '+' or '-' to change the spotlight rotated speed.\n");
	printf("You can press enter to restart.\n");
	printf("You can press delete to show or hide the frame.\n");
	printf("You can press number 0 to enable or disable the light.\n ");

	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (800,800);
	mainwin = glutCreateWindow ("test");

	glutDisplayFunc(content);
	glutReshapeFunc(reshape);

	viewer = ViewerNew (mainwin, 0,0,800, 800, content);
	ViewerCenter(viewer,0,5,0);
	ViewerExtent(viewer,30);
	
	init();
	glutTimerFunc (50, timer, 0);
	
 	glutKeyboardFunc(keyboard);
	glutMainLoop();
}