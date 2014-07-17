#include "duck.h"
#include <math.h>

#define NUM 14
#define PLAYTIME 40//seconds
#define KILLPOINT 100

int mainwin, viewer;
int mx = 5,my = 5;
int ww, wh;//glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)
int score = 0, remainTime = PLAYTIME;
int start = 5;

int game = 0;//0 : menu, 1 : playing, 2 : game over, 4 : restart
bool switching = false;// game mode switching

float alpha = 0;

char *font = "9x15";//"9x15";//"times roman";

duck *m_duck[NUM];
GLuint aimid, duckid, p2xid, n2xid, p4xid;

extern void generateRandom();
extern int  getRandom(int duckName);

void display()
{
}

void reshape(int w, int h)
{
	ww =w; wh = h;
	glViewport(0,0,w,h);
	ViewerReshape(viewer);
}

void textureSwitch(bool t0, bool t1, bool t2, bool t3, bool t4)
{
	glActiveTexture(GL_TEXTURE0);
	( t0 ) ? (glEnable(GL_TEXTURE_2D)) : (glDisable(GL_TEXTURE_2D));

	glActiveTexture(GL_TEXTURE1);
	( t1 ) ? (glEnable(GL_TEXTURE_2D)) : (glDisable(GL_TEXTURE_2D));

	glActiveTexture(GL_TEXTURE2);
	( t2 ) ? (glEnable(GL_TEXTURE_2D)) : (glDisable(GL_TEXTURE_2D));

	glActiveTexture(GL_TEXTURE3);
	( t3 ) ? (glEnable(GL_TEXTURE_2D)) : (glDisable(GL_TEXTURE_2D));

	glActiveTexture(GL_TEXTURE4);
	( t4 ) ? (glEnable(GL_TEXTURE_2D)) : (glDisable(GL_TEXTURE_2D));
}

void grid()
{
	glPushAttrib (GL_ALL_ATTRIB_BITS);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_POLYGON_OFFSET_FILL);	
	glPolygonOffset(1.3,1.3);
	
	glColor4f(1,0,1,.5);

	glBegin(GL_QUADS);
		glVertex3i(-10,0,10), glVertex3i(10,0,10), glVertex3i(10,0,-10), glVertex3i(-10,0,-10);
	glEnd();

	glBegin(GL_LINES);
		glColor3f(1,1,1);
		for(int i = -10 ; i <= 10 ; i++)
		{
			glVertex3i(i,0,10), glVertex3i(i,0,-10);
			glVertex3i(10,0,i), glVertex3i(-10,0,i);
		}
	glEnd();

	glPopAttrib();
}

void drawTarget()
{
	for(int i = 0 ; i < NUM ; i++)
	{
		glPushName (m_duck[i]->getName());
		m_duck[i]->drawAim();		
		glPopName();
	}
}

void label()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
		if( game == 1 || game == 2 )
		{
			glColor3f(1,1,1);

			glPushAttrib(GL_ALL_ATTRIB_BITS);

			if( remainTime <= PLAYTIME / 2.0 )
				glColor3f(1,0,0);
			
			drawstr(0,wh-20, "Time : %d s", remainTime);

			glPopAttrib();

			drawstr(120,wh-20, "Score : %d", score);
		}
				
	glPopAttrib();
}

