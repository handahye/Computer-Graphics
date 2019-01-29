
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
	glEnable(GL_DEPTH_TEST);//깊이 정보에 따라 이미지를 순서대로 그려줌
	glEnable(GL_LIGHTING);//gl상에서 제공하는 빛 기능을 사용
	glEnable(GL_LIGHT0);//gl에서 빛은 한장면에 최대 8개까지 사용
}

void setMinMax(float min[3], float max[3])//마우스의 최소 최대값 셋팅하는 함수
{
	max[0] = max[1] = max[2] = -RAND_MAX;//임의의 값
	min[0] = min[1] = min[2] = RAND_MAX; //임의의 값

	for (int i = 0; i<vCount; i++)//vertex갯수만큼 for문
	{
		if (pts[i][0]<min[0]) min[0] = pts[i][0];//face의 min값 셋팅
		if (pts[i][1]<min[1]) min[1] = pts[i][1];
		if (pts[i][2]<min[2]) min[2] = pts[i][2];
		if (pts[i][0]>max[0]) max[0] = pts[i][0];//face의 max값 셋팅
		if (pts[i][1]>max[1]) max[1] = pts[i][1];
		if (pts[i][2]>max[2]) max[2] = pts[i][2];
	}
}

void normalize()//정규화
{
	float min[3], max[3], Scale;
	float dimx, dimy, dimz;
	float transx, transy, transz;

	setMinMax(min, max);//최소 최댓값 설정 함수 호출

	dimx = max[0] - min[0];//최댓값과 최소값을 뺀 값 구하기->최대 x 거리
	dimy = max[1] - min[1];/
	dimz = max[2] - min[2];
	transx = min[0] + max[0];//최대값과 최소값을 더한 값은 이동할 x값
	transy = min[1] + max[1];
	transz = min[2] + max[2];
	transx *= 0.5;//trans에 0.5씩 곱해줌
	transy *= 0.5;
	transz *= 0.5;

	if (dimx>dimy && dimx>dimz)//x거리가 y거리보다 크고 x거리가 y거리보다 클 때
		Scale = 2.0f / dimx;//scale값은 2.0을 dimx로 나눈 값
	else if (dimy>dimx && dimy>dimz)
		Scale = 2.0f / dimy;
	else//나머지
		Scale = 2.0f / dimz;

	for (int i = 0; i<vCount; i++)//vCount만큼 for문 
	{
		pts[i][0] -= transx;//pts[i][0]값은 -transx만큼 이동 
		pts[i][1] -= transy; 
		pts[i][2] -= transz;
		pts[i][0] *= Scale;//scale값만큼 곱함
		pts[i][1] *= Scale;
		pts[i][2] *= Scale;
	}
}

void compute_normals()//노말벡터 계산
{
	float x, y, z;
	float x0, y0, z0, x1, y1, z1;
	float len;
	float recip;

	normal = new float*[fCount];//fCount갯수만큼 noraml 에러이 할당
	for (int l = 0; l<fCount; l++)
		normal[l] = new float[3];//3만큼 또 normal[]에 할당 

	for (int l = 0; l<vCount; l++)
		for (int i = 0; i<3; i++)
			normal[l][i] = 0;//normal 초기화시키기

	//for each face, calculate its normal
	for (int i = 0; i<fCount; i++)//각각의 face에 대한 노멀벡터 구하기
	{
		int p1, p2, p3;

		p1 = vts[i][0];// vertex값 p1 p2 p3에 값 넣기
		p2 = vts[i][1];
		p3 = vts[i][2];

		x0 = pts[p1][0] - pts[p2][0];//x0 구하기
		y0 = pts[p1][1] - pts[p2][1];
		z0 = pts[p1][2] - pts[p2][2];
		x1 = pts[p2][0] - pts[p3][0];
		y1 = pts[p2][1] - pts[p3][1];
		z1 = pts[p2][2] - pts[p3][2];

		//find cross-product between these vectors 외적해서 벡터 값 구하기
		x = y0 * z1 - z0 * y1;//벡터 x구하기
		y = z0 * x1 - x0 * z1;
		z = x0 * y1 - y0 * x1;

		//normalize this vector 구한 벡터에 대한 길이 구하기
		len = x*x + y*y + z*z;

		if (len == 0)//길이가 0이면 x, y, z는 0
			x = y = z = 0;
		else//길이가 0이 아니면
		{
			recip = 1 / sqrt(len);
			x *= recip;
			y *= recip;
			z *= recip;
		}

		normal[p1][0] += x;//noraml[p1][0]에 x값 더해주기
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
		x = normal[i][0];//앞에서 구한 normal벡터[i]로 x,y,z값 설정
		y = normal[i][1];
		z = normal[i][2];

		float len = x*x + y*y + z*z;//길이 구하기
		if (len == 0)//길이가 0이면
			x = y = z = 0;
		else//길이가 0이 아니면
		{
			recip = 1 / sqrt(len);
			x *= recip;
			y *= recip;
			z *= recip;
		}

		normal[i][0] = x;//noraml[i][0]를 x값으로 설정
		normal[i][1] = y;
		normal[i][2] = z;
	}
}

