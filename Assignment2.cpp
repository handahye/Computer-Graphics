
#include "stdafx.h"
#include <math.h>
#include <memory.h>
#include <gl/glut.h>
#include <fstream>


typedef struct {
	GLfloat pos[4];   // position
	GLfloat amb[4];   // ambient
	GLfloat dif[4];   // diffuse
	GLfloat spe[4];   // specular
} Light;


typedef struct {
	GLfloat amb[4];   // ambient
	GLfloat dif[4];   // diffuse
	GLfloat spe[4];   // specular
	GLfloat shi;    // shininess
} Material;

typedef struct _viewer
{
	GLfloat         eye[3];  // viewer position
	GLfloat         at[3];  // center
	GLfloat         up[3];  // up direction
} Viewer;

GLint mode = 0;
GLint vCount = 0;
GLint fCount = 0;
GLint **vts;
GLfloat **pts;
GLfloat **normal;
GLfloat xRot = 0.0f;
GLfloat yRot = 0.0f;
GLfloat zRot = 3.0f;

GLint oldX_ = 0, oldY_ = 0, newX_ = 0, newY_ = 0;   // For Mouse
bool Lbutton = false, Rbutton = false;
const GLfloat MIN_DISTANCE = 0.001f;
const GLfloat MAX_DISTANCE = 1000.0f;
int width_ = 0, height_ = 0;

Light light = {
	{ 0.5, 0.5, 1.0, 1.0 },   // position
	{ 1.0, 1.0, 1.0, 1.0 }, // ambient
	{ 1.0, 1.0, 1.0, 1.0 }, // diffuse
	{ 1.0, 1.0, 1.0, 1.0 }, // specular
};

Material mat = {
	{ 0.2, 0.0, 0.0, 1.0 }, // ambient
	{ 0.8, 0.8, 0.4, 1.0 }, // diffuse
	{ 1.0, 1.0, 1.0, 1.0 }, // specular
	{ 500.0 }, // shininess
};

Material mat2 = {
	{ 0.2, 0.2, 0.2, 1.0 }, // ambient
	{ 1.0, 1.0, 1.0, 1.0 }, // diffuse
	{ 1.0, 1.0, 1.0, 1.0 }, // specular
	{ 100.0 }, // shininess
};



void init()
{
	glEnable(GL_DEPTH_TEST);//���� ������ ���� �̹����� ������� �׷���
	glEnable(GL_LIGHTING);//gl�󿡼� �����ϴ� �� ����� ���
	glEnable(GL_LIGHT0);//gl���� ���� ����鿡 �ִ� 8������ ���
}

void setMinMax(float min[3], float max[3])//���콺�� �ּ� �ִ밪 �����ϴ� �Լ�
{
	max[0] = max[1] = max[2] = -RAND_MAX;//������ ��
	min[0] = min[1] = min[2] = RAND_MAX; //������ ��

	for (int i = 0; i<vCount; i++)//vertex������ŭ for��
	{
		if (pts[i][0]<min[0]) min[0] = pts[i][0];//face�� min�� ����
		if (pts[i][1]<min[1]) min[1] = pts[i][1];
		if (pts[i][2]<min[2]) min[2] = pts[i][2];
		if (pts[i][0]>max[0]) max[0] = pts[i][0];//face�� max�� ����
		if (pts[i][1]>max[1]) max[1] = pts[i][1];
		if (pts[i][2]>max[2]) max[2] = pts[i][2];
	}
}

void normalize()//����ȭ
{
	float min[3], max[3], Scale;
	float dimx, dimy, dimz;
	float transx, transy, transz;

	setMinMax(min, max);//�ּ� �ִ� ���� �Լ� ȣ��

	dimx = max[0] - min[0];//�ִ񰪰� �ּҰ��� �� �� ���ϱ�->�ִ� x �Ÿ�
	dimy = max[1] - min[1];/
	dimz = max[2] - min[2];
	transx = min[0] + max[0];//�ִ밪�� �ּҰ��� ���� ���� �̵��� x��
	transy = min[1] + max[1];
	transz = min[2] + max[2];
	transx *= 0.5;//trans�� 0.5�� ������
	transy *= 0.5;
	transz *= 0.5;

	if (dimx>dimy && dimx>dimz)//x�Ÿ��� y�Ÿ����� ũ�� x�Ÿ��� y�Ÿ����� Ŭ ��
		Scale = 2.0f / dimx;//scale���� 2.0�� dimx�� ���� ��
	else if (dimy>dimx && dimy>dimz)
		Scale = 2.0f / dimy;
	else//������
		Scale = 2.0f / dimz;

	for (int i = 0; i<vCount; i++)//vCount��ŭ for�� 
	{
		pts[i][0] -= transx;//pts[i][0]���� -transx��ŭ �̵� 
		pts[i][1] -= transy; 
		pts[i][2] -= transz;
		pts[i][0] *= Scale;//scale����ŭ ����
		pts[i][1] *= Scale;
		pts[i][2] *= Scale;
	}
}

