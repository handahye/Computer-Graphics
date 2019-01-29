// Prog10.cpp
//

#include "stdafx.h"
#include <math.h>
#include <gl/glut.h>

static double theta    = 0;
static double thetainc = 5;
static int    delay    = 100;

void myinit()
{
	glClearColor (1.0, 1.0, 1.0, 1.0);
	glColor3f    (1.0, 0.0, 0.0);
	glOrtho      (0.0, 416.0, 0.0, 240.0, -500.0, 500.0);
	glPointSize  (8);
}

void display()
{
	// read data
	static unsigned char buff[240][416];
	FILE *fpt=fopen("testo.y", "rb");
	fread(buff, 416*240, sizeof(char), fpt);
	fclose(fpt);
	
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POINTS);

	double M_PI = atan(1.0)*4.0;
	double rad  = M_PI/180.0;

	for ( int y=0; y<240; y++ )
	{
		for ( int x=0; x<416; x++ )
		{
			GLfloat c  = (GLfloat)buff[y][x]/255;
			GLfloat z  = (GLfloat)x*sin(theta*rad);
			GLfloat xp = (GLfloat)x*cos(theta*rad);
			GLfloat yp = 239-(GLfloat)y;

			glColor3f ( c, c, c   );
			glVertex3f( xp, yp, z );
		}
	}

	glEnd();

	glFlush();
}

void timer(int t)
{
	glutPostRedisplay();
	glutTimerFunc( delay, timer, t );

	theta+=thetainc;
	if ( theta<=0 || theta>80 )
	{
		theta -= thetainc;
		thetainc = -thetainc;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Prog10: Image in 3D");
	glutDisplayFunc(display);
	glutTimerFunc( delay, timer, 0 );

	myinit();

	glutMainLoop();

	return 0;
}
