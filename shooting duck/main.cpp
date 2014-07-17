#include "main.h"

void reshape(int w, int h)
{
	ww = w;	//save window size
	wh = h;
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

void generateRandom()
{									
	double random;
	int i, index;

	cprob[0] = prob[0];
	for(i = 1 ; i < BOUNSKINDS ; i++)
		cprob[i] = cprob[i - 1] + prob[i];

	for(i = 0 ; i < NUM +1 ; i++) 
	{
        random = rand() / (RAND_MAX + 1.0);
        
        for(index = 0 ; index < BOUNSKINDS ; index++)
		{
            if(random <= cprob[index]) 
				break;
		}
		save[i] = bonusKinds[index];
    }
}

void keyboardEvent(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_F2:
			if( game == 3 )
			{
				game = 1;
				start = TRANSTIME;
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

void mousePressEvent(int button, int state, int x, int y)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && game == 0)
	{
		if(mx >= ww/2 - 130 && mx <= ww/2 - 70)
		{
			if( my >= wh/2 && my <= wh/2 + 20)
			{
				game = 1;
				start = TRANSTIME;
				glutTimerFunc(1000, startGame, 0);
			}
		}
		if(mx >= ww/2 - 40 && mx <= ww/2 + 20)
		{
			if( my >= wh/2 && my <= wh/2 + 20)
				help = (help) ? (false):(true);
		}
	}
	
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && game == 2) 
		pick (x, y);
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

	#define BUFSIZE 10
	GLuint selectBuf[BUFSIZE];
	glSelectBuffer(BUFSIZE, selectBuf);
	glRenderMode (GL_SELECT);
	glInitNames();
	drawTarget();

	// restore PROJECTION matrix
	glMatrixMode (GL_PROJECTION); glPopMatrix();

	GLint hits;	
	hits = glRenderMode(GL_RENDER);

	GLuint *ptr;
	ptr = (GLuint *) selectBuf;

	if( hits >= 1)//processHits
	{
		ptr += 3;
		int deadName = (int)*ptr;

		if(m_duck[deadName - 1]->isHit() == false )
		{
			m_duck[deadName - 1]->setHit(true);
			score += KILLPOINT * m_duck[deadName - 1]->nowBonus();
		}
	}
}

void content()
{
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (0,0,8, 0,0,0, 0,1,0);

	if( help )
		helper();
	else
	{
		switch( game )
		{
			case 0: // menu
				menu();
				break;
			case 1: // transform
				transform();
				break;
			case 2: // playing
				playing();
				label();
				break;
			case 3: // over
				playing();	
				label();
				over();
				break;
			default:
				break;
		}
	}
	
	aim();

	glutSwapBuffers();
}

void helper()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);		
	BEGIN_2D_OVERLAY (ww, wh);

	glDisable(GL_LIGHTING);	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1,1,1, .5);

	glBegin(GL_QUADS);
		glVertex2d(ww/2 - 40, wh/2 - 20);   glVertex2d(ww/2 + 20,wh/2 - 20);
		glVertex2d(ww/2 + 20, wh/2);   glVertex2d(ww/2 - 40, wh/2);
	glEnd();
	
	glColor3f(1,1,1);
	drawstr(ww/2 - 180, wh/2 + 140, "1. Click your right button to kill ducks.");
	drawstr(ww/2 - 180, wh/2 + 110, "2. You have 40 seconds.");
	drawstr(ww/2 - 180, wh/2 + 80, "3. Watch out for the bonus on the ducks.");
	drawstr(ww/2 - 180, wh/2 + 50, "4. Enjoy this.");

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	if(mx >= ww/2 - 40 && mx <= ww/2 + 20)
		if( my >= wh/2 && my <= wh/2 + 20)
			glColor4f(1,0,0, .8);

	drawstr(ww/2 - 30, wh/2 - 15, "BACK");
	glPopAttrib();

	END_2D_OVERLAY();
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

void menu()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);		
	BEGIN_2D_OVERLAY (ww, wh);

	glDisable(GL_LIGHTING);	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1,1,1, .5);

	glBegin(GL_QUADS);
		glVertex2d(ww/2 - 130, wh/2 - 20);   glVertex2d(ww/2 - 70 ,wh/2 - 20);
		glVertex2d(ww/2 - 70, wh/2);   glVertex2d(ww/2 - 130, wh/2);

		glVertex2d(ww/2 - 40, wh/2 - 20);   glVertex2d(ww/2 + 20,wh/2 - 20);
		glVertex2d(ww/2 + 20, wh/2);   glVertex2d(ww/2 - 40, wh/2);
	glEnd();
	
	glColor3f(1,1,1);
	drawstr(ww/2 - 120,wh/2 + 20, "Shooting Ducks.");

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	if(mx >= ww/2 - 130 && mx <= ww/2 - 70)
		if( my >= wh/2 && my <= wh/2 + 20)
			glColor4f(1,0,0, .8);

	drawstr(ww/2 - 123,wh/2 - 15, "START");
	glPopAttrib();

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	if(mx >= ww/2 - 40 && mx <= ww/2 + 20)
		if( my >= wh/2 && my <= wh/2 + 20)
			glColor4f(1,0,0, .8);

	drawstr(ww/2 - 30, wh/2 - 15, "HELP");
	glPopAttrib();

	END_2D_OVERLAY();
	glPopAttrib();
}

