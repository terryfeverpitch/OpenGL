#include "duck.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define NUM        14
#define PLAYTIME   40    //in seconds
#define KILLPOINT  100
#define BOUNSKINDS 4
#define TRANSTIME  3 

int mainwin, viewer;
int ww, wh;

int    bonusKinds[BOUNSKINDS] = {1,   2,    -2,   4};		
double prob[BOUNSKINDS]		  = {0.5, 0.25, 0.15, 0.1};	
double cprob[BOUNSKINDS];
int save[NUM +1]; //random numbers saver

int mx			= 5;    //mouse position
int my			= 5;
int start		= TRANSTIME;	//transform counting
int game        = 0;	//0 : menu, 1 : playing, 2 : game over, 4 : restart
int score		= 0;
int remainTime  = PLAYTIME;

bool help = false;

float alpha = 0;

char *font = "9x15";

duck *m_duck[NUM];

GLuint aimid, duckid, p2xid, n2xid, p4xid;

void reshape(int w, int h);

void textureSwitch(bool t0, bool t1, bool t2, bool t3, bool t4);
void generateRandom();
//input
void keyboardEvent(int key, int x, int y);
void mouseMoveEvent(int x, int y);
void mousePressEvent(int button, int state, int x, int y);
void pick (int x, int y);

void init();
void create();
//render
void content();
void helper();
void drawTarget();
void menu();      //game = 0;		
void transform(); //game = 1;
void playing();   //game = 2;
void over();      //game = 3;
void label();
void aim();
//timer
void timer(int dummy);     //render timer
void remain(int dummy);    //remain timer
void startGame(int dummy); //transform timer