// Prog16.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <math.h>
#include <gl/glut.h>

GLfloat vertices[][3] = { {-1.0,-1.0,-1.0}, {1.0,-1.0,-1.0}, {1.0,1.0,-1.0}, {-1.0,1.0,-1.0}, 
                          {-1.0,-1.0, 1.0}, {1.0,-1.0, 1.0}, {1.0,1.0, 1.0}, {-1.0,1.0, 1.0} };
GLfloat colors  [][3] = { { 0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0,1.0, 0.0}, { 0.0,1.0, 0.0},
                          { 0.0, 0.0, 1.0}, {1.0, 0.0, 1.0}, {1.0,1.0, 1.0}, { 0.0,1.0, 1.0} };

static GLfloat theta = 0.0;

void init()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glOrtho     ( -2.0, 2.0, -2.0, 2.0, -10.0, 10.0 );
	glEnable    ( GL_DEPTH_TEST );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void polygon( int a, int b, int c, int d )
{
	glBegin(GL_POLYGON);
	  glColor3fv ( colors  [a] );
		glVertex3fv( vertices[a] );
		glColor3fv ( colors  [b] );
		glVertex3fv( vertices[b] );
		glColor3fv ( colors  [c] );
		glVertex3fv( vertices[c] );
		glColor3fv ( colors  [d] );
		glVertex3fv( vertices[d] );
	glEnd();
}

void spin_cube()
{
	theta += 2.0;
	if( theta > 360.0 ) theta -= 360.0;
	glutPostRedisplay();
}

void display()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glLoadIdentity();
	glRotatef(theta, 0.0, 1.0, 0.0);

	polygon(0, 3, 2, 1);
	polygon(2, 3, 7, 6);
	polygon(0, 4, 7, 3);
	polygon(1, 2, 6, 5);
	polygon(4, 5, 6, 7);
	polygon(0, 1, 5, 4);

	//glFlush();
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	glutInit( &argc, (char**)argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( 500, 500 );
	glutCreateWindow("Prog16: smooth rendering");
	glutIdleFunc(spin_cube);
	glutDisplayFunc(display);

	init();
	glutMainLoop();

	return 0;
}
