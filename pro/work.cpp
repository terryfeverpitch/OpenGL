#include "GL/glpng.h"
#include "gluit.h"

GLuint graph[7];
float zside,xside;
bool open = false;
bool close = false;

int mainwin, viewer;
void display(){}
void reshape(int w, int h)
{
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
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
	float a12[3]={-3.5,9.5,-0.05};
	float a13[3]={3.5,9.5,-0.05-zside};
	float a14[3]={-3.5,11,-0.05};
	float a15[3]={3.5,11,-0.05-zside};
	float a16[3]={0,9.5,0.1};
	float a17[3]={0,9.5,0.0};
	float a18[3]={0,11,0.0};
	float a19[3]={0,11,0.0};
	float a20[3]={0,9.5,0.00};
	float a21[3]={0+xside,9.5,0.00};
	float a22[3]={0,11,0.00};
	float a23[3]={0+xside,11,0.00};

	
	//first

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

/*	glBindTexture(GL_TEXTURE_2D, graph[6]);
	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	glTexCoord2f(1,0);glVertex3fv(a23);
	glTexCoord2f(1,1);glVertex3fv(a15);
	glTexCoord2f(0,1);glVertex3fv(a13);
	glTexCoord2f(0,0);glVertex3fv(a21);
	glEnd();
*/
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

	glBindTexture(GL_TEXTURE_2D, graph[2]); 
	glBegin(GL_POLYGON);
	glNormal3f(1,0,0);
	glTexCoord2f(1,0);glVertex3fv(a1);
	glTexCoord2f(1,1);glVertex3fv(a2);
	glTexCoord2f(0.35,1);glVertex3fv(a6);
	glTexCoord2f(0,0.49);glVertex3fv(a13);
	glTexCoord2f(0,0.51);glVertex3fv(a9);
	glTexCoord2f(0.35,0);glVertex3fv(a5);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, graph[5]);
	glBegin(GL_POLYGON);
	glNormal3f(-1,0,0);
	glTexCoord2f(1,0);glVertex3fv(a3);
	glTexCoord2f(1,1);glVertex3fv(a0);
	glTexCoord2f(0.3,1);glVertex3fv(a4);
	glTexCoord2f(0,0.51);glVertex3fv(a8);
	glTexCoord2f(0,0.49);glVertex3fv(a12);
	glTexCoord2f(0.3,0);glVertex3fv(a7);
	glEnd();
	glutPostRedisplay();
}


void content()
{
	glClear (GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	//grid
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glBegin(GL_LINES);
	for(int a=-10; a<=10; a++){
		glVertex3f(a,0,10);glVertex3f(a,0,-10);
		glVertex3f(10,0,a);glVertex3f(-10,0,a);
	}
	glEnd();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	milk();

	glutSwapBuffers();
}

void mouse(int button, int state, int x, int y){

	printf("%d",state);
}
void init()
{
	glClearColor(.4, .4, .4, 1);
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);

	pngInfo info;
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, graph[6]);
	glEnable(GL_CULL_FACE);
	pngSetStandardOrientation(1);
	
	graph[0] = pngBind("6.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
	graph[1] = pngBind("2.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
	graph[2] = pngBind("3.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
	graph[3] = pngBind("4.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
	graph[4] = pngBind("7.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
	graph[5] = pngBind("1.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);
	graph[6] = pngBind("8.png", PNG_NOMIPMAP, PNG_SOLID, &info, GL_CLAMP, GL_NEAREST, GL_NEAREST);

}

void keyboard(unsigned char key, int x, int y){
	printf("%d",key);
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

void time(int a){
	
	static int sign = 1;

	if(open == true && zside < 1.5)
		zside += 0.1;
	if(open == true && xside < 6)
		xside += 0.2;
	if(close == true && zside > 0.15)
		zside -= 0.1;
	if(close == true && xside > 0.15)
		xside -= 0.2;

	glutTimerFunc(10,time,0);
}

void main (int argc, char** argv)
{

	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (400,400);
	mainwin = glutCreateWindow ("tst");
    glutKeyboardFunc(keyboard);
	glutTimerFunc(10,time,0);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	
	viewer = ViewerNew (mainwin, 0,0,400,400, content);
	init();
	
	glutMainLoop();
}