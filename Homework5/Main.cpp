#include <gl/glut.h>
#include "PlyInput.h"
#include <string.h>
#include <iostream.h>
#include <gl/glaux.h>
#include <stdio.h>
#pragma comment(lib, "glaux.lib")

#define GLUT_DISABLE_ATEXIT_HACK
#define GL_LIGHT_MODEL_COLOR_CONTROL 0x81F8
#define GL_SEPARATE_SPECULAR_COLOR 0x81FA

const int width=500;
const int height=500;

GLint ArmadilloList=0;

GLfloat xRotation = 0.0f, yRotation = 0.0f;
const GLfloat rotSpeed = 50.0f;//速度
bool bLButtonDown = false;

const double half_width = 2.0;

FPoint3 *vertices = NULL;
FPoint3 *normals = NULL;
int nvertices = 0;
int* indices = NULL;
int nindices = 0;

GLfloat SclFac = 1.0f, lastSclFac = 1.0f;
GLfloat xTransl = 0.0f, yTransl = 0.0f;

int lastPx,lastPy;
bool bMButtonDown,bRButtonDown;

GLfloat LightPos[] = {0.5f*100,0.2f*100,0.0f,1.0f};
GLfloat lightSpin = 0.0f;
GLfloat lightSpeed = 0.2f;

FPoint3 CalcNormal(const FPoint3 &v0,const FPoint3 &v1,const FPoint3 &v2){
	FPoint3 p1,p2,p3;
	p1=v1-v0;
	p2=v2-v0;
	p3 = p1.cross(p2);
	return p3;
}

void init(){
	LoadSimplePlyFile("MyArmadillo.ply",vertices,nvertices,indices,nindices);

	//计算法向量的均值
	int *counts = new int[nvertices];
	memset(counts, 0 , sizeof(int) * nvertices);
	normals = new FPoint3[nindices];
	memset(normals,0,sizeof(FPoint3) * nvertices);

	memset(normals,0,12 * nvertices);

	int nFaces;
	nFaces = nindices / 3;
	int offset = 0;

	for (int i = 0; i < nFaces; i++) {
		int ind0 = indices[offset++];
		const FPoint3 &v0 = vertices[ind0];
		int ind1 = indices[offset++];
		const FPoint3 &v1 = vertices[ind1];
		int ind2 = indices[offset++];
		const FPoint3 &v2 = vertices[ind2];
		
		FPoint3 norm = CalcNormal(v0, v1, v2);
		normals[ind0] += norm;
		counts[ind0]++;
		normals[ind1] += norm;
		counts[ind1]++;
		normals[ind2] += norm;
		counts[ind2]++;
	}

	for (int j = 0; j < nvertices; j++) {
		FPoint3 &normal = normals[j];
		normal /= counts[j];
		normal.normalize();
	}

	delete[] counts;
	
	GLfloat mat_specular[]={1.0, 1.0, 1.0, 1.0};
	GLfloat mat_diffuse[]={1.0, 1.0, 1.0, 1.0};
	GLfloat mat_ambient[]={1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess={100.0};
	GLfloat light_ambient[]={0.0, 0.0, 0.0, 1.0};
	GLfloat light_diffuse[]={1.0, 1.0, 1.0, 1.0};
	GLfloat light_specular[]={1.0, 1.0, 1.0, 1.0};

	/* set up ambient, diffuse, and specular components for light 0 */
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	/* define material proerties for front face of all polygons */
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_NORMALIZE);
	
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

}

void keyboard ( unsigned char key, int x, int y)
{
     
    switch (key)  
    {
		case 27:
			exit (0); 
			break;
    }
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (bLButtonDown = state == GLUT_DOWN) {lastPx = x;lastPy = y;}
		break;
	case GLUT_MIDDLE_BUTTON:
		if (bMButtonDown = state == GLUT_DOWN) {lastPx = x;lastPy = y;}
		break;
	case GLUT_RIGHT_BUTTON:
		if (bRButtonDown = state == GLUT_DOWN) {lastPy = y;lastSclFac = SclFac;}
		break;
	default:
		break;
	}
}

void DrawModel(){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	glNormalPointer(GL_FLOAT,0,normals);
	glVertexPointer(3,GL_FLOAT,0,vertices);
	glDrawElements(GL_TRIANGLES,nindices,GL_UNSIGNED_INT,indices);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

GLint GenList()
{
	GLint lid=glGenLists(1); 
	glNewList(lid, GL_COMPILE);
	DrawModel();
	glEndList();
	return lid;
}

void motion(int x, int y)
{
	if (bLButtonDown) {
		yRotation -= (GLfloat) (lastPx - x) / rotSpeed;
		xRotation -= (GLfloat) (lastPy - y) / rotSpeed;
	} 
	else if (bRButtonDown) {
		GLfloat dy = y - lastPy;
		if (dy > 0.0f) {
			dy = sqrt(dy);
			SclFac = lastSclFac * dy;
		} 
		else if (dy < 0.0f) {
			dy = sqrt(-dy);
			SclFac = lastSclFac / dy;
		}
	} 
	else if (bMButtonDown) {
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		GLfloat halfHeight = viewport[3] >> 1;
		GLfloat reci = 1.0 / SclFac;
		xTransl += (GLfloat) (x - lastPx) * reci / halfHeight;
		yTransl -= (GLfloat) (y - lastPy) * reci / halfHeight;
		lastPx = x;
		lastPy = y;
	}
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	glScalef(SclFac,SclFac,SclFac);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
	lightSpin += lightSpeed;
	glDisable(GL_LIGHTING);
	glTranslatef(xTransl,yTransl,0.0f);
	glRotatef(xRotation, 1.0f, 0.0f, 0.0f);
	glRotatef(yRotation, 0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glRotatef(lightSpin, 0.0f, 1.0f, 0.0f);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
	lightSpin += lightSpeed;
	glDisable(GL_LIGHTING);
	glTranslatef(LightPos[0], LightPos[1], LightPos[2]);
	glutSolidSphere(0.05*100, 10, 10);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	
	//DrawModel();
	glCallList(ArmadilloList);

	glPopMatrix();
	glutSwapBuffers();
	
	glFlush();
}

void reshape(int w, int h)
{
	float a,b,c;
	a = 100;
	b = 100;
	c = 100;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-a, a, -b * (GLfloat) h / (GLfloat) w,
		b * (GLfloat) h / (GLfloat) w, -c, c);
	else
		glOrtho(-a * (GLfloat) w / (GLfloat) h,
		a * (GLfloat) w / (GLfloat) h, -b, b, -c, c);
	glMatrixMode(GL_MODELVIEW);
	display(); 
}

void destroy(){	
	if(indices!=NULL){delete[] indices;}
	if(vertices!=NULL){delete[] vertices;}
	if(normals!=NULL){delete[] normals;}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(width,height);
	glutCreateWindow(argv[0]);
	init();
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);
	glutDisplayFunc(display);
	glutIdleFunc(display);

	ArmadilloList = GenList();

	glutMainLoop();
	destroy();
	return 0;
}