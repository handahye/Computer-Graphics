// Prog08.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <gl/glut.h>

void init()
{
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glOrtho     ( 0.0, 50.0, 0.0, 50.0, -50.0, 50.0 );	
	glEnable    ( GL_DEPTH_TEST );
}

void display()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glColor3f( 1.0, 0.0, 0.0 );
	glBegin( GL_POLYGON );
	glVertex3f(15, 15, 35); glVertex3f(35, 15, 35); glVertex3f(35, 35, 35); glVertex3f(15, 35, 35);	
	glEnd();

	glColor3f( 0.0, 1.0, 0.0 );
	glBegin( GL_POLYGON );
	glVertex3f(15, 15, 35); glVertex3f(35, 15, 35); glVertex3f(40, 20, 15); glVertex3f(20, 20, 15);	
	glEnd();

	glColor3f( 0.0, 0.0, 1.0 );
	glBegin( GL_POLYGON );
	glVertex3f(20, 20, 15); glVertex3f(40, 20, 15); glVertex3f(40, 40, 15); glVertex3f(20, 40, 15);	
	glEnd();

	glColor3f( 1.0, 1.0, 0.0 );
	glBegin( GL_POLYGON );
	glVertex3f(20, 40, 15); glVertex3f(40, 40, 15); glVertex3f(35, 35, 35); glVertex3f(15, 35, 35);	
	glEnd();

	glColor3f( 0.0, 1.0, 1.0 );
	glBegin( GL_POLYGON );
	glVertex3f(15, 15, 35); glVertex3f(20, 20, 15); glVertex3f(20, 40, 15); glVertex3f(15, 35, 35);	
	glEnd();

	glColor3f( 1.0, 0.0, 1.0 );
	glBegin( GL_POLYGON );
	glVertex3f(35, 15, 35); glVertex3f(40, 20, 15); glVertex3f(40, 40, 15); glVertex3f(35, 35, 35);	
	glEnd();

	glFlush();
}

int main(int argc, char* argv[])
{
	glutInit( &argc, (char**)argv );
	glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( 500, 500 );
	glutCreateWindow("Prog08: hidden surface removal");
	glutDisplayFunc(display);

	init();
	glutMainLoop();

	return 0;
}
