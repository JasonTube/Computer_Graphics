#include <stdio.h>
#include <GL/glut.h>
#include <windows.h>

//���崰�ڴ�С
const GLfloat windowWidth = 700;
const GLfloat windowHeight = 500;

//�ֱ���ת�Ƕ�
GLfloat arm = 0.0f;			//�ֱ�
GLfloat hand = -5.0f;		//����

GLfloat finger[5][3] = {{-5.0f,-20.0f,-30.0f},
{-5.0f,-20.0f,-30.0f},
{-5.0f,-20.0f,-30.0f},
{-5.0f,-20.0f,-30.0f},
{-5.0f,-20.0f,-30.0f}};

GLfloat distance = 3.6f;	//���Ʒ���
GLfloat theta = -60.0f;		//����ת���Ƕ�

//�任����
const GLfloat n = 0.5f;
const GLfloat n11 = 0.05f;
const GLfloat n12 = 1.2f * n11;
const GLfloat n13 = 0.9f * n11;

void myInit(void);
void display(void);
void myReshape(GLsizei w,GLsizei h);
void processNormalKeys(unsigned char key,int x, int y);	//���ƹؽ�ת��
void processSpecialKeys(int key, int x, int y);			//���ƷŴ���С

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	/*1-������ʾģʽ*/
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	
	/*2-��ʼ������*/
	glutInitWindowSize(windowWidth, windowHeight);	//���ô��ڴ�С
	glutInitWindowPosition(100, 100);				//���ô���λ��
	
	/*3-��������*/
	glutCreateWindow("RobotArm");
	myInit();
	
	/*4-���ûص�����*/
	glutReshapeFunc(myReshape);				//���ڱ任
	glutKeyboardFunc(processNormalKeys);	//���ƹؽڻ
	glutSpecialFunc(processSpecialKeys);	//���Ʒ�������ת
	glutDisplayFunc(display);				//��Ⱦ
	glutIdleFunc(display);
	
	/*5-������ѭ��*/
	glutMainLoop();
	return 0;
}

