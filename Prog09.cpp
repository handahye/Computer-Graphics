// Prog09.cpp : Approximating a Sphere
//

#include "stdafx.h"
#include <stdlib.h>
#include <math.h>
#include <gl/glut.h>

void myinit()
{
	glClearColor (1.0, 1.0, 1.0, 1.0);
	glColor3f    (1.0, 0.0, 0.0);
	glOrtho      (-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
}

void display()
{
	float phi, theta;
	float phir, phir20, thetar;
	float M_PI = atan(1.0)*4;
	float c		 = M_PI/180.0;
	float x, y, z;

	glClear(GL_COLOR_BUFFER_BIT);

	for (phi=-80.0; phi<=80.0; phi+=10.0)
	{
		phir	 = c*phi;
		phir20 = c*(phi+20);

		glBegin(GL_QUAD_STRIP);
		for (theta=-180.0; theta<=180.0; theta+=10.0)
		{
			glColor3f(fabs(theta)/180, 0.0, 0.0);

			thetar = c*theta;
			x=sin(thetar)*cos(phir);
			y=cos(thetar)*cos(phir);
			z=sin(phir);
			glVertex3f(x,y,z);

			x=sin(thetar)*cos(phir20);
			y=cos(thetar)*cos(phir20);
			z=sin(phir);
			glVertex3f(x,y,z);
		}
		glEnd();
	}

	float c80=c*80.0;

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0, 0.0, 1.0);
	z=sin(c80);

	for (theta=-180.0; theta<=180.0; theta+=10.0)
	{
		glColor3f(fabs(theta)/180, 0.0, 0.0);

		thetar = c*theta;
		x=sin(thetar)*cos(c80);
		y=cos(thetar)*cos(c80);
		glVertex3f(x,y,z);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0.0, 0.0, -1.0);
	z=-sin(c80);

	for (theta=-180.0; theta<=180.0; theta+=10.0)
	{
		glColor3f(fabs(theta)/180, 0.0, 0.0);

		thetar = c*theta;
		x=sin(thetar)*cos(c80);
		y=cos(thetar)*cos(c80);
		glVertex3f(x,y,z);
	}
	glEnd();
	
	glFlush();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Simple OpenGL Example");
	glutDisplayFunc(display);
	myinit();

	glutMainLoop();

	return 0;
}