void compute_normals()//�븻���� ���
{
	float x, y, z;
	float x0, y0, z0, x1, y1, z1;
	float len;
	float recip;

	normal = new float*[fCount];//fCount������ŭ noraml ������ �Ҵ�
	for (int l = 0; l<fCount; l++)
		normal[l] = new float[3];//3��ŭ �� normal[]�� �Ҵ� 

	for (int l = 0; l<vCount; l++)
		for (int i = 0; i<3; i++)
			normal[l][i] = 0;//normal �ʱ�ȭ��Ű��

	//for each face, calculate its normal
	for (int i = 0; i<fCount; i++)//������ face�� ���� ��ֺ��� ���ϱ�
	{
		int p1, p2, p3;

		p1 = vts[i][0];// vertex�� p1 p2 p3�� �� �ֱ�
		p2 = vts[i][1];
		p3 = vts[i][2];

		x0 = pts[p1][0] - pts[p2][0];//x0 ���ϱ�
		y0 = pts[p1][1] - pts[p2][1];
		z0 = pts[p1][2] - pts[p2][2];
		x1 = pts[p2][0] - pts[p3][0];
		y1 = pts[p2][1] - pts[p3][1];
		z1 = pts[p2][2] - pts[p3][2];

		//find cross-product between these vectors �����ؼ� ���� �� ���ϱ�
		x = y0 * z1 - z0 * y1;//���� x���ϱ�
		y = z0 * x1 - x0 * z1;
		z = x0 * y1 - y0 * x1;

		//normalize this vector ���� ���Ϳ� ���� ���� ���ϱ�
		len = x*x + y*y + z*z;

		if (len == 0)//���̰� 0�̸� x, y, z�� 0
			x = y = z = 0;
		else//���̰� 0�� �ƴϸ�
		{
			recip = 1 / sqrt(len);
			x *= recip;
			y *= recip;
			z *= recip;
		}

		normal[p1][0] += x;//noraml[p1][0]�� x�� �����ֱ�
		normal[p1][1] += y;
		normal[p1][2] += z;

		normal[p2][0] += x;
		normal[p2][1] += y;
		normal[p2][2] += z;

		normal[p3][0] += x;
		normal[p3][1] += y;
		normal[p3][2] += z;
	}

	for (int i = 0; i<vCount; i++)
	{
		x = normal[i][0];//�տ��� ���� normal����[i]�� x,y,z�� ����
		y = normal[i][1];
		z = normal[i][2];

		float len = x*x + y*y + z*z;//���� ���ϱ�
		if (len == 0)//���̰� 0�̸�
			x = y = z = 0;
		else//���̰� 0�� �ƴϸ�
		{
			recip = 1 / sqrt(len);
			x *= recip;
			y *= recip;
			z *= recip;
		}

		normal[i][0] = x;//noraml[i][0]�� x������ ����
		normal[i][1] = y;
		normal[i][2] = z;
	}
}

void idle_handler()//idle�϶� 
{
	glutPostRedisplay();
}

void keyboard_handler(unsigned char key, int x, int y)//Ű���� -> ��
{
	if (key == 'x') light.pos[0] -= 0.1;//����ġ �̵�
	if (key == 'X') light.pos[0] += 0.1;
	if (key == 'y') light.pos[1] -= 0.1;
	if (key == 'Y') light.pos[1] += 0.1;
	if (key == 'z') light.pos[2] -= 0.1;
	if (key == 'Z') light.pos[2] += 0.1;

	if (key == 'a') { for (int i = 0; i<3; i++) light.amb[i] -= 0.1; }//�� ���� ���� ����
	if (key == 'A') { for (int i = 0; i<3; i++) light.amb[i] += 0.1; }
	if (key == 'd') { for (int i = 0; i<3; i++) light.dif[i] -= 0.1; }
	if (key == 'D') { for (int i = 0; i<3; i++) light.dif[i] += 0.1; }

	if (key == 'm') mode = (mode + 1) % 3;//�������
}