void display()
{
	//���������Ļ����ɫ���������Ļ����Ȼ���
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//���þ���ģʽΪģ�;���
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMatrixMode(GL_MODELVIEW);
	//�任
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -distance);
	glRotatef(theta, 0.0, 1.0, 0.0);
	
	//���ֱ�
	glTranslatef(-1.0*n, 0.0, 0.0);	//�ƶ����ֱ���ת����
	glRotatef(arm, 0.0, 0.0, 1.0);	//����ת����ת
	glTranslatef(1.0*n, 0.0, 0.0);	//�����ֱ����ĵ���л���
	glPushMatrix();					
	glColor4f(1.0f, 0.0, 0.0, 0.0);
	glScalef(2.0*n, 0.4*n, 0.6*n);	//������
	glutSolidCube(1.0);				
	glPopMatrix();
	
	//������
	glTranslatef(1.0*n, 0.0, 0.0);	//�ƶ���������ת����
	glRotatef(hand, 0.0, 0.0, 1.0);
	glTranslatef(0.5*n, 0.0, 0.0f);	//�ƶ�����������
	glPushMatrix();
	glColor4f(0.0f, 1.0f, 0.0, 0.0);
	glScalef(1.0*n, 0.3*n, 1.2*n);
	glutSolidCube(1.0);
	glPopMatrix();
	glTranslatef(-0.5*n,0.0,0.0f);

	//����ָ
	for(int i = 0;i<5;++i)
	{
		glPushMatrix();
		glTranslatef(0, 0, -1.0 * n/2 + i*n/4);
		
		glTranslatef(1.0 * n, 0.0, 0.0);
		glRotatef(finger[i][0], 0.0, 0.0, 1.0);
		glTranslatef(1.0 * n11, 0.0, 0.0f);
		glPushMatrix();
		glColor4f(0.0f, 1.0f, 1.0f, 0.0);
		glScalef(2.0 * n11, 0.4 * n11, 1.0 * n11);
		glutSolidCube(1.0);
		glPopMatrix();
		
		glTranslatef(1.0 * n11, 0.0, 0.0);
		glRotatef(finger[i][1], 0.0, 0.0, 1.0);
		glTranslatef(1.0 * n12, 0.0, 0.0f);
		glPushMatrix();
		glColor4f(0.0f, 1.0f, 1.0f, 0.0);
		glScalef(2.0 * n12, 0.4 * n12, 1.0 * n12);
		glutSolidCube(1.0);
		glPopMatrix();
		
		glTranslatef(1.0 * n12, 0.0, 0.0);
		glRotatef(finger[i][2], 0.0, 0.0, 1.0);
		glTranslatef(1.0 * n13, 0.0, 0.0f);
		glPushMatrix();
		glColor4f(0.0f, 1.0f, 1.0f, 0.0);
		glScalef(2.0 * n13, 0.4 * n13, 1.0 * n13);
		glutSolidCube(1.0);
		glPopMatrix();
		
		glPopMatrix();
	}
	
	//����ǰ�󻺳���
	glutSwapBuffers();
}
void myInit()
{
	glEnable(GL_DEPTH_TEST);
}
void myReshape(GLsizei w, GLsizei h)
{
	//�趨����
	glViewport(0, 0, w, h);
	//�趨͸�ӷ�ʽ
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0 * (GLfloat)w / (GLfloat)h, 1.0, 30.0);
}

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
		//all
	case 113://q'
		{
			arm += 5.0f;
			hand += 5.0f;
			for(int i = 0;i<5;++i)
			{
				for(int j = 0;j<3;++j)
				{
					finger[i][j] += 5.0f;
				}
			}
			break;
		}
	case 97://'a'
		{
			arm -= 5.0f;
			hand -= 5.0f;
			for(int i = 0;i<5;++i)
			{
				for(int j = 0;j<3;++j)
				{
					finger[i][j] -= 5.0f;
				}
			}
			break;
		}
		//�ֱ�
	case 119://w'
		arm += 5.0f;	
		break;
	case 115://'s'
		arm -= 5.0f;
		break;
		//����
	case 101://'e'
		hand += 5.0f;
		break;
	case 100://'d'
		hand -= 5.0f;
		break;
		//��ָ1
	case 114://'r'
		finger[4][0] += 5.0f;
		finger[4][1] += 5.0f;
		finger[4][2] += 5.0f;
		break;
	case 102://'f'
		finger[4][0] -= 5.0f;
		finger[4][1] -= 5.0f;
		finger[4][2] -= 5.0f;
		break;
		//��ָ2
	case 116://'t'
		finger[3][0] += 5.0f;
		finger[3][1] += 5.0f;
		finger[3][2] += 5.0f;
		break;
	case 103://'g'
		finger[3][0] -= 5.0f;
		finger[3][1] -= 5.0f;
		finger[3][2] -= 5.0f;
		break;
		//��ָ3
	case 121://'y'
		finger[2][0] += 5.0f;
		finger[2][1] += 5.0f;
		finger[2][2] += 5.0f;
		break;
	case 104://'h'
		finger[2][0] -= 5.0f;
		finger[2][1] -= 5.0f;
		finger[2][2] -= 5.0f;
		break;
		//��ָ4
	case 117://'u'
		finger[1][0] += 5.0f;
		finger[1][1] += 5.0f;
		finger[1][2] += 5.0f;
		break;
	case 106://'j'
		finger[1][0] -= 5.0f;
		finger[1][1] -= 5.0f;
		finger[1][2] -= 5.0f;
		break;
		//��ָ5
	case 105://'i'
		finger[0][0] += 5.0f;
		finger[0][1] += 5.0f;
		finger[0][2] += 5.0f;
		break;
	case 107://'k'
		finger[0][0] -= 5.0f;
		finger[0][1] -= 5.0f;
		finger[0][2] -= 5.0f;
		break;
	}
}

void processSpecialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		theta -= 1.0f;
		break;
	case GLUT_KEY_RIGHT:
		theta += 1.0f;
		break;
	case GLUT_KEY_UP:
		distance += 0.1f;
		break;
	case GLUT_KEY_DOWN:
		distance -= 0.1f;
		break;
	}
}