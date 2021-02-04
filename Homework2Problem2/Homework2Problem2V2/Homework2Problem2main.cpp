#include<windows.h>
#include<GL/glut.h>

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define PI 3.14159265358979324
/*
	Maeda Hanafi
	CSC431  Homework 2 Problem 2
*/

void init(void){
	glClearColor(1.0, 1.0, 1.0, 0.0);//set color wndow to white
	glMatrixMode(GL_PROJECTION); //set projection parm
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}

//draw a circle
void drawCircle(float cx, float cy, float r, int num_segments) { 
	
	glBegin(GL_POLYGON); 
	for(int i=0; i<num_segments; i++) { 
		float theta=2.0f*3.1415926f*float(i)/float(num_segments);//get the current angle 
		float x=r*cosf(theta);//calculate the x component 
		float y=r*sinf(theta);//calculate the y component 
		glVertex2f(x+cx, y+cy);//output vertex 
	} 
	glEnd(); 
}

//this is for my smile :)
void drawArc(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments) { 
	float theta = arc_angle / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open
	float tangetial_factor = tanf(theta);
	float radial_factor = cosf(theta);
	
	float x = r * cosf(start_angle);//we now start at the start angle
	float y = r * sinf(start_angle); 
    
	glBegin(GL_LINE_STRIP);//since the arc is not a closed curve, this is a strip now
	for(int ii = 0; ii < num_segments; ii++)	{ 
		glVertex2f(x + cx, y + cy);
		float tx = -y; 
		float ty = x; 
		x += tx * tangetial_factor; 
		y += ty * tangetial_factor; 
		x *= radial_factor; 
		y *= radial_factor; 
	} 
	glEnd(); 
}
//draw my  headscarf
void drawHeadscarf(){
	
	//draw the piece over the neck
	glColor4f(0.5, 0.5, 1.0, 1.0); //set RGBA color to the color of headscarf
	glBegin(GL_POLYGON);
		glVertex2f(100.0, 100.0);
		glVertex2f(20.0, 20.0);
		glVertex2f(180.0, 20.0);
	glEnd();
		
	glPolygonMode( GL_FRONT, GL_FILL); //set color my scarf
	glColor4f(0.5, 0.5, 1.0, 1.0); //setRGBA  color to the color of headscarf
	drawCircle(100.0, 100.0, 70.0, 200); //draw the piece around my head

	//draw the stiches of the scarf
	glColor4f(0.0, 0.0, 0.0, 1.0); //set RGBA color to the color of stitch
	glLineWidth(4.0); //line attribute
	glEnable(GL_LINE_STIPPLE);//enable  line stipple
	glLineStipple(1, 0x04);
	glBegin(GL_LINES);
		glVertex2f(100.0, 50.0);
		glVertex2f(100.0, 20.0);
	glEnd();
	glDisable(GL_LINE_STIPPLE); //disable stipple afterwards
	glLineWidth(1.0);


	//draw pin with pattern
	GLubyte pattern[] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
							0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
							0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
							0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
							0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
							0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
							0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
							0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,	
							0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
							0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
							0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
							0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
							0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
							0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
							0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00,
							0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
							
	};
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glEnable(GL_POLYGON_STIPPLE); //enable polygon stipple
		glPolygonStipple(pattern);
		glPolygonMode( GL_FRONT, GL_FILL); //set color my pin
		glColor4f(0.8, 0.0, 0.8, 1.0); //setRGBA  color to the color of headscarf
		glRecti(40, 140, 48, 130); //draw the pin
	glDisable(GL_POLYGON_STIPPLE);//disable when done
	
}

void drawShirt(){
	//draw shirt with color interpolation
	glShadeModel(GL_SMOOTH);//this is the default anyways
	glColor4f(0.5, 0.0, 0.8, 1.0); //set RGBA color to the color of shirt
	glPolygonMode( GL_FRONT, GL_FILL); //set color my shirt	
	//glRecti(50, 20, 150, 0);
	glBegin(GL_POLYGON);
		glColor4f(0.0, 0.0, 0.8, 1.0); //set RGBA color to the color of shirt
		glVertex2i(50, 20);
		glColor4f(0.5, 0.0, 0.8, 1.0); //set RGBA color to the color of shirt	
		glVertex2i(50, 0);
		glVertex2i(150, 0);
		glColor4f(0.0, 0.0, 0.8, 1.0);
		glVertex2i(150, 20);
	glEnd();
}

void drawFace(void){ //problem 2
	glClear(GL_COLOR_BUFFER_BIT);//clear display
	//draw my shirt
	drawShirt();

	//draw my head scarf
	drawHeadscarf();

	glColor3ub(255,204,102);
	glPolygonMode( GL_FRONT, GL_FILL); //set color my face
	drawCircle(100.0, 100.0, 50.0, 200); //fill my face

	glColor3f(0.0, 0.0, 0.0); //set line segment color to black
	glPolygonMode( GL_FRONT, GL_LINE);
	drawCircle(100.0, 100.0, 50.0, 200); //draw my face

	glColor3f(1.0, 1.0, 1.0); //set line segment color to white
	glPolygonMode( GL_FRONT, GL_FILL);
	drawCircle(120.0, 120.0, 5.0, 30); //draw my eyes
	drawCircle(80.0, 120.0, 5.0,  30);
	glColor3f(0.0, 0.0, 0.0); //set line segment color to black
	drawCircle(120.0, 120.0, 2.5, 30); //draw my eyes
	drawCircle(80.0, 120.0, 2.5,  30);

	glColor3f(0.0, 0.0, 0.0); //set line segment color to black
	drawArc(100.0, 90.0, 25.0, 3.10, 3.4, 30);//draw my smile!
	
	glFlush();//process all opengl routines as quickly as possible
	
}


void main(int argc, char** argv){
	glutInit(&argc, argv); //int glut
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //set disp mode
	glutInitWindowPosition(100,200); //set top-left display pos
	glutInitWindowSize(500, 500);//set display-window width and height
	glutCreateWindow("OpenGL!!"); //Create window

	init();
	glutDisplayFunc(drawFace);//send hwk problem 2 graphics to window display
	glutMainLoop();//display everything

}