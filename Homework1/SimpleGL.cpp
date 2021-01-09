#include<stdio.h>
#include<gl/GLUT.H>
#define GLUT_DISABLE_ATEXIT_HACK

static float year = 0, day = 0;
GLfloat theta = 0.0f;

void init(void)
{
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_FLAT);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,1.0,1.0);
	glLoadIdentity();
	
	glRotatef(theta, 0.0, 0.0, 1.0);
	glPushMatrix();
	gluLookAt(0.0,0.0,5,0.0,0.0,0.0,0.0,1.0,0.0);
	
	//Ì«Ñô
	glRotatef((GLfloat)0.07*year,0.0,1.0,0.0);
	glutWireSphere(0.8,20,20);
	
	//µØÇò
	glRotatef((GLfloat)0.93*year,0.0,1.0,0.0);
	glTranslatef(2.0,0.0,0.0);
	glRotatef((GLfloat)day,0.0,1.0,0.0);
	glutWireSphere(0.2,20,20);
	
	//ÔÂÇò
	glRotatef((GLfloat)year,0.0,1.0,0.0);
	glTranslatef(0.4,0.0,0.0);
	glRotatef((GLfloat)day,0.0,1.0,0.0);
	glutWireSphere(0.1,15,15);
	
    glPopMatrix();
	glutSwapBuffers();
}


void spinDisplay(void)
{
	year = year + 0.03;
    if ( year > 360.0)
	{
		year = year - 360.0;
	}
	day = day + 0.06;
    if ( day > 360.0)
	{
		day = day - 360.0;
	}
	
	
	glutPostRedisplay();
}


void reshape(int w,int h)
{
	glViewport(0,0,(GLsizei) w,(GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0,1.0,-1.0,1.0,1.5,20);
	glMatrixMode(GL_MODELVIEW);
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
	}
}

int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(600,600);
	glutInitWindowPosition(50,50);
	glutCreateWindow(argv[0]);
	init();
	glutSpecialFunc(processSpecialKeys);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(spinDisplay);
	glutMainLoop();
	return 0;
}