void block()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if( game == 2 )
	{
		if( switching )
		{
			glColor3f(0,0,0);

			glBegin(GL_QUADS);
				glVertex2d(0,0);   glVertex2d(ww,0);
				glVertex2d(ww,wh); glVertex2d(0,wh);
			glEnd();

			glColor3f(1,1,1);
			drawstr(ww/2 - 120,wh/2 + 20, "Start in %d second...", start);
		}
		else
		{
			glColor4f(0,0,0, alpha);

			glBegin(GL_QUADS);
				glVertex2d(0,0);   glVertex2d(ww,0);
				glVertex2d(ww,wh); glVertex2d(0,wh);
			glEnd();
			glColor3f(1,1,1);

			drawstr(ww/2 - 120,wh/2 + 20, "Game Over.");
			drawstr(ww/2 - 120,wh/2 - 10, "Your score is %d.", score);

			if( score >= 10000 ) drawstr(ww/2 - 120,wh/2 - 40, "WooooooooW!");
			else if( score >= 8000 ) drawstr(ww/2 - 120,wh/2 - 40, "Well done!");
			else if( score >= 6000 ) drawstr(ww/2 - 120,wh/2 - 40, "Good!");
			else if( score >= 4000 ) drawstr(ww/2 - 120,wh/2 - 40, "Not Bad!");

			drawstr(ww/2 - 120,wh/2 - 70, "Press \"F2\" to restart.");
		}
	}
	else if( game == 0)
	{
		if( switching )
			drawstr(ww/2 - 120,wh/2 + 20, "Start in %d second...", start);
		else
		{
			drawstr(ww/2 - 120,wh/2 + 20, "Shooting Ducks.");
			drawstr(ww/2 - 120,wh/2 - 20, "Press \"F2\" to start the game.");
		}
	}

	glPopAttrib();
}

void aim()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glColor3f(1,0,0);
	glLineWidth (1.8);

	glPushMatrix();
		glTranslatef(mx,wh-my,0);

		glBegin(GL_LINE_LOOP);
			double theta;
			for (theta = 0.0; theta < 2*3.14159; theta += 0.1)
				glVertex2d (15*cos(theta), 15*sin(theta));
		glEnd();
		
		glBegin (GL_LINES);
			glVertex2i (-15,0); glVertex2i (15,0);
			glVertex2i (0,-15); glVertex2i (0,15);
		glEnd();
	glPopMatrix();

	glPopAttrib();
}

void draw2DThings()
{
	BEGIN_2D_OVERLAY (ww, wh);	
	glDisable(GL_LIGHTING);

	label();
	block();
	aim();

	END_2D_OVERLAY();
}

void content()
{
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (0,0,10, 0,0,0, 0,1,0);

	if( game == 1 || game == 2 )
	{
		for(int i = 0 ; i < NUM ; i++)
		m_duck[i]->drawDuck();//render duck
		drawTarget();
	}
	
	//grid();
	draw2DThings();

	glutSwapBuffers();
}

void remain(int nn)
{
	if( game == 2 )
		return;

	remainTime -= 1;
	if(remainTime % 5 == 0)
	{
		generateRandom();
		for(int i = 0 ; i < NUM ; i++)
			m_duck[i]->setBonus(getRandom(i+1));
	}

	if(remainTime <= 0)
	{
		remainTime = 0;
		game = 2;
		cout << "Game Over." << endl;
	}
	glutTimerFunc(1000, remain, 0);
}

void restart()
{
	textureSwitch(false, false, false, false, false);

	int j = -4;

	generateRandom();	

	for(int i = 0 ; i < NUM ; i++)
	{
		if( i % 2 == 0)
			m_duck[i] = new duck(j, 2, 0, i+1);
		else
		{
			m_duck[i] = new duck(j, 0, 0, i+1);
			j += 2;
		}
		m_duck[i]->setBonus(getRandom(i+1));
	}

	game  = 1;
	alpha = 0;
	score = 0; 
	remainTime = PLAYTIME;
	switching = false;
	glutTimerFunc(1000, remain, 0);
}

void startGame(int i)
{
	start -= 1;

	if(start <= 0)
	{
		game = 1;
		restart();
		return;
	}

	glutTimerFunc(1000, startGame, 0);
}

void keyboard(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_F1:
			break;
		case GLUT_KEY_F2:
			if( switching )
				break;
			else if( game == 0 )
			{
				switching = true;
				glutTimerFunc(1000, startGame, 0);
			}
			else if( game == 2 )
			{
				start = 5;
				switching = true;
				glutTimerFunc(1000, startGame, 0);
			}			
			break;
		default:
			break;
	}

	glutPostRedisplay();			
}

void mouseMoveEvent(int x, int y)
{
	mx = x;
	my = y;
	glutPostRedisplay();
}

