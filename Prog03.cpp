// Prog02.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <gl/glut.h>

static int delay = 1;

static float x, y;
static float xinc, yinc;

void init()
{
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glColor3f   ( 1.0, 0.0, 0.0 );
	gluOrtho2D  ( 0.0, 50.0, 0.0, 50.0 );
	glClear     ( GL_COLOR_BUFFER_BIT );

	srand(time(0));
	x=rand()%50; y=rand()%50; xinc=(float)((rand()%50)-25)/250; yinc=(float)((rand()%50)-25)/250;
}

void display()
{
	glColor3f( x/50.0, y/50.0, 0.0 );

	glBegin( GL_POINTS );
	glVertex2f( x, y );
	glEnd();
	glFlush();

	x+=xinc; y+=yinc;
	if ( x<0 || x>50 ) { x-=xinc; xinc*=-1; }
	if ( y<0 || y>50 ) { y-=yinc; yinc*=-1; }
}

void timer(int t)
{
	glutPostRedisplay();
	glutTimerFunc( delay, timer, t );
}

int main(int argc, char* argv[])
{
	glutInit( &argc, (char**)argv );
	glutInitWindowSize( 500, 500 );
	glutCreateWindow("Prog03: Bouncing pixel");
	glutDisplayFunc(display);
	glutTimerFunc( delay, timer, 0 );

	init();
	glutMainLoop();

	return 0;
}
