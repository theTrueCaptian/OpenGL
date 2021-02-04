//////////////////////////////////////////////////////////////////////          
// Maeda Hanafi
// Homework 3
//  
// Comments: One of the includes in the given makefile is giving me errors, so I commented that out.
//			
//			All lines are drawn having 4 pixels per point (2 width and 2 length), thus DDA, Bresenham, 
//			antialiasing implementation is adapted to it. For instance, instead of stepping every pixel, the algorithm would
//			step for every two pixels, since two pixels makes a point.
//			
//			Also, in the documentation, Point refers to a 2 by 2 pixel point and subpixels refers to the pixels that make up a Point
////////////////////////////////////////////////////////////////////// 

#include <cstdlib>
#include <vector>
#include <iostream>
#include <math.h>

#ifdef __APPLE__
# include <GLUT/glut.h>
#elseif __linux__
# include <GL/glut.h>
#else
//# include "stdafx.h" this include gives out errors
# include <GL\glut.h>
#endif

// Use the STL extension of C++.
using namespace std;

// Globals.
static GLsizei width, height; // OpenGL window size.

// Point class. _______________________________________________________________________________________________________
class Point{
public:
	Point(){}
	Point(int x, int y) {
		xVal = x; yVal = y;		
		r=0; b=0; g=0;
	}
	void drawPoint(void); // Function to draw a point.
	int getX(void);
	int getY(void);	
	void setRGB(int inR, int inB, int inG);
private:
   float xVal, yVal; // x and y co-ordinates of point.
   int r,g,b;  //rgb of the point; by default it is black
};

// Function to draw a point.
void Point::drawPoint(void){ 
	glPointSize(2);
	glColor3ub(r,g,b);
	glBegin(GL_POINTS);
		glVertex3f(xVal, yVal, 0.0);
	glEnd();   
}
// Function to get x
int Point::getX(void){  
   return xVal; 
}
// Function to get y
int Point::getY(void){  
   return yVal; 
}
//Function to set RGB of point
void Point::setRGB(int inR, int inB, int inG){
	r=inR; b=inB; g=inG;
}

// Line class. _______________________________________________________________________________________________________
class Line{
public:	
	Line(Point instart, Point inend, char inalgorithm) {
		start = instart; 
		end = inend; 
		algorithm = inalgorithm;
	}
	// Vector of points (all the points in the line).
	vector<Point> linePoints;
	// Vector of points (all the subpixels in the upper line boundary).
	vector<Point> upperLine;
	// Vector of points (all the subpixels in the lower line boundary).
	vector<Point> lowerLine;
	void drawLine(void);
	void addPoint(Point);
	//This function adds the subpixels into upper and lowerLine
	void addSubpixs(int botx, int boty);
	//Given subpixels, is it in the polygon line?
	bool inPolygonLine(int x, int y);
private:
   Point start, end; //endpoints on the line
   char algorithm; //This refers to the algorithm used on the line 
  
};
// Function to draw the line
void Line::drawLine(void){  
  vector<Point>::iterator ptIterator = linePoints.begin();
   while((ptIterator) != linePoints.end()){	
	  ptIterator->drawPoint();
	  ptIterator++;
   }
}
// Function to add a point to the line
void Line::addPoint(Point in){  
	linePoints.push_back(in);
}
//Method to add subpixels to the upper and lowerLine 
void Line::addSubpixs(int botx, int boty){
	lowerLine.push_back(Point(botx, boty-1));
	upperLine.push_back(Point(botx, boty+1));
	lowerLine.push_back(Point(botx+1, boty-1));
	upperLine.push_back(Point(botx+1, boty+1 ));
}

//Given subpixels, is it in the polygon line?
bool Line::inPolygonLine(int xs, int ys){
			
	//Iterate through all subpixels
	vector<Point>::iterator ptIterator = lowerLine.begin();
	vector<Point>::iterator upIterator = upperLine.begin();
	
	//Scan left to right
	while((ptIterator) != lowerLine.end() || (upIterator) != upperLine.end()){	
			
		if(xs==(*ptIterator).getX()){
			if(((*ptIterator).getY()<=(ys) && (ys)<=(*upIterator).getY()) )
				return true;
		}
		ptIterator++;
		upIterator++;
	}
	return false;
}
// Vector of points (only line endpoints). ______________________________________________________________________
vector<Point> points;// Iterator to traverse a Point array.
vector<Point>::iterator pointsIterator; 
// Vector of lines 
vector<Line> lines;
// Iterator to traverse a Line array.
vector<Line>::iterator linesIterator; 

