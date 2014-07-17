#include "my_define.h"

void display(void){}
void reshape (int w, int h)
{
	glViewport(0,0,w,h);
	//ViewerReshape (viewer);
}

void grid_xy (void)
{
	glPushAttrib (GL_ALL_ATTRIB_BITS);
	glDisable (GL_LIGHTING);
	
	// grid line
	glColor3f (.4,.4,.4);
	glBegin (GL_LINES);
	for (int i = -1 * edge; i <= edge; i++) 
	{
		glVertex2i (i,-1 * edge);	glVertex2i (i, edge);
		glVertex2i (-1 * edge,i);	glVertex2i (edge,i);
	}
	glEnd();

	// axes
	glLineWidth (5.0);
	glBegin (GL_LINES);
	glColor3f (1,0,0), glVertex3i (0,0,0); glVertex3i (1,0,0);// red	x
	glColor3f (0,1,0), glVertex3i (0,0,0); glVertex3i (0,1,0);// green	y
	glColor3f (0,0,1), glVertex3i (0,0,0); glVertex3i (0,0,1);// blue	z
	glEnd();

	glPopAttrib();
}

void DrawAABB ()
{
	// four points on the AABB
	points[0] = Vec2 (min[0],min[1]);
	points[1] = Vec2 (max[0],min[1]);
	points[2] = Vec2 (max[0],max[1]);
	points[3] = Vec2 (min[0],max[1]);
	//camera position
	m_cam = cam;
	m_lookAt = lookAt;
	//the middle of the car
	mid = (max + min) / 2;

	// transform
	for (int i = 0; i < 4; i++) 
		points[i] = proj(HTrans3 (Vec2 (current_origin_x, current_origin_y)) * HRot3 (angle*vl_pi/180) * HTrans3 (Vec2(_distance,0)) * Vec3(points[i],1.0)); 
	
	m_cam = proj(HTrans3 (Vec2 (current_origin_x, current_origin_y)) * HRot3 (angle*vl_pi/180) * HTrans3 (Vec2(_distance,0)) * Vec3(m_cam,1.0));
	m_lookAt = proj(HTrans3 (Vec2 (current_origin_x, current_origin_y)) * HRot3 (angle*vl_pi/180) * HTrans3 (Vec2(_distance,0)) * Vec3(m_lookAt,1.0));
	mid = proj(HTrans3 (Vec2 (current_origin_x, current_origin_y)) * HRot3 (angle*vl_pi/180) * HTrans3 (Vec2(_distance,0)) * Vec3(mid,1.0));
	
	now_cp.MAX = findMAX(points[0], points[1], points[2], points[3]);
	now_cp.MIN = findMIN(points[0], points[1], points[2], points[3]);

	// display
	/*glPushAttrib (GL_ALL_ATTRIB_BITS);
	glDisable (GL_LIGHTING);
	glColor3f (1,0,1);
	glBegin (GL_LINE_LOOP);
	for (i = 0; i < 4; i++) 
		glVertex2dv (points[i].Ref());*/

	glEnd();
	glPopAttrib();
}

void DrawVehicle ()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable (GL_CULL_FACE);
	glTranslatef (0.5,0,0);
	glRotatef (90, 1,0,0);
	glRotatef (90, 0,1,0);	
	glmDraw (m1, GLM_SMOOTH|GLM_MATERIAL);
	glPopAttrib();
}

void drawXtree (const Vec3& center, const Vec3& size)
{
	glPushAttrib (GL_ALL_ATTRIB_BITS);

		glEnable (GL_TEXTURE_2D);
		glBindTexture (GL_TEXTURE_2D, treetexture);
		glEnable (GL_ALPHA_TEST);
		glDisable (GL_LIGHTING);
		glDisable (GL_CULL_FACE);
		glAlphaFunc (GL_GREATER, 0);

		glPushMatrix();
			glTranslated (center[0], center[1], center[2]);
			glRotatef (90, 1,0,0);
			glScaled (size[0], size[1], size[2]);
			
			glBegin (GL_QUADS);
			glTexCoord2f (0,0), glVertex2i (-1, 0);
			glTexCoord2f (1,0), glVertex2i (1, 0);
			glTexCoord2f (1,1), glVertex2i (1, 2);
			glTexCoord2f (0,1), glVertex2i (-1, 2);
			glEnd();
 
			glRotatef (90., 0.,1.,0.);
			glBegin (GL_QUADS);
			glTexCoord2f (0,0), glVertex2i (-1, 0);
			glTexCoord2f (1,0), glVertex2i (1, 0);
			glTexCoord2f (1,1), glVertex2i (1, 2);
			glTexCoord2f (0,1), glVertex2i (-1, 2);
			glEnd();
		glPopMatrix();
	glPopAttrib ();
}

