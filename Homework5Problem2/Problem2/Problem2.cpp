///////////////////////////////////////////////////////////////////////////////////      
// throwBall.cpp
//
// This program shows the motion of a ball subject to gravity. The gravitational
// acceleration and initial velocity of the ball are changeable.
//
// Interaction:
// Press space to toggle between animation on and off.
// Press right/left arrow kes to increase/decrease the initial horizontal velocity.
// Press up/down arrow keys to increase/decrease the initial vertical velocity.
// Press page up/down keys to increase/decrease gravitational acceleration.
// Press r to reset."
//
// Sumanta Guha.
///////////////////////////////////////////////////////////////////////////////////
/*
	Maeda Hanafi
	CSC431 Homework 5 Problem 2
	Ball Bounces
*/
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define PI 3.14159265
#define e  2.71828182

using namespace std;

// Globals.
static int isAnimate = 0; // Animated?
static int animationPeriod = 100; // Time interval between frames.
static float t = 0.0; // Time parameter.

//Variables the user specifies
static float initAmp = 100.0; // initial Amplitude
static float angularFrequency = PI/4;// w is the angular frequency
static float thetaPhase = 0;// theta is the phase angle
static float dampingCont = 0.1;// k is the damping constant

static float sphereInitX=-15.0, sphereInitY=-15.0, sphereInitZ=-100.0;//Initial coordinates of the sphere
static float sphereX=0.0, sphereY=initAmp, sphereZ=0.0;//Shifts of the sphere 
static float sphereCurrY = sphereInitY;//Y Coordinates of the sphere

static char theStringBuffer[10]; // String buffer.
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

// Routine to onvert floating point to char string.
void floatToString(char * destStr, int precision, float val) 
{
   sprintf(destStr,"%f",val);
   destStr[precision] = '\0';
}

// Write data.
void writeData(void)
{
   glColor3f(0.0, 0.0, 0.0);
   
   floatToString(theStringBuffer, 4, initAmp);
   glRasterPos3f(-4.5, 4.5, -5.1);
   writeBitmapString((void*)font, "Initial Amplitude: ");
   writeBitmapString((void*)font, theStringBuffer);
   
   floatToString(theStringBuffer, 4, angularFrequency);
   glRasterPos3f(-4.5, 4.2, -5.1);
   writeBitmapString((void*)font, "Angular Frequency: ");  
   writeBitmapString((void*)font, theStringBuffer);

   floatToString(theStringBuffer, 4, thetaPhase);
   glRasterPos3f(-4.5, 3.9, -5.1);
   writeBitmapString((void*)font, "Angular Phase: ");  
   writeBitmapString((void*)font, theStringBuffer);

   floatToString(theStringBuffer, 4, dampingCont);
   glRasterPos3f(-4.5, 3.6, -5.1);
   writeBitmapString((void*)font, "Damping Constant: ");  
   writeBitmapString((void*)font, theStringBuffer);
}

//Calculates the y of the bouncing ball given the parameters
// A = initial Amplitude
// w is the angular frequency
// theta is the phase angle
// k is the damping constant
int getBallY(double A, double x, double w, double theta, double k){
	return A * abs(sin((w*x)+theta)) * pow(e, -k*x);
}

// Drawing routine.
void drawScene(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glLoadIdentity();

   writeData();
   
   // Place scene in frustum.
   glTranslatef(sphereInitX,sphereInitY,sphereInitZ);
  
   // Apply equations of motion to transform sphere.
   glTranslatef(sphereX,sphereY, sphereZ);
   sphereCurrY = sphereY+sphereInitY;
 
   // Sphere.
   glColor3f(0.0, 0.0, 1.0);
   glutWireSphere(2.0, 10, 10);

   //Throws out the current matrix (pop out of stack)
   glPopMatrix();
   //Duplicates current matrix and this new one becomes active (push into stack)
   glPushMatrix();

   glutSwapBuffers();
}

// Timer function.
void animate(int value)
{
   //if(sphereCurrY<0 && t>3){
	//		isAnimate = 0;
   //}
   if (isAnimate) 
   {
	   t += 1.0;
	   sphereX=t;
	   sphereY=getBallY(initAmp, sphereX, angularFrequency, thetaPhase, dampingCont);
	   if(sphereY<0) sphereY=0;
	   sphereZ=0.0;
	   cout << "Next Coor:" << sphereX<< " " << sphereY << " "<< sphereZ <<endl;

	   //stop animation if the y is 0
	   //if(sphereY==0) isAnimate=0;
   }
   glutTimerFunc(animationPeriod, animate, 1);
   glutPostRedisplay();
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
         if (isAnimate) isAnimate = 0;
		 else isAnimate = 1;
         glutPostRedisplay();
		 break;
	  case 'r':
         isAnimate = 0;
		 t = 0.0;
         glutPostRedisplay();
		 break;
      default:
         break;
   }
}


// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if(key == GLUT_KEY_UP) initAmp+=0.5;//v += 0.05;
   if(key == GLUT_KEY_DOWN) initAmp-=0.5;//if (v > 0.1) v -= 0.05;
   if(key == GLUT_KEY_RIGHT) angularFrequency+=PI/32;
   if(key == GLUT_KEY_LEFT) angularFrequency-=PI/32;
   if(key == GLUT_KEY_PAGE_UP) thetaPhase+=PI/32; 
   if(key == GLUT_KEY_PAGE_DOWN) thetaPhase-=PI/32;
   if(key == GLUT_KEY_HOME) dampingCont+=0.1;  
   if(key == GLUT_KEY_END) dampingCont-=0.1;

   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between animation on and off." << endl
        << "Press right/left arrow kes to increase/decrease the angular frequency." << endl
        << "Press up/down arrow keys to increase/decrease the initial Amplitude." << endl
        << "Press page up/down keys to increase/decrease phase angle." << endl
		<< "Press home or end keys to increase/decrease damping constant." << endl
        << "Press r to reset." << endl;
}

// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow ("throwBall.cpp"); 
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutTimerFunc(5, animate, 1);
   glutSpecialFunc(specialKeyInput);
   glutMainLoop(); 

   return 0;  
}