//Holds information about which algorithm to apply. By default use dda
char algorithmState = 'd';
//Returns point size
int getPointSize(void){
	return 2;
}

//Method for drawing a line with bresenham
//If argument pointSize is false, then the steps are incremented by 1, else by pointsize. By default it should by true
//If argument calcBoundaries is true, then the subpixels of boundaries of the line are recorded.
Line bresenham(Point start, Point end, bool pointSize, bool calcBoundaries){
	//This is the Line we are going to store the calculated points in
	Line calcLine = Line(start, end, 'b'); 	
	//calculate initial descion point, po = 2 * deltay - deltax
	int deltaY = end.getY() - start.getY(), deltaX = end.getX() - start.getX();

	//number of steps
	int steps = deltaX;

	//calculate constants 
	int constD1 = 2*deltaY - 2*deltaX, constD2 = 2*deltaY;
	int po = constD2-deltaX;
		
	//add start point
	int drawX = start.getX(), drawY = start.getY();
	calcLine.addPoint(start);		
	calcLine.addSubpixs(drawX, drawY);
	cout << "Bresenham INIT POINT:" << drawX << "," << drawY <<endl;

	//Based on the passed argument, if the point size is considered, then increments is doubled
	int inc = 1;
	if(pointSize) inc = getPointSize();
	
	if(deltaY>=deltaX){//if the slope is greater than 1(deltaY greater than deltaX), then the roles x and y are interchanged
		int pk = po;		//pk is calculated as po (initally desicion point is based off po
		steps = deltaY;
		for(int i=1; i<=steps; i=i+inc){	//Step for every two pixels (because of point size) if needed
			//draw rest of the points (decision point is based on pk)	
			if(pk>0){//if pk>0, then draw (x k+1, y k+1)
				drawX = drawX + inc;
				drawY = drawY + inc;
				pk = pk + 2*deltaX - 2*deltaY; //calculate next pk, pk + 2*deltaX - 2*deltaY
			}else{//else draw (x , y k+1)
				drawX = drawX ; //if slope is greater than 1, then the y is incremented
				drawY = drawY+ inc;	
				pk = pk + 2*deltaX;//calculate next pk, pk + 2*deltaX
			}
			calcLine.addPoint(Point(drawX, drawY));
			if(calcBoundaries) { //add subpixel boundaries into line information (to be used in antialiasing)
				calcLine.addSubpixs(drawX, drawY);
			}
			cout << "Bresenahm POINT CALCULATED:" << drawX << "," << drawY <<endl;
		}	
	}else{
		int pk = po;		//pk is calculated as po (initally desicion point is based off po
		for(int i=1; i<=steps; i=i+inc){
			//draw rest of the points (decision point is based on pk)	
			if(pk>0){//if pk>0, then draw (x k+1, y k+1)
				drawX = drawX + inc;
				drawY = drawY + inc;
				pk = pk + constD1; //calculate next pk, pk + 2*deltaY - 2*deltaX 
			}else{//else draw (x k+1, y)
				drawX = drawX + inc; //if slope is less than 1, then the x is incremented
				drawY = drawY;	
				pk = pk + constD2;//calculate next pk, pk + 2*deltaY
			}			
			calcLine.addPoint(Point(drawX, drawY));
			if(calcBoundaries) { //add subpixel boundaries into line information (to be used in antialiasing)
				calcLine.addSubpixs(drawX, drawY);
			}
			cout << "Bresenahm POINT CALCULATED:" << drawX << "," << drawY <<endl;
		}	
	}
	return calcLine;
}
//Method for computing the slope
float getSlope(Point start, Point end){
	return ((float)(end.getY()-start.getY()))/((float)(end.getX()-start.getX()));
}

/*
 * This antialiasing function uses the supersampling method for lines of finite width. The pixels of the line
 * are chosen by Bresenham's algorithm. The line is defined by a polygon line, whose boundaries are upperLine and lowerLine, vectors
 * that hold subpixels of polygon boundaries.
 * If a subpixel's left lower corner is inside the line's polygon, then it is inside a line and contributes to the Point's intensity.
 * Each Point intensity is set proportional to the number of subpixels intersecting the line, which in this case is calculated based on
 * total number of subpixels encompassed by polygon line. 
 *
 * PseudoCode
 * Scan from right to left, scanX
 *		In each scan, scan down (1 Point below calcLine)  to up (1 Point above calcLine), scanY
 *			Given a Point(scanX, scanY), check if its subpixel intersects any part of the polygon
 *				Subpixel declarations: xs = scanX, ys = scanY
 *				The subpixels: (xs, ys), (xs+1, ys), (xs, ys+1), (xs+1, ys+1)
 *				if(isInPolygon(subpixel)
 *					add total
 *				Calculate new intensity
 *				Add a new Point(scanX, scanY, intensity)
 */