void idle_handler()//idle일때 
{
	glutPostRedisplay();
}

void keyboard_handler(unsigned char key, int x, int y)//키보드 -> 빛
{
	if (key == 'x') light.pos[0] -= 0.1;//빛위치 이동
	if (key == 'X') light.pos[0] += 0.1;
	if (key == 'y') light.pos[1] -= 0.1;
	if (key == 'Y') light.pos[1] += 0.1;
	if (key == 'z') light.pos[2] -= 0.1;
	if (key == 'Z') light.pos[2] += 0.1;

	if (key == 'a') { for (int i = 0; i<3; i++) light.amb[i] -= 0.1; }//빛 주위 광원 조절
	if (key == 'A') { for (int i = 0; i<3; i++) light.amb[i] += 0.1; }
	if (key == 'd') { for (int i = 0; i<3; i++) light.dif[i] -= 0.1; }
	if (key == 'D') { for (int i = 0; i<3; i++) light.dif[i] += 0.1; }

	if (key == 'm') mode = (mode + 1) % 3;//모드조절
}

void mouse_click_handler(GLint Button, GLint State, GLint x, GLint y)//마우스 클릭했을 때
{
	newX_ = x;//newX값에 x좌표 위치 넣기
	newY_ = y;//newY값에 y좌표 위치 넣기

	if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN) {//마우스 왼쪽버튼 눌렸을 때
		Lbutton = true;//Lbutton true값으로 바꾸기
		Rbutton = false;;//Rbutton flase값으로 바꾸기
	}
	if (Button == GLUT_RIGHT_BUTTON && State == GLUT_DOWN) {//마우스 오른쪽버튼 눌렸을 때
		Lbutton = false;
		Rbutton = true;
	}
}

void mouse_move_handler(GLint x, GLint y)//마우스 움직일 때
{
	oldX_ = newX_;//oldX값에 맨 처음 클릭했을 때 x좌표 넣기
	oldY_ = newY_;
	newX_ = x;//움직였을 때 새로운 x좌표 값을 newX값에 넣어주기
	newY_ = y;

	float RelX = (newX_ - oldX_) / (float)width_;//RelX값은 (새로운 x좌표 - 처음 클릭했을 때 x좌표)/넓이로 구함
	float RelY = (newY_ - oldY_) / (float)height_;

	if (Rbutton)//오른쪽 버튼이 클릭 됐으면 ->가깝고 먼거 조절
	{
		zRot += 10 * RelY;//zRot값은 Rely*10씩 더해줌

		if (zRot > MAX_DISTANCE) zRot = MAX_DISTANCE;//만약 zRot이 MAX_DISTANCE보다 크면 그 값을 MAX값으로 설정
		if (zRot < MIN_DISTANCE) zRot = MIN_DISTANCE;
	}
	else if (Lbutton)//왼쪽 버튼이 클릭 됐으면 -> 움직이는거 조절
	{
		yRot += (RelX * 180);//yRot은 Relx*180씩 더해줌
		xRot += (RelY * 180);
	}
}

void reshape_handler(GLint w, GLint h)//리셰이프 설정
{
	glViewport(0, 0, w, h);

	width_ = w;
	height_ = h;
}

