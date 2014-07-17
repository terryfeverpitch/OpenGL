/*#ifndef DUCK_H
#define DUCK_H*/
#include <GL/glew.h>
#include <GL/gluit.h>
#include <GL/glpng.h>
#include <svl/svl.h>

#pragma comment (lib, "glew32.lib")

class duck
{
public:
	duck(int n);
	duck(float x, float y, float z, int n, float s = 0.1);

	void intro();

	void setName(int name);
	int  getName();
	void setHit(bool h);
	bool isHit();
	bool isFlip();

	void step();
	//void render();
	void drawDuck();//render the duck
	void drawAim();//render the aim
	void setBonus(int b);
	int  nowBonus();

private:	
	void dead();
	void respawn();
	void textureSwitch(bool t0, bool t1, bool t2, bool t3, bool t4);	
	void useBonus();

	Vec3  pos;
	int	  edge;
	
	float speed;

	int   name;
	int	  flip;
	int   angle;
	int   rate;
	int   respawnTime;
	int   bonus; 	

	bool  hit;
	bool  lied;
};
