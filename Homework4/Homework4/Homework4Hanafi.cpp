/*
 * Maeda Hanafi
 * CSC431 CG Homework 4
 *
 */


#include <cstdlib>
#include <vector>
#include <iostream>
#include <math.h>
#include <stdlib.h>

#ifdef __APPLE__
# include <GLUT/glut.h>
#elseif __linux__
# include <GL/glut.h>
#else
//# include "stdafx.h"
# include <GL\glut.h>
#endif

// Use the STL extension of C++.
using namespace std;

/*  Set initial display-window size. */
GLsizei winWidth = 600, winHeight = 600;  

/*  Set range for world coordinates.  */
GLfloat xwcMin = 0.0, xwcMax = 225.0;
GLfloat ywcMin = 0.0, ywcMax = 225.0;

// Translating, rotation, scaling increments
GLfloat tx = 0.0, ty = 0.0;
GLdouble theta = 0.0; 
GLfloat sx = 1.0, sy = 1.0;

class wcPt2D {
  public:
		GLfloat x, y;
};

typedef GLfloat Matrix3x3 [3][3];

Matrix3x3 matComposite;

const GLdouble pi = 3.14159;

// Vector of points (for pentagons).
wcPt2D points[5];
int nPoints = 0;


void init (void)
{
   /*  Set color of display window to white.  */
   glClearColor (1.0, 1.0, 1.0, 0.0);

   // Clear the display
   //glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/* Construct the 3 by 3 identity matrix. */
void matrix3x3SetIdentity (Matrix3x3 matIdent3x3)
{
   GLint row, col;

   for (row = 0; row < 3; row++)
      for (col = 0; col < 3; col++)
         matIdent3x3 [row][col] = (row == col);
}

/* Premultiply matrix m1 times matrix m2, store result in m2. */
void matrix3x3PreMultiply (Matrix3x3 m1, Matrix3x3 m2)
{
   GLint row, col;
   Matrix3x3 matTemp;

   for (row = 0; row < 3; row++)
      for (col = 0; col < 3 ; col++)
         matTemp [row][col] = m1 [row][0] * m2 [0][col] + m1 [row][1] *
                            m2 [1][col] + m1 [row][2] * m2 [2][col];

   for (row = 0; row < 3; row++)
      for (col = 0; col < 3; col++)
         m2 [row][col] = matTemp [row][col];
}

void translate2D (GLfloat tx, GLfloat ty)
{
   Matrix3x3 matTransl;

   /*  Initialize translation matrix to identity.  */
   matrix3x3SetIdentity (matTransl);  

   matTransl [0][2] = tx;
   matTransl [1][2] = ty;

   /*  Concatenate matTransl with the composite matrix.  */
   matrix3x3PreMultiply (matTransl, matComposite);

  
}

void rotate2D (wcPt2D pivotPt, GLfloat theta)
{
   Matrix3x3 matRot;
   
   /*  Initialize rotation matrix to identity.  */
   matrix3x3SetIdentity (matRot);    

   matRot [0][0] = cos (theta);
   matRot [0][1] = -sin (theta);
   matRot [0][2] = pivotPt.x * (1 - cos (theta)) + 
                        pivotPt.y * sin (theta);
   matRot [1][0] = sin (theta);
   matRot [1][1] = cos (theta);
   matRot [1][2] = pivotPt.y * (1 - cos (theta)) - 
                        pivotPt.x * sin (theta);

   /*  Concatenate matRot with the composite matrix.  */
   matrix3x3PreMultiply (matRot, matComposite);

   
}

void scale2D (GLfloat sx, GLfloat sy, wcPt2D fixedPt)
{
   Matrix3x3 matScale;

   /*  Initialize scaling matrix to identity.  */
   matrix3x3SetIdentity (matScale);    

   matScale [0][0] = sx;
   matScale [0][2] = (1 - sx) * fixedPt.x;
   matScale [1][1] = sy;
   matScale [1][2] = (1 - sy) * fixedPt.y;

   /*  Concatenate matScale with the composite matrix.  */
   matrix3x3PreMultiply (matScale, matComposite);
}

/* Using the composite matrix, calculate transformed coordinates. */
void transformVerts2D (GLint nVerts, wcPt2D * verts)
{
   GLint k;
   GLfloat temp;

   for (k = 0; k < nVerts; k++) {
      temp = matComposite [0][0] * verts [k].x + matComposite [0][1] * verts [k].y + matComposite [0][2];
      verts [k].y =(matComposite [1][0] * verts [k].x + matComposite [1][1] * verts [k].y + matComposite [1][2]);
         verts [k].x = (temp);
   }
}

void pentagon (wcPt2D *verts)
{
    glBegin(GL_POLYGON);

	cout << "DRAW "<<endl;
	if(nPoints==5)
		for(int i=0; i<5; i++){
			glVertex2f(verts[i].x, verts[i].y); 
		}
	glEnd();
}

void displayFcn (void)
{
    //  Clear display window.
	init();
	if(nPoints == 5){
		GLint nVerts = 5;
	  
	   //  Set geometric transformation parameters.  
	   wcPt2D pivPt;
	   pivPt.x = 0.0, pivPt.y = 0.0;

	   //  Initialize composite matrix to identity.  
	   matrix3x3SetIdentity (matComposite);

	   //  Construct composite matrix for transformation sequence.  
	   scale2D (sx, sy, pivPt);   //  First transformation: Scale.
	   rotate2D (pivPt, theta);     //  Second transformation: Rotate 
	   translate2D (tx, ty);        //  Final transformation: Translate.
	   
	   //  Apply composite matrix to pentagon vertices.  
	   transformVerts2D (nVerts, points);

	   glColor3f (1.0, 0.0, 0.0);  // Set color for transformed pentagon.
	   pentagon (points);           // Display transformed pentagon.
	   
	   // Reset the increments
	   tx=0.0, ty=0.0;
	   sx = 1.0, sy = 1.0;
	   theta = 0.0;
	}
	glFlush ( );
}

void winReshapeFcn (GLint newWidth, GLint newHeight)
{
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ( );
   //gluOrtho2D (xwcMin, xwcMax, ywcMin, ywcMax);

   // Set viewing box dimensions equal to window dimensions.
   glOrtho(0.0, (float)winWidth, 0.0, (float)winHeight, -1.0, 1.0);

   
   glClear (GL_COLOR_BUFFER_BIT);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y){
	switch (key) {
      case 27:
         exit(0);
         break;
	  case 'x': // Translate pentagon 5 pixels in negative direction  
		  tx = -5.0;
		  break;
	  case 'X': // Translate 5 pixel to the postive
		  tx = 5.0;
		  break;
	  case 'y': // Translate pentagon in negative y direction
		  ty = -5.0;
		  break;
	  case 'Y': // Translate pentagon in positive y direction
		  ty = 5.0;
		  break;
	  case 'r': // Rotate pentagon in CCW by 5 degrees; Rotation point is origin
		  theta = -(5*pi)/180;
		  break;
	  case 'R': // Rotate pentagon in CW by 5 degrees
		  theta = (5*pi)/180;
		  break;
	  case '+': // Scale pentagon 110% of current size
		  sx = 1.1, sy = 1.1;
		  break;
	  case '-': // Scale pentagon 90% of current size
		  sx = 0.9, sy = 0.9;
		  break;
      default:
         break;
   }
   glutPostRedisplay();
}

void addPoint(GLfloat x, GLfloat y){
	points[nPoints].x = x;
	points[nPoints].y = y;
	cout << "Points added: "<< x << "  " << (winHeight - y) <<endl;
	nPoints++;
}

// Mouse callback routine.
void mouseControl(int button, int state, int x, int y){
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
	   // Store the pentagon in the points array after correcting from event to OpenGL co-ordinates.
	   addPoint(x, winHeight -y);

	   // If the number of points exceed the pentagon, then clear the points vector and add new point
	   if(nPoints>5){
		   cout << "Clear points array" <<endl;

		   nPoints = 0;
		   addPoint(x, winHeight -y);	   
	   }
   }
   if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 
	   exit(0);
   
   glutPostRedisplay();
}

int main (int argc, char ** argv)
{
   glutInit (&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowPosition (50, 50);
   glutInitWindowSize (winWidth, winHeight);
   glutCreateWindow ("Geometric Transformation Sequence");

   init ( );
   glutDisplayFunc (displayFcn);
   glutReshapeFunc (winReshapeFcn);
   // Register the keyboard
   glutKeyboardFunc(keyInput);
   // Register the mouse callback function. 
   glutMouseFunc(mouseControl); 

   cout << "Define the pentagon by clicking on screeen 5 times" <<endl;
		

   glutMainLoop ( );
}
