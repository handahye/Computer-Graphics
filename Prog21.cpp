// Prog21.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <math.h>
#include <gl/glut.h>

GLfloat vertices[][3] = { {-1.0,-1.0,-1.0}, {1.0,-1.0,-1.0}, {1.0,1.0,-1.0}, {-1.0,1.0,-1.0}, 
                          {-1.0,-1.0, 1.0}, {1.0,-1.0, 1.0}, {1.0,1.0, 1.0}, {-1.0,1.0, 1.0} };
GLfloat colors  [][3] = { { 0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0,1.0, 0.0}, { 0.0,1.0, 0.0},
                          { 0.0, 0.0, 1.0}, {1.0, 0.0, 1.0}, {1.0,1.0, 1.0}, { 0.0,1.0, 1.0} };
GLubyte indices []    =   { 0,3,2,1, 2,3,7,6, 0,4,7,3, 1,2,6,5, 4,5,6,7, 0,1,5,4 };

static GLfloat theta     = 0.0;
static GLint   axis      = 1;
static GLfloat viewer[3] = { 0.0, 0.0, 130.0 };
static GLint   rot       = 0;

static unsigned char buff[240][416];

void init()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
	
	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glOrtho     ( -250.0, 250.0, -250.0, 250.0, 0.0, 500.0 );
	glEnable    ( GL_DEPTH_TEST );

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// read data
	FILE *fpt=fopen("testo.y", "rb");
	fread(buff, 416*240, sizeof(char), fpt);
	fclose(fpt);
}

void spin_cube()
{
	if ( rot )
	{
		theta += 2.0;
		if( theta > 360.0 ) theta -= 360.0;
	}
	glutPostRedisplay();
}

void keyboard_handler(unsigned char key, int x, int y)
{
	if (key == 'x') viewer[0] -= 5.0; if (key == 'X') viewer[0] += 5.0;
	if (key == 'y') viewer[1] -= 5.0; if (key == 'Y') viewer[1] += 5.0;
	if (key == 'o') { viewer[0] = 0.0; viewer[1] = 0.0; viewer[2] = 130.0; }
	if (key == 'r') rot = 1-rot;
}

void display()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glLoadIdentity();
	gluLookAt( viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
	
	glRotatef( theta, 0.0, 0.0, 1.0 );
	glScalef( 1.0, -1.0, 1.0 );
	glTranslatef( -208, -120, -128  );

	GLint xp, yp, z;
	for ( int y=1; y<240-1; y++ )
	{
		for ( int x=1; x<416-1; x++ )
		{
			glBegin( GL_LINE );
			
			xp=x+0; yp=y+0; z=(GLfloat)buff[yp][xp]; glColor3f( (GLfloat)z/255, (GLfloat)z/255, (GLfloat)z/255 );
			glVertex3i( xp, yp, -z );
			glVertex3i(xp, yp, z);

			/*xp=x+1; yp=y+0; z=(GLfloat)buff[yp][xp]; glColor3f( (GLfloat)z/255, (GLfloat)z/255, (GLfloat)z/255 );
			glVertex3i( xp, yp, z );

			xp=x+1; yp=y+1; z=(GLfloat)buff[yp][xp]; glColor3f( (GLfloat)z/255, (GLfloat)z/255, (GLfloat)z/255 );
			glVertex3i( xp, yp, z );

			xp=x+0; yp=y+1; z=(GLfloat)buff[yp][xp]; glColor3f( (GLfloat)z/255, (GLfloat)z/255, (GLfloat)z/255 );
			glVertex3i( xp, yp, z );*/

			glEnd();
		}
	}

	glFlush();
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	glutInit( &argc, (char**)argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutInitWindowSize( 500, 500 );
	glutCreateWindow("Prog21: image mesh");
	glutIdleFunc(spin_cube);
	glutKeyboardFunc(keyboard_handler);
	glutDisplayFunc(display);

	init();
	glutMainLoop();

	return 0;
}