void transform()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	BEGIN_2D_OVERLAY (ww, wh);

	glDisable(GL_LIGHTING);

	drawstr(ww/2 - 120,wh/2 + 20, "Start in %d second...", start);

	END_2D_OVERLAY();	
	glPopAttrib();
}

void playing()
{
	for(int i = 0 ; i < NUM ; i++)
		m_duck[i]->drawDuck(); //render duck
	
	drawTarget();
}

void over()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	BEGIN_2D_OVERLAY (ww, wh);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glColor4f(0,0,0, alpha);

	glBegin(GL_QUADS);
		glVertex2d(0,0);   glVertex2d(ww,0);
		glVertex2d(ww,wh); glVertex2d(0,wh);
	glEnd();

	glColor3f(1,1,1);

	drawstr(ww/2 - 120,wh/2 + 20, "Game Over.");
	drawstr(ww/2 - 120,wh/2 - 10, "Your score is %d.", score);

	if( score >= 10000 )	 drawstr(ww/2 - 120,wh/2 - 40, "WooooooooW!");
	else if( score >= 8000 ) drawstr(ww/2 - 120,wh/2 - 40, "Well done!");
	else if( score >= 6000 ) drawstr(ww/2 - 120,wh/2 - 40, "Good!");
	else if( score >= 4000 ) drawstr(ww/2 - 120,wh/2 - 40, "Not Bad!");

	drawstr(ww/2 - 120,wh/2 - 70, "Press \"F2\" to restart.");
	
	END_2D_OVERLAY();
	glPopAttrib();
}

void label()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	BEGIN_2D_OVERLAY (ww, wh);
	
	glColor3f(1,1,1);

	if( remainTime <= PLAYTIME / 2.0 )
		glColor3f(1,0,0);
			
	drawstr(0,wh-20, "Time : %d s", remainTime);
	
	glColor3f(1,1,1);
	drawstr(120,wh-20, "Score : %d", score);

	END_2D_OVERLAY();
	glPopAttrib();
}

void aim()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	BEGIN_2D_OVERLAY (ww, wh);
	
	glColor3f(1,0,0);
	glLineWidth (1.8);

	glPushMatrix();
		glTranslatef(mx,wh-my,0);

		glBegin(GL_LINE_LOOP);
			double theta;
			for (theta = 0.0 ; theta < 2 * 3.14159 ; theta += 0.1)
				glVertex2d (15*cos(theta), 15*sin(theta));
		glEnd();
		
		glBegin (GL_LINES);
			glVertex2i (-15,0); glVertex2i (15,0);
			glVertex2i (0,-15); glVertex2i (0,15);
		glEnd();
	glPopMatrix();

	END_2D_OVERLAY();
	glPopAttrib();
}

void init()
{
	glEnable (GL_DEPTH_TEST);

	glewInit();

	pngSetStandardOrientation(1);

	duckid = pngBind("duck.png", PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_REPEAT, GL_LINEAR, GL_LINEAR);
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
	srand(time(NULL));
}

void create()
{
	textureSwitch(false, false, false, false, false);

	int j = -6;

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
		m_duck[i]->setBonus( save[i+1] );
	}

	game       = 2;
	alpha      = 0;
	score      = 0; 
	remainTime = PLAYTIME;
	glutTimerFunc(1000, remain, 0);
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
	glutTimerFunc(64, timer, 0);
	
	viewer = ViewerNew (mainwin, 0,0,400,400, content);

	init();

	glutSpecialFunc(keyboardEvent);
	ViewerAddClicker (viewer, mousePressEvent);
	glutPassiveMotionFunc(mouseMoveEvent);

	glutMainLoop();
}

void timer(int dummy)
{
	if ( game == 3 )
	{
		alpha += 0.015;
	}
	else if( game == 2 )
	{
		for(int i = 0 ; i < NUM ; i++)
			m_duck[i]->step();
	}
	glutTimerFunc(64, timer, 0);
	glutPostRedisplay();
}

void remain(int dummy)
{
	remainTime -= 1;

	if(remainTime % 5 == 0)
	{
		generateRandom();
		for(int i = 0 ; i < NUM ; i++)
			m_duck[i]->setBonus( save[i+1] );
	}

	if( remainTime <= 0 )
	{
		remainTime = 0;
		game = 3;
		cout << "Game Over." << endl;
		return;
	}
	glutTimerFunc(1000, remain, 0);
}

void startGame(int dummy)
{
	start -= 1;

	if(start <= 0)
	{
		game = 2;
		create();
		return;
	}

	glutTimerFunc(1000, startGame, 0);
}