void pick (int x, int y)
{
	// start picking
	GLint viewport[4];
	GLfloat project[16];
		
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetFloatv(GL_PROJECTION_MATRIX, project);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
		
	/*  create 5x5 pixel picking region near cursor location */
	gluPickMatrix((GLdouble) x, (GLdouble) (viewport[3] - y), 5.0, 5.0, viewport);
	glMultMatrixf (project);
	
	glMatrixMode (GL_MODELVIEW);

	#define BUFSIZE 100
	GLuint selectBuf[BUFSIZE];
	glSelectBuffer(BUFSIZE, selectBuf);
	glRenderMode (GL_SELECT);
	glInitNames();
	drawTarget();

	// restore PROJECTION matrix
	glMatrixMode (GL_PROJECTION); glPopMatrix();

	GLint hits;
		
	hits = glRenderMode(GL_RENDER);
	//printf ("hit: %d\n", hits);
	//cout <<  "["<< mx << ", "<< wh - my << "]" << endl;

	//GLint hits, GLuint buffer[]

	unsigned int i, j;
	GLuint names, *ptr;

	ptr = (GLuint *) selectBuf;
	for (i = 0; i < hits; i++) 
	{
		names = *ptr;
		/*printf(" number of names for hit = %d\n", names);*/ ptr++;
		/*printf("  z1 is %g;", (float) *ptr/0xffffffff);*/ ptr++;
		/*printf(" z2 is %g\n", (float) *ptr/0xffffffff);*/ ptr++;
		/*printf("   the name is ");*/

		for (j = 0; j < names; j++) 
			/*printf("%d \n", *ptr);*/ ptr++;
	}
 //if (hit) name = *(ptr+3);

	if( hits >= 1)
	{
		ptr -= 1;
		int deadName = (int)*ptr;

		if(m_duck[deadName - 1]->isHit() == false )
		{
				m_duck[deadName - 1]->setHit(true);
				score += KILLPOINT * m_duck[deadName - 1]->nowBonus();
		}
	}
}

void mousePressEvent(int button, int state, int x, int y)
{
	if( game != 1 )
		return;
	
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 
	{
		pick (x, y);
	}
}

void timer(int dummy)
{
	if ( game == 2 )
	{
		alpha += 0.02;
	}
	else if( game == 1 )
	{
		for(int i = 0 ; i < NUM ; i++)
			m_duck[i]->step();
	}
	glutTimerFunc(50, timer, 0);
	glutPostRedisplay();
}

void init()
{
	glEnable (GL_DEPTH_TEST);

	glewInit();

	pngSetStandardOrientation(1);

	duckid = pngBind("duck.png", PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_REPEAT, GL_LINEAR, GL_LINEAR); // rgba
	p2xid  = pngBind("2x.png", PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	n2xid  = pngBind("-2x.png", PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	p4xid  = pngBind("4x.png", PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
	aimid  = pngBind("aim.png", PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR); 

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, duckid);

	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, p2xid);

	glActiveTexture(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, n2xid);

	glActiveTexture(GL_TEXTURE3);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, p4xid);

	glActiveTexture(GL_TEXTURE4);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, aimid);

    textureSwitch(false, false, false, false, false);

	glutSetCursor(GLUT_CURSOR_NONE);	

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity(); 
	gluPerspective (100, 1, 0.1, 100 );
	
	setfont(font, 40);//set drawstr font

	game = 0;
}

void main (int argc, char** argv)
{
	glutInit (&argc, argv);

	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (400,400);
	ww = 400;
	wh = 400;
	mainwin = glutCreateWindow ("duck");

	glutDisplayFunc(content);
	
	glutReshapeFunc(reshape);
	glutTimerFunc(50, timer, 0);
	
	viewer = ViewerNew (mainwin, 0,0,400,400, content);
	init();
	ViewerAddClicker (viewer, mousePressEvent);
	glutSpecialFunc(keyboard);
	glutPassiveMotionFunc(mouseMoveEvent);

	glutMainLoop();
}