void display()//디스플레이함수
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();//프로젝션 매트릭스 초기화
	gluPerspective(60.0f, (GLfloat)width_ / (GLfloat)height_, MIN_DISTANCE, MAX_DISTANCE);//원근투영
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();//모델뷰 매트릭스 초기화

	glClearColor(1.0, 1.0, 1.0, 1.0);//화면색
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//색깔 버퍼와 싶이 버퍼 비움
	glLoadIdentity();//매트릭스 포기화
	gluLookAt(0, 0, zRot, 0, 0, 0, 0, 1, 0);//시점 설정
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat.amb);//다각형 앞면의 재질 속성을 ambient로 설정, mat.amb는 속성을 설정한 배열의 포인터
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat.dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat.spe);
	glMaterialf(GL_FRONT, GL_SHININESS, mat.shi);

	if (mode != 2)//모드가 2가 아니면
		glShadeModel(GL_FLAT);//flat shading으로 설정
	else//아니면
		glShadeModel(GL_SMOOTH);//smooth shading으로 설정

	for (int i = 0; i<fCount; i++)//fCount만큼 for문
	{
		if (mode == 0)//mode가 0이면
			glBegin(GL_LINE_LOOP);//원그리기
		else//아니면
			glBegin(GL_TRIANGLES);//삼각형그리기

		float x, y, z;
		float nx1, ny1, nz1;
		int p1, p2, p3;

		p1 = vts[i][0];//p1에 vts[i][0]대입
		nx1 = normal[p1][0];//nx1에 노말벡터[p1] x값 대입
		ny1 = normal[p1][1];
		nz1 = normal[p1][2];
		glNormal3f(nx1, ny1, nz1);//노멀벡터 적용
		x = pts[p1][0];//x에 pts[p1] x값 대입
		y = pts[p1][1];
		z = pts[p1][2];
		glVertex3f(x, y, z);//정점

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

		glEnd();//그리기 끝내기

	}

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat2.amb);//다각형 앞면의 재질 속성을 ambient로 설정, mat2.amb는 속성을 설정한 배열의 포인터
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat2.dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat2.spe);
	glMaterialf(GL_FRONT, GL_SHININESS, mat2.shi);

	glTranslatef(light.pos[0], light.pos[1], light.pos[2]);//빛 위치 이동
	glutSolidSphere(0.05, 10, 10);//반지름 0.05의 구 그리기 -> 빛
	glLightfv(GL_LIGHT0, GL_POSITION, light.pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light.amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light.dif);

	glFlush();
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	//FILE *fpt = fopen("대원사다층석탑.ply", "r");
	//FILE *fpt = fopen("정혜사지십삼층석탑.ply", "r");
	//FILE *fpt = fopen("화엄사사사자삼층석탑.ply", "r");
	//FILE *fpt = fopen("진전사지삼층석탑.ply", "r");
	FILE *fpt = fopen("cow.ply", "r");//파일 읽어옴
	char line[2048];
	int p1, p2, p3;
	int i = 0, j = 0;

	if (!fpt)//파일이 null이면
	{
		printf("파일이 없습니다.\n");
		return 0;
	}

	while (fgets(line, 2048, fpt)) {//파일의 각 줄을 line에 읽어옴
		if (!strncmp(line, "element vertex ", 15))//읽은 line이 element vertex와 같은경우
		{
			sscanf(line + 15, "%d", &vCount);//line+15에 있는 문자열을 %d로 읽어오고 vCount에 저장-->vertex 수
		}
		if (!strncmp(line, "element face ", 13))//읽은 line이 element face와 같은경우
		{
			sscanf(line + 13, "%d", &fCount);//line+13에 있는 문자열을 %d로 읽어와서 fCount에 저장
			break;
		}
	}

	pts = new float*[vCount];//pts를 vCount의 갯수만큼 할당
	for (int l = 0; l<vCount; l++) 
		pts[l] = new float[3];//각각의 pts[]를 또 어레이 3만큼 할당

	vts = new int*[fCount];//vts를 fCount의 갯수만큼 할당
	for (int l = 0; l<fCount; l++)
		vts[l] = new int[3];//각각의 vts[]를 또 어레디 3만큼 할당

	while (strncmp(line, "end_header", 10))//파일에서 읽어온 문자열이 end_header일때까지 while문 실행
		fgets(line, 2048, fpt);//line에 읽어오기

	for (i = 0; i<vCount; i++)//vCount갯수만큼 for문
	{
		fgets(line, 2048, fpt);//line에 읽어오기
		sscanf(line, "%f %f %f", &pts[i][0], &pts[i][1], &pts[i][2]);//line에 문자열을 %f %f%f형식으로 읽어오고 pts어레이에 저장
	}

	while (fgets(line, 2048, fpt))
	{
		if (!strncmp(line, "3 ", 2))//line에서 읽어온 값이 3으로 시작하면
		{
			sscanf(line + 2, "%d %d %d", &p1, &p2, &p3);//line+2부터의 문자열을 %d %d %d형식으로 읽어와서 p1 p2 p3에 저장
			vts[j][0] = p1;//p1 p2 p3값을 vts 어레이에 넣어주기
			vts[j][1] = p2;
			vts[j][2] = p3;
			j++;
		}

	}
	fclose(fpt);//파일 닫기

	compute_normals();//노말벡터 계산 함수호출
	normalize();//normalize함수 호출

	width_ = 800;
	height_ = 600;
	glutInit(&argc, (char**)argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//디스플레이모드 설정
	glutInitWindowSize(width_, height_);//윈도우 사이즈 설정
	glutCreateWindow("ply viewer");
	glutIdleFunc(idle_handler);//idle할때 함수 설정
	glutKeyboardFunc(keyboard_handler);//키보드 함수 설정
	glutMouseFunc(mouse_click_handler);//마우스 클릭할 때 함수 설정
	glutMotionFunc(mouse_move_handler);//마우스 움직일 때 함수 설정
	glutReshapeFunc(reshape_handler);//리셰이프 콜백 함수 
	glutDisplayFunc(display);//디스플레이 함수 설정

	init();//초기화
	glutMainLoop();//이벤트 처리 루프

	free(vts);//vts할당 끝내기
	free(pts);//pts할당 끝내기
	free(normal);//normal할당 끝내기

	return 0;
}