void mouse_click_handler(GLint Button, GLint State, GLint x, GLint y)//���콺 Ŭ������ ��
{
	newX_ = x;//newX���� x��ǥ ��ġ �ֱ�
	newY_ = y;//newY���� y��ǥ ��ġ �ֱ�

	if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN) {//���콺 ���ʹ�ư ������ ��
		Lbutton = true;//Lbutton true������ �ٲٱ�
		Rbutton = false;;//Rbutton flase������ �ٲٱ�
	}
	if (Button == GLUT_RIGHT_BUTTON && State == GLUT_DOWN) {//���콺 �����ʹ�ư ������ ��
		Lbutton = false;
		Rbutton = true;
	}
}

void mouse_move_handler(GLint x, GLint y)//���콺 ������ ��
{
	oldX_ = newX_;//oldX���� �� ó�� Ŭ������ �� x��ǥ �ֱ�
	oldY_ = newY_;
	newX_ = x;//�������� �� ���ο� x��ǥ ���� newX���� �־��ֱ�
	newY_ = y;

	float RelX = (newX_ - oldX_) / (float)width_;//RelX���� (���ο� x��ǥ - ó�� Ŭ������ �� x��ǥ)/���̷� ����
	float RelY = (newY_ - oldY_) / (float)height_;

	if (Rbutton)//������ ��ư�� Ŭ�� ������ ->������ �հ� ����
	{
		zRot += 10 * RelY;//zRot���� Rely*10�� ������

		if (zRot > MAX_DISTANCE) zRot = MAX_DISTANCE;//���� zRot�� MAX_DISTANCE���� ũ�� �� ���� MAX������ ����
		if (zRot < MIN_DISTANCE) zRot = MIN_DISTANCE;
	}
	else if (Lbutton)//���� ��ư�� Ŭ�� ������ -> �����̴°� ����
	{
		yRot += (RelX * 180);//yRot�� Relx*180�� ������
		xRot += (RelY * 180);
	}
}

void reshape_handler(GLint w, GLint h)//�������� ����
{
	glViewport(0, 0, w, h);

	width_ = w;
	height_ = h;
}