Line antialiasing(Point start, Point end){
	//Line to return
	Line finalLine = Line(start, end, 'a');

	//This is the Line we are going to store the calculated points in
	Line calcLine = Line(start, end, 'a'); 	
	float m = getSlope(start, end);
	//Define the polygon area that the line of width 2 encompasses, with two lines lowerLine and upperLine, by telling bresenham function to calculate 
	//subpixels of the line polygon boundary
	calcLine = bresenham(start, end, true, true);
	
	cout << "CHANGING INTENSITIES:" << start.getX() << "," << start.getY() <<endl;
	//Loop through the points of the actual line, as well as the upper and lower bounds and change the points intensity
	pointsIterator = calcLine.linePoints.begin();
	//Scan from right to left, scanX
	while((pointsIterator) != calcLine.linePoints.end()){ 
		int scanX = (*pointsIterator).getX();
		//In each scan, scan down (1 Point below calcLine)  to up (1 Point above calcLine), scanY
		for(int scanY=(*pointsIterator).getY()-(2*getPointSize()); scanY<(*pointsIterator).getY()+(2*getPointSize()); scanY=scanY+getPointSize()){
			//Given a Point(scanX, scanY), check if its subpixel intersects any part of the polygon
				
			//Subpixel declarations: xs = scanX, ys = scanY
			int xs = scanX, ys = scanY;
			int totalInside = 0;
			//The subpixels: (xs, ys), (xs+1, ys), (xs, ys+1), (xs+1, ys+1)
			//if(isInPolygon(subpixel)
			if(calcLine.inPolygonLine(xs, ys)) totalInside++;
			if(calcLine.inPolygonLine(xs, ys+1)) totalInside++;
			if(calcLine.inPolygonLine(xs+1, ys)) totalInside++;
			if(calcLine.inPolygonLine(xs+1, ys+1)) totalInside++;				
			//Calculate new intensity
			int newIntensity = (4-totalInside)*(255/4);
			//Add a new Point(scanX, scanY, intensity)
			Point newPoint = Point(scanX, scanY);
			newPoint.setRGB(newIntensity, newIntensity, newIntensity);
			cout << "New point" << newPoint.getX() << "," << newPoint.getY()<<" intensity: " << newIntensity << " totalintersect: "<<totalInside <<endl;
			    
			//Add the point into the finalLine
			finalLine.addPoint(newPoint);
		}
		pointsIterator++;
		
	}
   return finalLine;
}


//Method for rounding to nearest integer, used in dda
int round(float in){
	return floor(in+0.5);
}

//Method for drawing line using DDA
Line dda(Point start, Point end){
	//This is the Line we are going to store the calculated points in
	Line calcLine = Line(start, end, 'd'); 

	//compute slope
	float slope = getSlope(start, end);
	float drawX=start.getX(), drawY=start.getY();
	//steps holds number of sampling and it is decided based on slope
	float steps = 0;
	//these are constant when sampling, and initially calculated after deciding which axis to sample
	float incrementX=0, incrementY=0;
	//start.drawPoint();
	calcLine.addPoint(start);
	
	cout << "DDA INITAL POINT DRAWN:" << start.getX() << "," << start.getY() <<endl;

	if(slope<1){//if slope is < 1, then sample along x	
		//Calculate the number of steps/sampling along x axis. Notice the number of steps also takes account of the number of pixels per point
		steps = (int)((end.getX()-start.getX())/getPointSize());
	}else{//else if slope is > 1, then sample along y and calculate the number of steps/sampling along y axis
		steps = (int)((end.getY()-start.getY())/getPointSize());			
	}		
	cout << "DDA STEPS:" << steps <<endl;
	//calculate the increments for each axis
	incrementX = (end.getX()-start.getX())/steps;
	incrementY = (end.getY()-start.getY())/steps;	
	for(int i=0; i<steps; i++){
		drawX=drawX+incrementX;
		drawY=drawY+incrementY;
		calcLine.addPoint(Point(round(drawX), round(drawY)));
		cout << "DDA POINT CALCULATED:" << round(drawX) << "," << round(drawY) <<"  intervals:" <<incrementX << "," << incrementY <<endl;
	}
	return calcLine;
}