bool Check_Intersect(CarPos *R, TreePos *C, double Rad)
{
	double Rad2; 
	Rad2 = Rad * Rad;

	//Translate coordinates, placing C at the origin. 
	R->MAX[0] -= C->x;  R->MAX[1] -= C->y;
	R->MIN[0] -= C->x;  R->MIN[1] -= C->y;
	
	if (R->MAX[0] < 0)	// R to left of circle center 
	{
		if (R->MAX[1] < 0) // R in lower left corner 
			return ((R->MAX[0] * R->MAX[0] + R->MAX[1] * R->MAX[1]) < Rad2);
	
		else if (R->MIN[1] > 0) // R in upper left corner 
			return ((R->MAX[0] * R->MAX[0] + R->MIN[1] * R->MIN[1]) < Rad2);
   		else 					// R due West of circle 
			return(ABS(R->MAX[0]) < Rad);
	}
 	else if (R->MIN[0] > 0)	// R to right of circle center
	{
		if (R->MAX[1] < 0)	// R in lower right corner 
			return ((R->MIN[0] * R->MIN[0] + R->MAX[1] * R->MAX[1]) < Rad2);
   		else if (R->MIN[1] > 0)  	// R in upper right corner 
     		return ((R->MIN[0] * R->MIN[0] + R->MIN[1] * R->MIN[1]) < Rad2);
   		else 				// R due East of circle 
			return (R->MIN[0] < Rad);
	}
	//R on circle vertical centerline, and 
 	else if (R->MAX[1] < 0) 	// R due South of circle  	
		return (ABS(R->MAX[1]) < Rad);
	else if (R->MIN[1] > 0)  	// R due North of circle 
     	return (R->MIN[1] < Rad);
   	else // R contains circle centerpoint 
     	return(true);
} 	

void circle(const Vec3& center, const double r)
{
	double angle, x, y, z;
	glPushAttrib (GL_ALL_ATTRIB_BITS);
		glDisable (GL_LIGHTING);
		glColor3f (1,0,0);
		glBegin (GL_POLYGON);
		glPolygonMode (GL_FRONT, GL_LINE);
		for( angle = 0 ; angle < 2 * 3.14 ; angle += 0.1)
		{
			x = center[0] + r * sin(angle);
			y = center[1] + r * cos(angle);
			z = 0.0f;
			glVertex3f(x,y,z);
		}
		glEnd();
	glPopAttrib ();
}

int whichIsNearest()
{
	int i = treeAmount;
	double temp[treeAmount];
	double x;
	double y;
	double dis;
	int index;
	for( i = 0 ; i < treeAmount ; i++)
	{
		x = (tree[i].x - mid[0]);
		y = (tree[i].y - mid[1]);
		x = ABS(x); y = ABS(y);

		temp[i] = x*x + y*y;
	}

	dis = temp[0];
	index = 0;

	for( i = 0 ; i < treeAmount ; i++)
	{
		if ( dis > temp[i] )
		{
			dis = temp[i];
			index = i;
		}
	}
	return index;
}

