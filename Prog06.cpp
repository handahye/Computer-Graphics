// Prog06.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include <stdlib.h>
#include <time.h>
#include <gl/glut.h>

static int delay = 1;

static GLfloat x[3], y[3];
static GLfloat xinc[3], yinc[3];

void init()
{
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glColor3f   ( 1.0, 0.0, 0.0 );
	gluOrtho2D  ( 0.0, 50.0, 0.0, 50.0 );
	glClear     ( GL_COLOR_BUFFER_BIT );

	srand(time(0));

	for ( int i=0; i<3; i++ )
	{
		x[i]=rand()%50; y[i]=rand()%50; xinc[i]=(GLfloat)((rand()%50)-25)/250; yinc[i]=(GLfloat)((rand()%50)-25)/250;
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f( x[0]/50.0, y[0]/50.0, 0.0 );

	glBegin( GL_TRIANGLES );

	for ( int i=0; i<3; i++ )
	{
		glVertex2f( x[i], y[i] );
	}

	glEnd();
	glFlush();

	for ( int i=0; i<3; i++ )
	{
		x[i]+=xinc[i]; y[i]+=yinc[i];
		if ( x[i]<0 || x[i]>50 ) { x[i]-=xinc[i]; xinc[i]*=-1; }
		if ( y[i]<0 || y[i]>50 ) { y[i]-=yinc[i]; yinc[i]*=-1; }
	}
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
	glutCreateWindow("Prog06: Bouncing triangle");
	glutDisplayFunc(display);
	glutTimerFunc( delay, timer, 0 );

	init();
	glutMainLoop();

	return 0;
}
