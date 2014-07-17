#include "svl/svl.h"
#include <vector>
#include <deque>
#include <iostream>
#include <glut.h>
#include <GL/glpng.h>
using namespace std;

#pragma comment(lib, "gifcapture.lib") 

extern void ProcessCapture(int,int=10);

void special (int key, int x, int y)
{
	if (key == GLUT_KEY_UP) {
		ProcessCapture(glutGetWindow ());
	}
}

typedef struct _particle 
{
	Vec2 pos, vel;
	int birth_time;  // birth_timestamp
	deque<Vec2> trail;
	bool boom;
	float r,g,b;
} PARTICLE;

vector<PARTICLE> particles;
Vec2 force(0,-1);  // gravity

int now;
GLuint background;

float frand(float lo, float hi)
{
	int r = rand()%143;
	return lo + (hi-lo)*r/142.;
}

void display()
{
	glClear (GL_COLOR_BUFFER_BIT);
	
	glBegin(GL_QUADS);
	//glBindTexture (GL_TEXTURE_2D, background);
	glTexCoord2f(0,0); glVertex2i(-10, 0);
	glTexCoord2f(1,0); glVertex2i(10, 0);
	glTexCoord2f(1,1); glVertex2i(10,10);
	glTexCoord2f(0,1); glVertex2i(-10,10);
	glEnd();
	
	glPushAttrib (GL_ALL_ATTRIB_BITS);
	glDisable(GL_TEXTURE_2D);
#define max(x,y) ((x)>(y)?(x):(y))
	for (int i = 0; i < particles.size(); i++) 
	{
		PARTICLE p = particles[i];
		float life = (now - p.birth_time)/1000.;
		float intensity = max(0, (10 - life)/10);

		glColor3f (intensity,intensity,intensity);
		glBegin (GL_POINTS);
		glVertex2dv (p.pos.Ref());	
		glEnd();
		
		// trail display
		glBegin (GL_LINE_STRIP);
		for (int i = 0; i < p.trail.size(); i++) 
		{
			glColor4f (intensity*p.r,	//r
					   intensity*p.g,	//g
					   intensity*p.b,	//b
					   1.0*i/p.trail.size());	//alpha

			glVertex2dv (p.trail[i].Ref());
		}
		glEnd();
	}
	glPopAttrib();
	glutSwapBuffers();
}

PARTICLE init_particle ()
{
	PARTICLE p;
	p.birth_time = glutGet (GLUT_ELAPSED_TIME);
	p.pos = Vec2 (frand(-7,7), 1.5);
	p.vel = Vec2 (frand (-1.3, 1.3), frand(3.8, 5.3));
	p.trail.push_back (p.pos);
	p.boom = false;
	p.r = frand(0,1);
	p.g = frand(0,1);
	p.b = frand(0,1);
	return p;
}

float life (PARTICLE *p) 
{
	return	(now - p->birth_time)/1000.;
}
#define TRAIL_SIZE 15

void boom_particle (PARTICLE p, int index, int fragment)
{
	for(int i = 0 ; i < fragment ; i++)
	{
		PARTICLE boom_fragment;
		boom_fragment.birth_time = glutGet (GLUT_ELAPSED_TIME) - (now - p.birth_time);
		boom_fragment.pos = p.pos;
		boom_fragment.vel = p.vel;
		if(i % 2 == 0)
		{
			if(boom_fragment.vel[0] > 0.6 )
			{
				boom_fragment.vel[0] += frand(-1.5, 0.1);
				boom_fragment.vel[1] += frand(-0.1, 0.8);
			}
			else if(boom_fragment.vel[0] < -0.6)
			{
				boom_fragment.vel[0] += frand(-0.1, 1.5);
				boom_fragment.vel[1] += frand(-0.1, 0.8);
			}
			else
			{
				boom_fragment.vel[0] += frand(-0.5, 0.5);
				boom_fragment.vel[1] += frand(-0.1, 0.8);
			}
		}
		else
		{
			if(boom_fragment.vel[0] > 0.6 )
			{
				boom_fragment.vel[0] += frand(-2, 0.1);
				boom_fragment.vel[1] += frand(-0.2, 1);
			}
			else if(boom_fragment.vel[0] < -0.6)
			{
				boom_fragment.vel[0] += frand(-0.1, 2);
				boom_fragment.vel[1] += frand(-0.2, 1);
			}
			else
			{
				boom_fragment.vel[0] += frand(-0.5, 0.5);
				boom_fragment.vel[1] += frand(-0.2, 1);
			}
		}
		boom_fragment.trail.push_back(boom_fragment.pos);
		boom_fragment.boom = true;
		boom_fragment.r = p.r;	boom_fragment.g = p.g;	boom_fragment.b = p.b;
		
		particles.push_back(boom_fragment);
	}
}

void timer(int dummy)
{
	double dt = 0.1;  // seconds
	now = glutGet (GLUT_ELAPSED_TIME);

	for (int i = 0 ; i < particles.size() ; i++) 
	{
		PARTICLE p = particles[i];
		p.pos += p.vel * dt;
		p.trail.push_back (p.pos);
		if (p.trail.size() > TRAIL_SIZE)
			p.trail.pop_front();
		p.vel += force * dt;

		if(p.vel[1] <= 0 || life(&p) > 5)
		{
			if( !p.boom)
			{
				p.boom = true;
				boom_particle(p, i, frand(5, 15));
			}
		}
		// restore pos/vel back to vector
		particles[i] = p;

		if (life(&p) > 6) 
			particles.erase (particles.begin()+i); // remove from vector
	}

	glutTimerFunc (100, timer, 0);
	glutPostRedisplay();
}

void init()
{
	// pipeline
    glViewport (0,0,400,400);
	glMatrixMode (GL_PROJECTION); glLoadIdentity(); gluOrtho2D (-10,10,-4,16);
	glMatrixMode (GL_MODELVIEW);

	// graphics setting
    glEnable (GL_POINT_SMOOTH);
    glPointSize (4);
	glLineWidth (2);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);

	glEnable(GL_TEXTURE_2D);

	pngSetStandardOrientation(1);
	background = pngBind("chicago.png", PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);
}

void keyboard (unsigned char key, int x, int y)
{
	if (key == ' ') 
	{
		if(particles.size() <= 200)
			particles.push_back (init_particle ());
	}
	glutPostRedisplay();
}

void main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowSize (400,400);
	glutCreateWindow ("firework");
	
	glutDisplayFunc (display);
	glutTimerFunc (100, timer, 0);
	glutKeyboardFunc (keyboard);

	init();
	glutSpecialFunc (special);
	
	glutMainLoop();
}
