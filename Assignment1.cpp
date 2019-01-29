
#include "stdafx.h"
#include <math.h>
#include <gl/glut.h>

static int Day = 0, Time = 0;
static int delay = 60;
float hour = 0;
float minute = 0;
float second = 0;
float angle = 0;
void draw_arm(float angle, float length)
{
	glPushMatrix();
	glRotatef(-angle, 0.0f, 0.0f, 1.0f);
	glScalef(5.5 * length, 1.0f, 1.0f);
	if (length == 1.0f)
		glColor3f(0.4, 0.7, 0.2);
	if (length == 1.2f)
		glColor3f(0.0f, 0.5f, 0.3f);
	if (length == 1.4f)
		glColor3f(0.0, 0.3, 0.3f);
	glBegin(GL_POLYGON);
	glVertex3f(-0.1, 0.0, 0.0);
	glVertex3f(0.0, 0.07, 0.0);
	glVertex3f(0.0, -0.07, 0.0);
	glEnd();
	glPopMatrix();
}
void draw_flower(int i)
{
	
	glRotatef(angle*i, 0.0f, 0.0f, -1.0f);
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(0.9, 0.8, 0.2);
	glVertex3f(-0.2, 0.2, 0.2);
	glColor3f(0.9, 0.9, 0.3);
	glVertex3f(0.2, 0.2, 0.2);
	glColor3f(0.4, 0.7, 0.2);
	glVertex3f(0.0, -0.2, 0.0);
	glEnd();
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	for (int i = 0; i < 60; i++)
	{
		glPushMatrix();
		glRotatef(angle*i, 0.0f, 0.0f, -1.0f);
		draw_flower(i);
		glPopMatrix();
	}
	draw_arm(hour, 1.0f);
	draw_arm(minute, 1.2f);
	draw_arm(second, 1.4f);
	glLoadIdentity();
	glRotatef((GLfloat)Day, 0.0, 1.0, 0.0);//È¸Àü
	glRotatef((GLfloat)Time, 0.0, 1.0, 0.0);
	glColor3f(0.0, 0.0, 0.0f);
	glutWireSphere(0.8, 10, 8);
	glPushMatrix();
		glRotatef((GLfloat)Time, 0.0, 0.1, 0.0);
		glColor3f(0.0, 0.0, 0.0);
		glutSolidSphere(0.08, 10, 8);
		glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();
}


void timer(int t) {

	angle += 6.0f;
	hour += 3.0f;
	minute += 6.0f;
	second += 12.0f;
	Day = (Day + 1) % 360;
	Time = (Time + 15) % 360; 
	glutPostRedisplay();
	glutTimerFunc(delay, timer, t);
}
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		gluOrtho2D(-2.0, 2.0, -2.0*(GLfloat)h / (GLfloat)w, 2.0*(GLfloat)h / (GLfloat)w);
	else
		gluOrtho2D(-2.0*(GLfloat)w / (GLfloat)h, 2.0*(GLfloat)w / (GLfloat)h, -2.0, 2.0);

}
int main(int argc, char* argv[])
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("²É½Ã°è");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glutDisplayFunc(display);
	glutTimerFunc(delay, timer, 0);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;
}