void content ()
{
	glClear (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (m_cam[0],m_cam[1],1, m_lookAt[0], m_lookAt[1], 1, 0,0, 1);

	grid_xy();

	glPushMatrix();
	for( int i = 0 ; i < treeAmount ; i++ )
	{
		drawXtree(Vec3(tree[i].x, tree[i].y, 0), Vec3(1,1,1));
		glPushMatrix();
		if(showCircle)
			circle(Vec3(tree[i].x, tree[i].y, 0), 1);
		glPopMatrix();
	}
	glPopMatrix();
	
	if ( mid[0] >= edge || mid[1] >= edge || mid[0] <= -1 * edge || mid[1] <= -1 * edge)
		speed = 0;


	if ( Check_Intersect(&now_cp, &tree[whichIsNearest()], 1) )
		speed = 0;

	// draw AABB in world coordinates
	DrawAABB();
	
	glPushMatrix();
	glTranslatef (current_origin_x, current_origin_y, 0);
	glRotatef (angle, 0,0,1);
	glTranslatef (_distance + 0.5, 0,0);
	DrawVehicle();
	glPopMatrix();
	
	glPushAttrib (GL_ALL_ATTRIB_BITS);
	glDisable (GL_LIGHTING);
	glColor3ub (255,255,255);
	BEGIN_2D_OVERLAY(15,15);
	drawstr (1,15, "Time : ");
	drawstr (4,15, "Score : ");
	//drawstr (1,1, "left/right to turn");
	END_2D_OVERLAY();
	glPopAttrib();
	
	glutSwapBuffers();
}

void idle(void)
{
	static int last;
	int now;
	float dt;

	if (last == 0) 
	{
		last = glutGet (GLUT_ELAPSED_TIME);
		return;
	}
	now = glutGet (GLUT_ELAPSED_TIME);
	dt = (now - last)/1000.;  // in "seconds"
	last = now;

	_distance += speed*dt;
	glutPostRedisplay();
	//ViewerRedisplay(viewer);
}

void restart()
{
	current_origin_x = current_origin_y = 0;
	angle = 0;
	_distance = 0;
	speed = 0;

	now_cp.MAX = Vec2 (max[0],max[1]);
	now_cp.MIN = Vec2 (min[0],min[1]);

	int x, y;
	for( int i = 0 ; i < treeAmount ; i++ )
	{
		x = rand() % (edge + 11) - (edge - 10);
		y = rand() % (edge + 11) - (edge - 10);
		
		if( x*x + y*y < 2 )
		{
			i--;
			continue;
		}

		tree[i].x = x;
		tree[i].y = y;
		printf("tree[%d] = [%d,%d]\n", i, tree[i].x, tree[i].y);
	}

}

void init()
{
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);
	glEnable (GL_LIGHT0);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity(); 
	gluPerspective (100, 1, 0.1, 100 );

	// reading OBJ model
	m1 = glmReadOBJ ("toycar.obj");
	glmUnitize (m1);
	glmFacetNormals (m1);
	glmVertexNormals (m1, 90);

	pngSetStandardOrientation(1); 
	treetexture = pngBind("shrub1.png", PNG_NOMIPMAP, PNG_ALPHA, NULL, GL_CLAMP, GL_LINEAR, GL_LINEAR);

	now_cp.MAX = Vec2 (max[0],max[1]);
	now_cp.MIN = Vec2 (min[0],min[1]);

	int x, y;
	for( int i = 0 ; i < treeAmount ; i++ )
	{
		x = rand() % (edge + 11) - (edge - 10);
		y = rand() % (edge + 11) - (edge - 10);
		
		if( x*x + y*y < 2 )
		{
			i--;
			continue;
		}

		tree[i].x = x;
		tree[i].y = y;
		printf("tree[%d] = [%d,%d]\n", i, tree[i].x, tree[i].y);
	}
}

void special (int key, int mx, int my)
{
	switch (key) {
	case GLUT_KEY_F2:
		restart();
		break;
	case GLUT_KEY_F3:
		showCircle = ( showCircle ) ? false : true;
		break;
	case GLUT_KEY_UP:
		if (speed < maxspeed)
			speed += 0.5;
		break;
	case GLUT_KEY_DOWN:
		if (speed > max_reverse)
			speed -= 0.5;
		break;
	case GLUT_KEY_RIGHT:
		current_origin_x += _distance*cos(angle*3.14/180); 
		current_origin_y += _distance*sin(angle*3.14/180);
		_distance = 0.0;
		angle = ((angle - 2.5) < 0) ? (360.0 - 2.5) : (angle - 2.5);
		break;
	case GLUT_KEY_LEFT:
		current_origin_x += _distance*cos(angle*3.14/180); 
		current_origin_y += _distance*sin(angle*3.14/180);
		_distance = 0.0;
		angle = ((angle + 2.5) >= 360) ? (0) : (angle + 2.5);
		break;
	}
}

int main (int argc, char** argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_RGB|GLUT_DOUBLE |GLUT_DEPTH);
	glutInitWindowSize (400,400);

	mainwin = glutCreateWindow ("drive");
	glutDisplayFunc (content);
	glutReshapeFunc (reshape);

	//viewer = ViewerNew (mainwin, 0,0,400,400, content);
	//ViewerExtent (viewer, 20);	

	srand (time(NULL));
	init();
	glutSpecialFunc (special);
	glutIdleFunc (idle);

	glutMainLoop();
}
