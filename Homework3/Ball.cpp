# include <stdlib.h>
# include <math.h>
# include <GL/glut.h>

typedef float point[3];
static GLfloat theta[] = {0.0,0.0,0.0};
int n, mode;

static GLdouble spin; 

point v[]={	{0.0,		0.0,		1.0}, 
			{0.0,		0.942809,	-0.333333},
			{-0.816497, -0.471405,	-0.333333}, 
			{0.816497,	-0.471405,	-0.333333}};

void triangle( point a, point b, point c)
{
	if(mode==0)
	{
		glBegin(GL_LINE_LOOP);
		glVertex3fv(a);
		glVertex3fv(b);
		glVertex3fv(c);
	}
	else if(mode==1)
	{
		glBegin(GL_POLYGON);
		glNormal3fv(a);
		glVertex3fv(a);
		glVertex3fv(b);
		glVertex3fv(c);
	}
	else if(mode==2)
	{
		glBegin(GL_POLYGON);
		glNormal3fv(a);
		glVertex3fv(a);
		glNormal3fv(b);
		glVertex3fv(b);
		glNormal3fv(c);
		glVertex3fv(c);
	}
	glEnd();
}

void normal(point p)
{ 
	float d = 0.0;
	int i;
	for(i=0; i<3; i++) d+=p[i]*p[i];
	d=sqrt(d);
	if(d>0.0) for(i=0; i<3; i++) p[i]/=d;
}

void divide_triangle(point a, point b, point c, int m)
{
	point v1, v2, v3;
	if(m > 0)
	{
		for(int j = 0; j < 3; j++)
		{
			v1[j] = a[j] + b[j];
			v2[j] = a[j] + c[j];
			v3[j] = b[j] + c[j];
		}
		normal(v2);
		normal(v1);
		normal(v3);

		divide_triangle(a, v1, v2, m-1);
		divide_triangle(c, v2, v3, m-1);
		divide_triangle(b, v3, v1, m-1);
		divide_triangle(v1, v3, v2, m-1);
	}
	else(triangle(a,b,c));
}

void tetrahedron(int m)
{
	divide_triangle(v[0], v[1], v[2], m);
	divide_triangle(v[3], v[2], v[1], m);
	divide_triangle(v[0], v[3], v[1], m);
	divide_triangle(v[0], v[2], v[3], m);
}

void display(void)
{
	GLfloat light_pos[]={0.0,0.0,1.5,1.0}; 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(0.0,0.0,-5.0);

	glPushMatrix();
	glRotatef(spin,0.0,1.0,0.0);
	glLightfv(GL_LIGHT0 , GL_POSITION, light_pos);
	glTranslated(0.0,0.0,1.5);

	glDisable(GL_LIGHTING);
	glColor3f(0.0,1.0,1.0);
	glutWireCube(0.1);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	
	mode=0;
	tetrahedron(n);
/*
	mode=1;
	glTranslatef(-3.0, 0.0,0.0);
	tetrahedron(n);
	mode=2;
	glTranslatef(6.0, 0.0,0.0);
	tetrahedron(n);
*/
	glPopMatrix();
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-4.0, 4.0, -4.0 * (GLfloat) h / (GLfloat) w,
		4.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
	else
		glOrtho(-4.0 * (GLfloat) w / (GLfloat) h,
		4.0 * (GLfloat) w / (GLfloat) h, -4.0, 4.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	display(); 
}

void init()
{
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

	glShadeModel(GL_SMOOTH); /* enable smooth shading */
	glEnable(GL_LIGHTING); /* enable lighting */
	glEnable(GL_LIGHT0);  /* enable light 0 */
	glEnable(GL_DEPTH_TEST); /* enable z buffer */

	glClearColor (0.0, 0.0, 0.0, 0.0);
	glColor3f (0.0, 0.0, 0.0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 033:
		exit(EXIT_SUCCESS);
		break;
	case 'w':
	case 'W':
		spin += 5.0f;
		glutPostRedisplay();
	}
}

void main(int argc, char **argv)
{
	n = 0;

	glutInit(&argc, argv); 

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(500, 500);

	glutCreateWindow("sphere");

	init();

	glutReshapeFunc(reshape);

	glutDisplayFunc(display);

	glutKeyboardFunc(keyboard);

	glutMainLoop();
}