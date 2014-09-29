#include "mouse.h"
#include "geometry.h"
#include "GLFW\glfw3.h"

void Mouse::Initialize(double p_cursor_x, double p_cursor_y) {
	right_pressed = false;
	left_pressed = false;
	cursor_x = p_cursor_x;
	cursor_y = p_cursor_y;
	displacement_x = 0.f;
	displacement_y = 0.f;

}
void Mouse::update(int button,int state,int x, int y){
	int c;
	if(state==GLUT_DOWN){
		startX=endX=x;
		startY=endY=y;
		if(button==GLUT_LEFT_BUTTON		){leftDown=1;}
		if(button==GLUT_MIDDLE_BUTTON	){middleDown=1;}
		if(button==GLUT_RIGHT_BUTTON	){rightDown=1;}
#ifdef GLUT_SCROLL_WHEEL
		if(button==GLUT_SCROLL_WHEEL	){scrollDown=1;}
#endif

		c=glutGetModifiers();
		if(c&GLUT_ACTIVE_SHIFT){shiftDown=1;}
		else{shiftDown=0;}
		if(c&GLUT_ACTIVE_CTRL){ctrlDown=1;}
		else{ctrlDown=0;}
		if(c&GLUT_ACTIVE_ALT){altDown=1;}
		else{altDown=0;}
	}
	else if(state==GLUT_UP){
		endX=x;
		endY=y;
		if(button==GLUT_LEFT_BUTTON		){leftDown=0;}
		if(button==GLUT_MIDDLE_BUTTON	){middleDown=0;}
		if(button==GLUT_RIGHT_BUTTON	){rightDown=0;}
#ifdef GLUT_SCROLL_WHEEL
		if(button==GLUT_SCROLL_WHEEL	){scrollDown=0;}
#endif
	}
}
Point<2> Mouse::move(int x,int y){
	Point<2> d;

	d.coord[0]=x-endX;
	d.coord[1]=y-endY;
	endX=x;
	endY=y;
	return d;
}
