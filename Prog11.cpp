// Prog11.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <math.h>
#include <gl/glut.h>

static int    delay = 10;
static double angle = 0;

void init()
{
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	// glOrtho     ( 0.0, 50.0, 0.0, 50.0, -50.0, 50.0 );
	glOrtho     ( -100.0, 100.0, -100.0, 100.0, -100.0, 100.0 );	
	glEnable    ( GL_DEPTH_TEST );
}

void rotate(GLfloat* v, GLfloat d)
{
	GLfloat x, y, z;
	double  M_PI = atan(1.0)*4.0;
	double  theta = d*M_PI/180.0;

	x = v[0]*cos(theta) + v[2]*sin(theta);
	y = v[1];
	z = v[0]*(-sin(theta)) + v[2]*cos(theta);

	v[0]=x; v[1]=y; v[2]=z;
}

void timer(int t)
{
	glutPostRedisplay();
	glutTimerFunc( delay, timer, t );

	angle += 1.0;
}

void display()
{
	GLfloat v0[3] = { 15, 15, 35 };
	GLfloat v1[3] = { 35, 15, 35 };
	GLfloat v2[3] = { 35, 35, 35 };
	GLfloat v3[3] = { 15, 35, 35 };
	GLfloat v4[3] = { 40, 20, 15 };
	GLfloat v5[3] = { 20, 20, 15 };
	GLfloat v6[3] = { 40, 40, 15 };
	GLfloat v7[3] = { 20, 40, 15 };

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	rotate(v0, angle); rotate(v1, angle); rotate(v2, angle); rotate(v3, angle); 
	rotate(v4, angle); rotate(v5, angle); rotate(v6, angle); rotate(v7, angle); 

	glColor3f( 1.0, 0.0, 0.0 );
	glBegin( GL_POLYGON );
	glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(v2); glVertex3fv(v3);
	glEnd();

	glColor3f( 0.0, 1.0, 0.0 );
	glBegin( GL_POLYGON );
	glVertex3fv(v0); glVertex3fv(v1); glVertex3fv(v4); glVertex3fv(v5);
	glEnd();

	glColor3f( 0.0, 0.0, 1.0 );
	glBegin( GL_POLYGON );
	glVertex3fv(v5); glVertex3fv(v4); glVertex3fv(v6); glVertex3fv(v7);	
	glEnd();

	glColor3f( 1.0, 1.0, 0.0 );
	glBegin( GL_POLYGON );
	glVertex3fv(v7); glVertex3fv(v6); glVertex3fv(v2); glVertex3fv(v3);	
	glEnd();

	glColor3f( 0.0, 1.0, 1.0 );
	glBegin( GL_POLYGON );
	glVertex3fv(v0); glVertex3fv(v5); glVertex3fv(v7); glVertex3fv(v3);	
	glEnd();

	glColor3f( 1.0, 0.0, 1.0 );
	glBegin( GL_POLYGON );
	glVertex3fv(v1); glVertex3fv(v4); glVertex3fv(v6); glVertex3fv(v2);	
	glEnd();

	glFlush();
}

int main(int argc, char* argv[])
{
	glutInit( &argc, (char**)argv );
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( 500, 500 );
	glutCreateWindow("Prog11: 3D cube rotation");
	glutDisplayFunc(display);
	glutTimerFunc( delay, timer, 0 );

	init();
	glutMainLoop();

	return 0;
}
