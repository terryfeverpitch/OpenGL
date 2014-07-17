#include "duck.h"

duck::duck(int n)
{	
	pos   = Vec3(0,0,0);
	edge  = pos[0] + 6;
	speed = 0.1;

	name  = n;
	flip  = 1;
	angle = 0;
	rate  = 0;
	respawnTime = 0;
	bonus = 1;

	hit  = false;
	lied = false;
}

duck::duck(float x, float y, float z, int n, float s)
{
	pos   = Vec3(x,y,z);
	edge  = pos[0] + 6;
	speed = s;

	flip  = (speed > 0) ? (1) : (-1);

	name  = n;
	angle = 0;
	rate  = 0;
	respawnTime = 0;

	bonus = 1;

	hit  = false;
	lied = false;
}

void duck::intro()
{
	cout << "Duck " << name << endl;
	cout << "Pos	= " << pos << endl;
	cout << "Speed	= " << speed << endl;
	cout << "Bonus  = " << bonus << endl;
	cout << "Press NUMBER " << name << " to kill." << endl << endl;
	//cout << "defalut angle	" << angle << endl;
	//cout << "defalut rate	" << rate << endl << endl;
}

void duck::setName(int n)
{
	name = n;
}

int duck::getName()
{
	return name;
}

void duck::setHit(bool h)
{
	hit = h;

	if( hit )
		dead();
}

bool duck::isHit()
{
	return hit;
}

void duck::step()
{
	pos[0] += speed;

	if( pos[0] >= edge || pos[0] <= edge - 12 ) 
	{
		speed *= -1;
		flip  *= -1;
	}
	
	if( hit )
	{
		if( lied )
		{	
			respawnTime -= 1;
			if( respawnTime <= 0 )
				respawn();
		}

		angle -= rate;

		if( angle <= -90 && !lied )
		{
			angle       = -90;
			rate        = 0;
			respawnTime = 20;
			lied = true;
		}

		if( angle >= 0 )
		{
			angle = 0;
			rate = 0;
			hit = false;
		}
	}
}

/*void duck::render()
{
	glPushMatrix();
	drawDuck();
	textureSwitch(false, false, false, false, false);
	drawAim();
	glPopMatrix();
}*/

void duck::drawDuck()
{
	glPushAttrib (GL_ALL_ATTRIB_BITS);

	glEnable(GL_ALPHA_TEST);
	
	glAlphaFunc(GL_GREATER, 0.0);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-2,-2);

	glPushMatrix();
		glTranslatef(pos[0],pos[1],pos[2]);
		glRotatef(angle, 1,0,0);
		useBonus();
	glPopMatrix();

	glPopAttrib();
}

void duck::drawAim ()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	textureSwitch(false, false, false, false, false);

	glActiveTexture (GL_TEXTURE4); glEnable(GL_TEXTURE_2D);

	glEnable (GL_ALPHA_TEST);
	glAlphaFunc (GL_GREATER, 0.0);

	glEnable (GL_POLYGON_OFFSET_FILL);
	glPolygonOffset (-3,-3);

	glPushMatrix();	
	glTranslatef(pos[0],pos[1],pos[2]);
	glRotatef(angle, 1,0,0);
		glBegin (GL_QUADS);
			glMultiTexCoord2f(GL_TEXTURE4, 0,0); glVertex2f (-.3,0);
			glMultiTexCoord2f(GL_TEXTURE4, 1,0); glVertex2f (+.3,0);
			glMultiTexCoord2f(GL_TEXTURE4, 1,1); glVertex2f (+.3, .6);
			glMultiTexCoord2f(GL_TEXTURE4, 0,1); glVertex2f (-.3, .6);
		glEnd();
	glPopMatrix();

	glPopAttrib();
}

void duck::setBonus(int b)
{
	bonus = b;
}

int duck::nowBonus()
{
	return bonus;
}

void duck::dead()
{
	rate = 10;
}

void duck::respawn()
{
    rate = -10;
	lied = false;
	hit = true;
}

void duck::textureSwitch(bool t0, bool t1, bool t2, bool t3, bool t4)
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

void duck::useBonus()
{
	switch(bonus)
	{
		case 1:		// no bonus
			textureSwitch(true, false, false, false, false);
			glActiveTexture(GL_TEXTURE0);
			glBegin(GL_QUADS);
				glTexCoord2f(0,0);     glVertex2i(-1,0);
				glTexCoord2f(flip,0);  glVertex2i(1,0);
				glTexCoord2f(flip,1);  glVertex2i(1,2);
				glTexCoord2f(0,1);     glVertex2i(-1,2);
			glEnd();
			break;
		case 2:		// 2x
			textureSwitch(true, true, false, false, false);
			glBegin(GL_QUADS);
				glMultiTexCoord2f(GL_TEXTURE0, 0,0);    glMultiTexCoord2f(GL_TEXTURE1, 0,0); glVertex2i(-1,0);
				glMultiTexCoord2f(GL_TEXTURE0, flip,0); glMultiTexCoord2f(GL_TEXTURE1, 1,0); glVertex2i(1,0);
				glMultiTexCoord2f(GL_TEXTURE0, flip,1); glMultiTexCoord2f(GL_TEXTURE1, 1,1); glVertex2i(1,2);
				glMultiTexCoord2f(GL_TEXTURE0, 0,1);    glMultiTexCoord2f(GL_TEXTURE1, 0,1); glVertex2i(-1,2);
			glEnd();
			break;
		case -2:	// -2x
			textureSwitch(true, false, true, false, false);
			glBegin(GL_QUADS);
				glMultiTexCoord2f(GL_TEXTURE0, 0,0);    glMultiTexCoord2f(GL_TEXTURE2, 0,0); glVertex2i(-1,0);
				glMultiTexCoord2f(GL_TEXTURE0, flip,0); glMultiTexCoord2f(GL_TEXTURE2, 1,0); glVertex2i(1,0);
				glMultiTexCoord2f(GL_TEXTURE0, flip,1); glMultiTexCoord2f(GL_TEXTURE2, 1,1); glVertex2i(1,2);
				glMultiTexCoord2f(GL_TEXTURE0, 0,1);    glMultiTexCoord2f(GL_TEXTURE2, 0,1); glVertex2i(-1,2);
			glEnd();
			break;
		case 4:		// 4x
			textureSwitch(true, false, false, true, false);
			glBegin(GL_QUADS);
				glMultiTexCoord2f(GL_TEXTURE0, 0,0);    glMultiTexCoord2f(GL_TEXTURE3, 0,0); glVertex2i(-1,0);
				glMultiTexCoord2f(GL_TEXTURE0, flip,0); glMultiTexCoord2f(GL_TEXTURE3, 1,0); glVertex2i(1,0);
				glMultiTexCoord2f(GL_TEXTURE0, flip,1); glMultiTexCoord2f(GL_TEXTURE3, 1,1); glVertex2i(1,2);
				glMultiTexCoord2f(GL_TEXTURE0, 0,1);    glMultiTexCoord2f(GL_TEXTURE3, 0,1); glVertex2i(-1,2);
			glEnd();
			break;
		default:	// force other situations to be bonus = 1.
			bonus = 1;
			textureSwitch(true, false, false, false, false);
			glActiveTexture(GL_TEXTURE0);
			glBegin(GL_QUADS);
				glTexCoord2f(0,0);     glVertex2i(-1,0);
				glTexCoord2f(flip,0);  glVertex2i(1,0);
				glTexCoord2f(flip,1);  glVertex2i(1,2);
				glTexCoord2f(0,1);     glVertex2i(-1,2);
			glEnd();
			break;
	}	
}



