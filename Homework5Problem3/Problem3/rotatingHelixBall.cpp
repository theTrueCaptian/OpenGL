/////////////////////////////////////////////////////////////////        
// rotatingHelix1.cpp
//
// This program, based on helix.cpp, animates a helix by rotating
// it around its axis via keyboard presses.
//
// Interaction:
// Press space to turn the helix.
//
//  Sumanta Guha.
/////////////////////////////////////////////////////////////////
/*
	Maeda Hanafi
	CSC431 Computer Graphics
	Homework 5 Problem 3: Ball on Helix
*/
#include <cstdlib>
#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265

using namespace std;

// Globals.
static float angle = 0.0; // Angle of rotation.
static float tBall = 10 * PI ; // Angle of rotation.

// Drawing routine.
void drawScene(void)
{  
   float R = 20.0; // Radius of helix.

   float t; // Angle parameter along helix.

   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0);
   glPushMatrix();

   glBegin(GL_LINE_STRIP);
   for(t = -10 * PI; t <= 10 * PI; t += PI/20.0) 
      glVertex3f(R * cos(t), t, R * sin(t) - 60.0);
   glEnd();
   
   //Allow for transformations to be applied on another object
   glPopMatrix();
   glPushMatrix();
   glLoadIdentity();

   //position the ball on the correct coordinates
   glTranslatef(R * cos(tBall), tBall, R * sin(tBall) - 60.0);

   //Draw Shpere
   glColor3f(1.0, 0.0, 0.0);
   glutSolidSphere(2.0, 5, 5);

   //Allow for transformations to be applied on another object
   glPopMatrix();
   glPushMatrix();
   glLoadIdentity();

   glutSwapBuffers();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0); 
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Routine to increase the rotation angle.
void increaseAngle(void)
{
   angle += 5.0;
   if (angle > 360.0) angle -= 360.0;

   //Also increase the time
   tBall -= PI/20.0;
   if(tBall < -10 * PI ) tBall = 10 * PI ; 
	   
   glutPostRedisplay();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
         exit(0);
         break;
      case ' ': 
		 increaseAngle();
         break;
      default:
         break;
   }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to animate ball around the helix." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("Problem 3: Rotating Ball");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutMainLoop(); 

   return 0;  
}