// Drawing routine.
void drawScene(void){
   glClear(GL_COLOR_BUFFER_BIT);
   glColor3f(0.0, 0.0, 0.0); 
   cout << "DRAWING ROUTINE*****************"<< endl; 
	  
   //loop through the lines array and draw it
   linesIterator = lines.begin();
   while((linesIterator) != lines.end()){	
	  linesIterator->drawLine();
	  linesIterator++;
   }
  
   glFlush();
}
//Method to decide if set of points should be discarded (if slope is negative)
bool discard(Point start, Point end){
	if(getSlope(start, end)>=0){ 
		return false;
	}else{ 
		cout << "DISCARD ENDPOINTS:" << start.getX() << "," << start.getY() << " "<< end.getX() << "," << end.getY() <<"______________"<<endl;
		return true;
	}
}
//Prints out the content of points array for debugging purposes
void printPointsContent(void){
	pointsIterator = points.begin();	
	cout << "List all:" <<endl;
	while(pointsIterator != points.end()){	   
		cout << "POINT:" << pointsIterator->getX() << "," << pointsIterator->getY() <<endl;
		pointsIterator++;
	}
}
void calculatePoints(bool recalcLastLine){
	

	//If a line (meaning two points exist) is defined, then either discard it or calculate it points  
	if(points.size()%2==0 && points.size()!=0){
		//Decide if we are recalculating a previous line, then remove that line from the Lines list
		if(recalcLastLine ){
			lines.erase(lines.end()-1);	
		}
		//iterator points at the latest, start-endpoint
		pointsIterator = points.end()-1; 	

		//Switch the points around, if the points are defined from right to left
		if(((*(pointsIterator-1)).getX()-(*(pointsIterator)).getX()) >0){
			std::swap(*(pointsIterator-1),*(pointsIterator));   
		}
		   
		if(discard( *(pointsIterator-1),*(pointsIterator) )){
			//erase both endpoints
			points.erase(pointsIterator-1);	
			points.erase(points.end()-1);					   
		}else{//If slope is acceptable, use algorithm to calculate the line's points
			if(algorithmState == 'd'){
				cout << "drawing points using dda." << "starting points:" << (pointsIterator-1)->getX() << ","<< (pointsIterator-1)->getY()<< endl; 
				//draw the line if the endpoints exist with dda
				lines.push_back( dda(*(pointsIterator-1),*(pointsIterator) ));
			}else if(algorithmState=='b' ){
				cout << "drawing points using bresenham." << "starting points:" << (pointsIterator-1)->getX() << ","<< (pointsIterator-1)->getY()<< endl; 
				//draw the line if the endpoints exist with bresenham
				lines.push_back( bresenham(*(pointsIterator-1),*(pointsIterator), true,false ));
			}else if(algorithmState=='a' ){
				cout << "drawing points using antialiasing." << "starting points:" << (pointsIterator-1)->getX() << ","<< (pointsIterator-1)->getY()<< endl; 
				//draw the line if the endpoints exist with antialiasing
				lines.push_back(antialiasing(*(pointsIterator-1),*(pointsIterator) ));
			}
		}
	}
	glutPostRedisplay();
}
// Mouse callback routine.
void mouseControl(int button, int state, int x, int y){
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
	   

	   // Store the endpoint in the points array after correcting from event to OpenGL co-ordinates.
	   //also store information about line algorithm
       points.push_back( Point(x, height - y) );
	   calculatePoints(false);
	   

	   //this is to print values in vector for debugging purposes
	   printPointsContent();
   }
   if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) 
	   exit(0);
   
   glutPostRedisplay();
}

// Initialization routine.
void setup(void){
   glClearColor(1.0, 1.0, 1.0, 0.0);  
}

// OpenGL window reshape routine.
void resize(int w, int h){
   glViewport (0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // Set viewing box dimensions equal to window dimensions.
   glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);

   // Pass the size of the OpenGL window to globals.
   width = w; 
   height = h; 

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}


// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y){
   switch (key) {
      case 27:
         exit(0);
         break;
	  case 'a': 
		  
		  algorithmState = 'a';
		  calculatePoints(true);
		  
		  break;
	  case 'b':
		  algorithmState = 'b';
		  calculatePoints(true);
		  break;
	  case 'd':
		  algorithmState = 'd';
		  calculatePoints(true);
		  break;
      default:
         break;
   }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void){
   cout << "Interaction:" << endl;
   cout << "Left mouse click to draw a point, right mouse click to exit." << endl; 
   cout << "Enter in d for DDA, b for Bresenham, and a for antialiasing." << endl; 
}

// Main routine.
int main(int argc, char **argv){
   printInteraction();
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("Line Algorithms");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   
   // Register the mouse callback function. 
   glutMouseFunc(mouseControl); 

   glutMainLoop(); 

   return 0;  
}