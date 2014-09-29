#ifndef MOUSE_INCLUDED
#define MOUSE_INCLUDED

/** This class represents the current state of the mouse */

template< int dimension>
class Point;

enum ActiveButton{ NONE_BUTTON, RIGHT_BUTTON, LEFT_BUTTON, MIDDLE_BUTTON };

class Mouse{
public:
	void Initialize(double p_cursor_x, double p_cursor_y) {
		cursor_x = p_cursor_x;
		cursor_y = p_cursor_y;
		displacement_x = 0.f;
		displacement_y = 0.f;
		active_button = NONE_BUTTON;
		any_button_pressed = false;
	}
	ActiveButton active_button;
	bool any_button_pressed;
	double cursor_x,cursor_y;
	double displacement_x, displacement_y;
};
#endif // MOUSE_INCLUDED