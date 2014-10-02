#ifndef WINDOW_INCLUDE
#define WINDOW_INCLUDE

#include "mouse.h"
#include "scene.h"
#include "GLFW\glfw3.h" 
//#include "mouse.h"

#define KEY_UPARROW		101
#define KEY_DOWNARROW	103
#define KEY_LEFTARROW	100
#define KEY_RIGHTARROW	102
#define KEY_PGUP		104
#define KEY_PGDN		105
#define KEY_CTRL_C  3
#define KEY_BS      8
#define KEY_ENTER  13
#define KEY_ESC    27

class Window{
public:
	///** The ascii value of the escape character */
	//const char KEY_ESCAPE;
	//enum{
	//	/** A value indicating that no culling should be used */
	//	NO_CULL,
	//	/** A value indicating that back-face culling should be used */
	//	CULL_BACK_FACE,
	//	/** A value indicating that front-face culling should be used */
	//	CULL_FRONT_FACE
	//};

	static GLFWwindow *window;
	/** A pointer to the scene that is drawn.*/
	static Scene * scene;
	/** The current state of the mouse */
	static Mouse mouse;
	///** The center of the bounding box of the drawn scene */
	//vec3 center;
	///** The radius of the bounding box of the drawn scene */
	//double radius;
	/** Is the window visible */
	static int isVisible;

	static void Mouse_Callback(GLFWwindow* window, int button, int action, int mods);
	static void Cursor_Callback(GLFWwindow* window, double x, double y);
	static void mainLoop();

	///** The number of frames that have been drawn since the last
	//* updated frame rate */
	//int frameCount;
	///** The time of the last updated frame rate */
	//double frameCountStart;
	///** The current frame rate */
	//double frameRate;

	//double startTime;

	////static int curveFit;
	//Prompt prompt;

	///** This function prints out the state of the OpenGL error. */
	//int PrintError(const int& showNoError = 0);
	///** This function writes out the specified string, left-aligned, at the specified location, onto the OpenGL window. */
	//void WriteLeftString(const int& x, const int& y, const char* str);
	///** This function writes out the specified string, right-aligned, at the specified location, onto the OpenGL window. */
	//void WriteRightString(const int& x, const int& y, const char* str);
	////void WriteString(int x, int y, void* font, bool bottom, bool left, const char* format, ...);

	///** This function reads the current frame buffer and sets the pixels of the image accordingly. */
	////int TakeSnapshot(Image32& img);
	///** This function is called when no events need to be processed. */
	//void IdleFunction(void);
	///** This function is called when the visibility state of the window changes. */
	//void VisibilityFunction(int state);


	//////////////////////////
	// Mouse event handlers //
	//////////////////////////
	/** This function is called when the state of a mouse button is changed */
	//void MouseFunction(int button, int state, int x, int y);
	///** This function is called when one of the mouse buttons is depressed and the mouse is moved. */
	//void MotionFunction(int x, int y);
	///** This function is called when the mouse is moved moved but no buttons are depressed. */
	//void PassiveMotionFunction(int x, int y);

	///////////////////////////////
	//// Keyboard event handlers //
	///////////////////////////////
	///** This function is called when the user presses a key. */
	//void KeyboardFunction(unsigned char c, int x, int y);
	///** This function is called when the user presses one of the special keys. */
	//void SpecialFunction(int key, int x, int y);

	///////////////////////////
	//// Menu event handlers //
	///////////////////////////
	///** This function is called when the user updates the draw mode in the drop-down menu. */
	//void DrawModeMenu(int entry);
	///** This function is called when the user updates the cull mode in the drop-down menu. */
	//void CullModeMenu(int entry);
	///** This function is called when the user updates the curve fitting method in the drop-down menu. */
	//void CurveFitMenu(int entry);
	///** This function is called when the user selects one of the main menu options in the drop-down menu. */
	//void MainMenu(int entry);
	///**  This function draws the OpenGL window. */
	//void drawScene(float jitterX, float jitterY);
	//void DisplayFunction(void);
	///** This function is called when the window is resized. */
	//void ReshapeFunction(int width, int height);


	//void raytraceScene(void);

	///** This function instantiates the OpenGL window, reading in the RayScene from the specified file
	//* and setting the initial OpenGL window size. The function never returns! */
	static void Initialize(Scene* scene, int width, int height);
};

#endif //WINDOW_INCLUDE