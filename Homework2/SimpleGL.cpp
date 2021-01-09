#include <stdio.h>
#include <GL/glut.h>
#include <windows.h>

//定义窗口大小
const GLfloat windowWidth = 700;
const GLfloat windowHeight = 500;

//手臂旋转角度
GLfloat arm = 0.0f;			//手臂
GLfloat hand = -5.0f;		//手掌

GLfloat finger[5][3] = {{-5.0f,-20.0f,-30.0f},
{-5.0f,-20.0f,-30.0f},
{-5.0f,-20.0f,-30.0f},
{-5.0f,-20.0f,-30.0f},
{-5.0f,-20.0f,-30.0f}};

GLfloat distance = 3.6f;	//控制放缩
GLfloat theta = -60.0f;		//控制转动角度

//变换倍数
const GLfloat n = 0.5f;
const GLfloat n11 = 0.05f;
const GLfloat n12 = 1.2f * n11;
const GLfloat n13 = 0.9f * n11;

void myInit(void);
void display(void);
void myReshape(GLsizei w,GLsizei h);
void processNormalKeys(unsigned char key,int x, int y);	//控制关节转动
void processSpecialKeys(int key, int x, int y);			//控制放大缩小

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	/*1-设置显示模式*/
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	
	/*2-初始化窗口*/
	glutInitWindowSize(windowWidth, windowHeight);	//设置窗口大小
	glutInitWindowPosition(100, 100);				//设置窗口位置
	
	/*3-创建窗口*/
	glutCreateWindow("RobotArm");
	myInit();
	
	/*4-设置回调函数*/
	glutReshapeFunc(myReshape);				//窗口变换
	glutKeyboardFunc(processNormalKeys);	//控制关节活动
	glutSpecialFunc(processSpecialKeys);	//控制放缩和旋转
	glutDisplayFunc(display);				//渲染
	glutIdleFunc(display);
	
	/*5-启动主循环*/
	glutMainLoop();
	return 0;
}

void display()
{
	//设置清除屏幕的颜色，并清除屏幕和深度缓冲
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//设置矩阵模式为模型矩阵
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMatrixMode(GL_MODELVIEW);
	//变换
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -distance);
	glRotatef(theta, 0.0, 1.0, 0.0);
	
	//画手臂
	glTranslatef(-1.0*n, 0.0, 0.0);	//移动到手臂旋转边上
	glRotatef(arm, 0.0, 0.0, 1.0);	//绕旋转边旋转
	glTranslatef(1.0*n, 0.0, 0.0);	//返回手臂中心点进行绘制
	glPushMatrix();					
	glColor4f(1.0f, 0.0, 0.0, 0.0);
	glScalef(2.0*n, 0.4*n, 0.6*n);	//放缩比
	glutSolidCube(1.0);				
	glPopMatrix();
	
	//画手掌
	glTranslatef(1.0*n, 0.0, 0.0);	//移动到手掌旋转边上
	glRotatef(hand, 0.0, 0.0, 1.0);
	glTranslatef(0.5*n, 0.0, 0.0f);	//移动到手掌中心
	glPushMatrix();
	glColor4f(0.0f, 1.0f, 0.0, 0.0);
	glScalef(1.0*n, 0.3*n, 1.2*n);
	glutSolidCube(1.0);
	glPopMatrix();
	glTranslatef(-0.5*n,0.0,0.0f);

	//画手指
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
	
	//交换前后缓冲区
	glutSwapBuffers();
}
void myInit()
{
	glEnable(GL_DEPTH_TEST);
}
void myReshape(GLsizei w, GLsizei h)
{
	//设定视区
	glViewport(0, 0, w, h);
	//设定透视方式
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
		//手臂
	case 119://w'
		arm += 5.0f;	
		break;
	case 115://'s'
		arm -= 5.0f;
		break;
		//手掌
	case 101://'e'
		hand += 5.0f;
		break;
	case 100://'d'
		hand -= 5.0f;
		break;
		//手指1
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
		//手指2
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
		//手指3
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
		//手指4
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
		//手指5
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