void display()//���÷����Լ�
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();//�������� ��Ʈ���� �ʱ�ȭ
	gluPerspective(60.0f, (GLfloat)width_ / (GLfloat)height_, MIN_DISTANCE, MAX_DISTANCE);//��������
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();//�𵨺� ��Ʈ���� �ʱ�ȭ

	glClearColor(1.0, 1.0, 1.0, 1.0);//ȭ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//���� ���ۿ� ���� ���� ���
	glLoadIdentity();//��Ʈ���� ����ȭ
	gluLookAt(0, 0, zRot, 0, 0, 0, 0, 1, 0);//���� ����
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat.amb);//�ٰ��� �ո��� ���� �Ӽ��� ambient�� ����, mat.amb�� �Ӽ��� ������ �迭�� ������
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat.spe);
	glMaterialf(GL_FRONT, GL_SHININESS, mat.shi);

	if (mode != 2)//��尡 2�� �ƴϸ�
		glShadeModel(GL_FLAT);//flat shading���� ����
	else//�ƴϸ�
		glShadeModel(GL_SMOOTH);//smooth shading���� ����

	for (int i = 0; i<fCount; i++)//fCount��ŭ for��
	{
		if (mode == 0)//mode�� 0�̸�
			glBegin(GL_LINE_LOOP);//���׸���
		else//�ƴϸ�
			glBegin(GL_TRIANGLES);//�ﰢ���׸���

		float x, y, z;
		float nx1, ny1, nz1;
		int p1, p2, p3;

		p1 = vts[i][0];//p1�� vts[i][0]����
		nx1 = normal[p1][0];//nx1�� �븻����[p1] x�� ����
		ny1 = normal[p1][1];
		nz1 = normal[p1][2];
		glNormal3f(nx1, ny1, nz1);//��ֺ��� ����
		x = pts[p1][0];//x�� pts[p1] x�� ����
		y = pts[p1][1];
		z = pts[p1][2];
		glVertex3f(x, y, z);//����

		p2 = vts[i][1];
		nx1 = normal[p2][0];
		ny1 = normal[p2][1];
		nz1 = normal[p2][2];
		glNormal3f(nx1, ny1, nz1);
		x = pts[p2][0];
		y = pts[p2][1];
		z = pts[p2][2];
		glVertex3f(x, y, z);

		p3 = vts[i][2];
		nx1 = normal[p3][0];
		ny1 = normal[p3][1];
		nz1 = normal[p3][2];
		glNormal3f(nx1, ny1, nz1);
		x = pts[p3][0];
		y = pts[p3][1];
		z = pts[p3][2];
		glVertex3f(x, y, z);

		glEnd();//�׸��� ������

	}

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat2.amb);//�ٰ��� �ո��� ���� �Ӽ��� ambient�� ����, mat2.amb�� �Ӽ��� ������ �迭�� ������
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat2.dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat2.spe);
	glMaterialf(GL_FRONT, GL_SHININESS, mat2.shi);

	glTranslatef(light.pos[0], light.pos[1], light.pos[2]);//�� ��ġ �̵�
	glutSolidSphere(0.05, 10, 10);//������ 0.05�� �� �׸��� -> ��
	glLightfv(GL_LIGHT0, GL_POSITION, light.pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light.amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light.dif);

	glFlush();
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	//FILE *fpt = fopen("����������ž.ply", "r");
	//FILE *fpt = fopen("���������ʻ�����ž.ply", "r");
	//FILE *fpt = fopen("ȭ�������ڻ�����ž.ply", "r");
	//FILE *fpt = fopen("��������������ž.ply", "r");
	FILE *fpt = fopen("cow.ply", "r");//���� �о��
	char line[2048];
	int p1, p2, p3;
	int i = 0, j = 0;

	if (!fpt)//������ null�̸�
	{
		printf("������ �����ϴ�.\n");
		return 0;
	}

	while (fgets(line, 2048, fpt)) {//������ �� ���� line�� �о��
		if (!strncmp(line, "element vertex ", 15))//���� line�� element vertex�� �������
		{
			sscanf(line + 15, "%d", &vCount);//line+15�� �ִ� ���ڿ��� %d�� �о���� vCount�� ����-->vertex ��
		}
		if (!strncmp(line, "element face ", 13))//���� line�� element face�� �������
		{
			sscanf(line + 13, "%d", &fCount);//line+13�� �ִ� ���ڿ��� %d�� �о�ͼ� fCount�� ����
			break;
		}
	}

	pts = new float*[vCount];//pts�� vCount�� ������ŭ �Ҵ�
	for (int l = 0; l<vCount; l++) 
		pts[l] = new float[3];//������ pts[]�� �� ��� 3��ŭ �Ҵ�

	vts = new int*[fCount];//vts�� fCount�� ������ŭ �Ҵ�
	for (int l = 0; l<fCount; l++)
		vts[l] = new int[3];//������ vts[]�� �� ��� 3��ŭ �Ҵ�

	while (strncmp(line, "end_header", 10))//���Ͽ��� �о�� ���ڿ��� end_header�϶����� while�� ����
		fgets(line, 2048, fpt);//line�� �о����

	for (i = 0; i<vCount; i++)//vCount������ŭ for��
	{
		fgets(line, 2048, fpt);//line�� �о����
		sscanf(line, "%f %f %f", &pts[i][0], &pts[i][1], &pts[i][2]);//line�� ���ڿ��� %f %f%f�������� �о���� pts��̿� ����
	}

	while (fgets(line, 2048, fpt))
	{
		if (!strncmp(line, "3 ", 2))//line���� �о�� ���� 3���� �����ϸ�
		{
			sscanf(line + 2, "%d %d %d", &p1, &p2, &p3);//line+2������ ���ڿ��� %d %d %d�������� �о�ͼ� p1 p2 p3�� ����
			vts[j][0] = p1;//p1 p2 p3���� vts ��̿� �־��ֱ�
			vts[j][1] = p2;
			vts[j][2] = p3;
			j++;
		}

	}
	fclose(fpt);//���� �ݱ�

	compute_normals();//�븻���� ��� �Լ�ȣ��
	normalize();//normalize�Լ� ȣ��

	width_ = 800;
	height_ = 600;
	glutInit(&argc, (char**)argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//���÷��̸�� ����
	glutInitWindowSize(width_, height_);//������ ������ ����
	glutCreateWindow("ply viewer");
	glutIdleFunc(idle_handler);//idle�Ҷ� �Լ� ����
	glutKeyboardFunc(keyboard_handler);//Ű���� �Լ� ����
	glutMouseFunc(mouse_click_handler);//���콺 Ŭ���� �� �Լ� ����
	glutMotionFunc(mouse_move_handler);//���콺 ������ �� �Լ� ����
	glutReshapeFunc(reshape_handler);//�������� �ݹ� �Լ� 
	glutDisplayFunc(display);//���÷��� �Լ� ����

	init();//�ʱ�ȭ
	glutMainLoop();//�̺�Ʈ ó�� ����

	free(vts);//vts�Ҵ� ������
	free(pts);//pts�Ҵ� ������
	free(normal);//normal�Ҵ� ������

	return 0;
}