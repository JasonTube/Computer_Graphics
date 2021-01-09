# include <stdlib.h>
# include <stdio.h>
# include <tchar.h>
# include <gl/glaux.h>
# include <gl/glut.h>
# pragma comment(lib, "glaux.lib")

GLuint		    texture[1];	
GLUquadricObj	*q;	
AUX_RGBImageRec *TextureImage[1];	
GLfloat	xrot=0.1;				// Angle For The Triangle 

AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
{
	FILE *File=NULL;									// File Handle
								
	if (!Filename)										// Make Sure A Filename Was Given
	{
		return NULL;									// If Not Return NULL
	}

	File=fopen(Filename,"r");							// Check To See If The File Exists

	if (File)											// Does The File Exist?
	{
		fclose(File);									// Close The Handle
		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
	}

	return NULL;										// If Load Failed Return NULL
}

void init ( void )
 {
    GLfloat mat_specular [ ] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_diffuse [ ] = { 0.0, 2.0, 2.0, 0.1};
    GLfloat mat_shininess [ ] = { 100.0 };

 
    glClearColor ( 0.0, 0.0, 0.0, 1.0 );
    glShadeModel ( GL_SMOOTH );

/*******************use the quadratic to creat the object**************************************/
    q = gluNewQuadric();								  // Create A New Quadratic

    gluQuadricNormals(q, GL_SMOOTH);					  // Generate Smooth Normals For The Quad
    gluQuadricTexture(q, GL_TRUE);						  // Enable Texture Coords For The Quad

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// Set Up Sphere Mapping

/*******************Init Texture**************************************/


    memset(TextureImage,0,sizeof(void *)*1);			  // Set The Pointer To NULL

    if (TextureImage[0]=LoadBMP("DATA/earth.bmp")) 		
    { 							
      glGenTextures(1, &texture[0]);					  // Create The Texture
					
      glBindTexture(GL_TEXTURE_2D, texture[0]);
	  glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
	  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	  if (TextureImage[0])						// If Texture Exists
      {
        if (TextureImage[0]->data)			// If Texture Image Exists
        {
          free(TextureImage[0]->data);		// Free The Texture Image Memory
        }

        free(TextureImage[0]);				   // Free The Image Structure 
      }
    }
	glEnable (GL_TEXTURE_2D);   
    glEnable (GL_DEPTH_TEST);
	glRotatef(-70,1.0,0.0,0.0);
}

 void display ( void )
 {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glRotatef(xrot,0.0f,0.0f,1.0f);	
	gluSphere(q, 1.3f, 100, 100);

	glRasterPos2f(0.0, 0.0);
	glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');

	glFlush ();
 }

void update(int value)
{
  xrot = 0.1f;      
  value = 25;
  glutPostRedisplay(); 
  glutTimerFunc(value, update, 0);
}

void reshape (int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ( );
	
    if (w <= h) 
        glOrtho (-1.5, 1.5, -1.5 * ( GLfloat ) h / ( GLfloat ) w, 1.5 * ( GLfloat ) h / ( GLfloat ) w, -10.0, 10.0 );
    else
        glOrtho (-1.5 * ( GLfloat ) w / ( GLfloat ) h, 1.5 * ( GLfloat ) w / ( GLfloat ) h, -1.5, 1.5, -10.0, 10.0);

    glMatrixMode ( GL_MODELVIEW ); 
	glLoadIdentity ( ) ;
	glRotatef(-70,1.0,0.0,0.0);
}

void keyboard ( unsigned char key, int x, int y)
{
     
    switch (key)  
    {   case 'r':
		case 'R':
			xrot=0.2;
			glutPostRedisplay();
			break;
        case 27: //'ESC'
        exit (0); 
        break;
    }
}

int main(int argc, char** argv)
{

    glutInit (&argc, argv);

    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize (400, 400);

    glutInitWindowPosition (100, 100);

    glutCreateWindow ( argv [ 0 ] );

    init ( );

    glutDisplayFunc ( display );

    glutReshapeFunc ( reshape );

    glutKeyboardFunc ( keyboard );

	glutTimerFunc(25, update, 0); //Add a timer

    glutMainLoop( );

	return 0;
}
 