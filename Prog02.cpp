// Prog02.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <gl/glut.h>

static int delay = 10;

void init()
{
	srand(time(0));
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glColor3f   ( 1.0, 0.0, 0.0 );
	gluOrtho2D  ( 0.0, 50.0, 0.0, 50.0 );
	glClear     ( GL_COLOR_BUFFER_BIT );
}

void display()
{
	int x1, y1, x2, y2, r, g, b;
	float a;
	x1=rand()%50;  y1=rand()%50;  x2=rand()%50; y2=rand()%50;
	r =rand()%256; g =rand()%256; b =rand()%256;
	a = rand();

	printf("%lf\n", a);

	glColor3f( (GLfloat)r/255, (GLfloat)g/255, (GLfloat)b/255 );
		
	glBegin( GL_POLYGON );
	glVertex2f( x1, y1 ); glVertex2f( x1, y2 );
	glVertex2f( x2, y2 );	glVertex2f( x2, y1 );
	glEnd();

	glFlush();
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
	glutCreateWindow("Prog02: Random boxes");
	glutDisplayFunc(display);
	glutTimerFunc( delay, timer, 0 );

	init();
	glutMainLoop();

	return 0;
}
