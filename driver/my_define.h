#include "GL/gluit.h"
#include "gl/glpng.h"
#include "svl/svl.h"
#include "math.h"
#include <time.h>

#define ABS(a)	(((a) < 0) ? -(a) : (a)) //absolute value
#define treeAmount 20
#define edge 30

typedef struct CarPos_S
{
	Vec2 MIN;
	Vec2 MAX;	
}CarPos;

typedef struct TreePos_S
{
	int x;
	int y;	
}TreePos;

Vec2 findMAX(Vec2 a, Vec2 b, Vec2 c, Vec2 d)
{
	double A = a[0] + a[1];
	double B = b[0] + b[1];
	double C = c[0] + c[1]; 
	double D = d[0] + d[1];
	double max;

	max = ( A > B ) ? (A) : (B);
	max = ( max > C ) ? (max) : (C);
	max = ( max > D ) ? (max) : (D);

	return ( max == A ) ? a : ((max == B) ? b : ((max == C) ? c : d));
}

Vec2 findMIN(Vec2 a, Vec2 b, Vec2 c, Vec2 d)
{
	double A = a[0] + a[1];
	double B = b[0] + b[1];
	double C = c[0] + c[1]; 
	double D = d[0] + d[1];
	double min;

	min = ( A < B ) ? (A) : (B);
	min = ( min < C ) ? (min) : (C);
	min = ( min < D ) ? (min) : (D);

	return ( min == A ) ? a : ((min == B) ? b : ((min == C) ? c : d));
}


int mainwin, viewer;
bool showCircle = false;
GLMmodel *m1;
GLuint treetexture;

float current_origin_x, current_origin_y;
float angle;
float _distance;  // somehow, "distance" collide with a system symbol

float speed = 0.0;
float maxspeed = 6.0;
float max_reverse = -4.0;

// AABB for the arrow
Vec2 min(0,-.5), max(2,.5), cam(-2,0), lookAt(0, 0);
Vec2 points[4];
Vec2 m_cam, m_lookAt, mid;

CarPos now_cp;
TreePos tree[treeAmount];