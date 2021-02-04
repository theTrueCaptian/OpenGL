#include<windows.h>
#include<GL/glut.h>

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
/*
	Maeda Hanafi
	CSC431  Homework 2 Problem 2
*/

void init(void){
	glClearColor(1.0, 1.0, 1.0, 0.0);//set color wndow to white
	glMatrixMode(GL_PROJECTION); //set projection parm
	gluOrtho2D(0.0, 200.0, 0.0, 150.0);
}


class screenPt{
private: GLint x, y;
public:
	screenPt(){
		x=y=0;
	}
	void setCoords(GLint xCoordValue, GLint yCoordValue){
			x=xCoordValue;
			y=yCoordValue;
	}
	GLint getx() const{
		return x;
	}
	GLint gety() const{
		return y;
	}
	void incrementx(){
		x++;
	}
	void decrementy(){
		y++;
	}
};
void setPixel(GLint xCoord, GLint yCoord){
	glBegin(GL_POINTS);
		glVertex2i(xCoord, yCoord);
	glEnd();
}

void circleMidpoint(GLint xc, GLint yc, GLint radius){
	screenPt circPt;
	GLint p=1-radius; //initla value for midpoint parameter
	circPt.setCoords(0, radius); //set corrds for top point of circle
	void circlePlotPoints(GLint, GLint, screenPt);
	//plot the init point inn each circle quadrant
	circlePlotPoints(xc, yc, circPt);
	//calcuate next point and plot in each octant
	while(circPt.getx()<circPt.gety()){
		circPt.incrementx();
		if(p<0)
			p+= 2* circPt.getx()+1;
		else{
			circPt.decrementy();
			p+=2*(circPt.getx()*circPt.gety())+1;
		}
		circlePlotPoints(xc, yc, circPt);
	}
}

void drawInitials(void){ //problem 2
	glClear(GL_COLOR_BUFFER_BIT);//clear display

	glColor3f(1.0, 0.0, 0.0); //set line segment color to red
	//GLint x=100, y=100, r=50;
	//circleMidpoint(x, y, r);
	float t=0, r=50, x=100, y=100;
	glBegin(GL_LINES);	//draw my face!
		for(t=0; t<90; t=t+5 ){//draw points only in the first quadrant
			float xc=r*cos(t);
			float yc=r*sin(t);
			if(xc>0 && yc>0){
				glVertex3f(xc+x, yc+y, 0.0);
			}else if(xc<0 && yc>0){
				xc=-xc;
				glVertex3f(xc+x, yc+y, 0.0);
			}else if(xc<0 && yc<0){
				xc=-xc; yc=-yc;
				glVertex3f(xc+x, yc+y, 0.0);
			}else if(xc>0 && yc<0){
				yc=-yc;
				glVertex3f(xc+x, yc+y, 0.0);
			}
		}
	glEnd();
	

	glFlush();//process all opengl routines as quickly as possible
	
}
void circlePlotPoints(GLint xc, GLint yc, screenPt circPt){
	setPixel(xc+circPt.getx(), yc+circPt.gety());
	setPixel(xc-circPt.getx(), yc+circPt.gety());
	setPixel(xc+circPt.getx(), yc-circPt.gety());
	setPixel(xc-circPt.getx(), yc-circPt.gety());
	setPixel(xc+circPt.gety(), yc+circPt.getx());
	setPixel(xc-circPt.gety(), yc+circPt.getx());
	setPixel(xc+circPt.gety(), yc-circPt.getx());
	setPixel(xc-circPt.gety(), yc-circPt.getx());

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