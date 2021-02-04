#include<windows.h>
#include<GL/glut.h>

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
/*
	Maeda Hanafi
	CSC431  Homework 1 Problem 2
*/

void init(void){
	glClearColor(1.0, 1.0, 1.0, 0.0);//set color wndow to white
	glMatrixMode(GL_PROJECTION); //set projection parm
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

void drawInitials(void){ //problem 2
	glClear(GL_COLOR_BUFFER_BIT);//clear display

	glColor3f(1.0, 0.0, 0.0); //set line segment color to red
	glBegin(GL_LINE_STRIP);
		glVertex2i(40,70);//draw an M
		glVertex2i(40,90);
		glVertex2i(55,80);
		glVertex2i(70,90);
		glVertex2i(70,70);		
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex2i(80,70);//draw an F
		glVertex2i(80,80);
		glVertex2i(95,80);
		glVertex2i(80,80);
		glVertex2i(80,90);	
		glVertex2i(110,90);	
	glEnd();
	glBegin(GL_LINES);
		glVertex2i(120,90);//draw an H
		glVertex2i(120,70);

		glVertex2i(150,90);
		glVertex2i(150,70);

		glVertex2i(120,80);	
		glVertex2i(150,80);	
	glEnd();

	glFlush();//process all opengl routines as quickly as possible
}

void main(int argc, char** argv){
	glutInit(&argc, argv); //int glut
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //set disp mode
	glutInitWindowPosition(100,200); //set top-left display pos
	glutInitWindowSize(400, 400);//set display-window width and height
	glutCreateWindow("OpenGL!!"); //Create window

	init();
	glutDisplayFunc(drawInitials);//send hwk problem 2 graphics to window display
	glutMainLoop();//display everything

}