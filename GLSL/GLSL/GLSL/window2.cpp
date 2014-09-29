#include <string.h>
#include "time.h"
#include <stdlib.h>
#include "window2.h"
#include "math.h"

GLFWwindow* Window::window = NULL;
Scene* Window::scene = NULL;
Mouse Window::mouse;
int Window::isVisible;
vec3 Window::center;
double Window::radius;

//double Window::frameRate;
//double Window::frameCountStart;
//Prompt Window::prompt;
//int Window::frameCount;
//double Window::startTime;


//#define PI 3.1415926
//
//const char Window::KEY_ESCAPE = '\033';
//Scene* Window::scene = NULL;
//Mouse Window::mouse;
////int Window::curveFit = LINEAR;
//int Window::isVisible;
//int Window::frameCount;
//double Window::startTime;
//double Window::radius;
//double Window::frameRate;
//double Window::frameCountStart;
//Prompt Window::prompt;
//Point<3> Window::center;
//
//
//
///** This function prints out the state of the OpenGL error. */
//int Window::PrintError(const int& showNoError){
//	int x, y;
//	int e = 1;
//	switch (glGetError()){
//	case GL_NO_ERROR:
//		if (showNoError){ printf("No error\n"); }
//		e = 0;
//		break;
//	case GL_INVALID_ENUM:
//		printf("Invalid Enum\n");
//		break;
//	case GL_INVALID_VALUE:
//		printf("Invalid Value\n");
//		break;
//	case GL_INVALID_OPERATION:
//		printf("Invalid Operation\n");
//		break;
//	case GL_STACK_OVERFLOW:
//		printf("Stack Overflow\n");
//		break;
//	case GL_STACK_UNDERFLOW:
//		printf("Stack Underflow\n");
//		break;
//	case GL_OUT_OF_MEMORY:
//		printf("Out of memory\n");
//		break;
//	}
//	if (!showNoError && !e){ return 0; }
//
//	glGetIntegerv(GL_MATRIX_MODE, &x);
//	if (x == GL_MODELVIEW){
//		glGetIntegerv(GL_MODELVIEW_STACK_DEPTH, &x);
//		glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &y);
//		printf("Modelview Matrix: %d/%d\n", x, y);
//	}
//	else if (x == GL_PROJECTION){
//		glGetIntegerv(GL_PROJECTION_STACK_DEPTH, &x);
//		glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &y);
//		printf("Projection Matrix: %d/%d\n", y);
//	}
//	return e;
//}
//
///** This function writes out the specified string, left-aligned, at the specified location, onto the OpenGL window. */
//void Window::WriteLeftString(const int& x, const int& y, const char* str){
//	GLint vp[4];
//	GLint dt = glIsEnabled(GL_DEPTH_TEST);
//	GLint lm = glIsEnabled(GL_LIGHTING);
//	GLint mm;
//
//	glGetIntegerv(GL_VIEWPORT, vp);
//	glGetIntegerv(GL_MATRIX_MODE, &mm);
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glLoadIdentity();
//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//	glLoadIdentity();
//	glOrtho(0, vp[2], 0, vp[3], 0, 1);
//
//	glDisable(GL_DEPTH_TEST);
//	glDisable(GL_LIGHTING);
//	glColor4f(0, 0, 0, 1);
//	glRasterPos2f(x, y);
//	int len = strlen(str);
//	for (int i = 0; i<len; i++){ glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]); }
//	glPopMatrix();
//	glMatrixMode(GL_MODELVIEW);
//	glPopMatrix();
//
//	if (dt){ glEnable(GL_DEPTH_TEST); }
//	if (lm){ glEnable(GL_LIGHTING); }
//	glMatrixMode(mm);
//}
///** This function writes out the specified string, right-aligned, at the specified location, onto the OpenGL window. */
//void Window::WriteRightString(const int& x, const int& y, const char* str){
//	GLint vp[4];
//	glGetIntegerv(GL_VIEWPORT, vp);
//
//	WriteLeftString(vp[2] - x - glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (unsigned char*)str), y, str);
//}
//
///** This function reads the current frame buffer and sets the pixels of the image accordingly. */
////int Window::TakeSnapshot(Image32& img){
////	GLfloat *pixels;
////	int i, j, temp;
////	Pixel p;
////	GLint vp[4];
////	glGetIntegerv(GL_VIEWPORT, vp);
////
////	if (!img.setSize(vp[2], vp[3])){ return 0; }
////	pixels = new GLfloat[vp[2] * vp[3] * 3];
////	if (!pixels){ return 0; }
////	glReadBuffer(GL_FRONT);
////	glReadPixels(vp[0], vp[1], vp[2], vp[3], GL_RGB, GL_FLOAT, pixels);
////
////	for (i = 0; i<vp[3]; i++){
////		for (j = 0; j<vp[2]; j++){
////			temp = 0 + j * 3 + (vp[3] - i - 1)*(vp[2]) * 3;
////			p.r = 255 * pixels[temp];
////			temp = 1 + j * 3 + (vp[3] - i - 1)*(vp[2]) * 3;
////			p.g = 255 * pixels[temp];
////			temp = 2 + j * 3 + (vp[3] - i - 1)*(vp[2]) * 3;
////			p.b = 255 * pixels[temp];
////			img(j, i) = p;
////		}
////	}
////	delete[] pixels;
////	return 1;
////}
//
///** This function is called when no events need to be processed. */
//void Window::IdleFunction(void){
//	// Update the parameter values
//	scene->setCurrentTime(GetTime() - startTime);
//	// Just draw the scene again
//	if (isVisible){ glutPostRedisplay(); }
//}
///** This function is called when the visibility state of the window changes. */
//void Window::VisibilityFunction(int state){
//	if (state == GLUT_VISIBLE){ isVisible = 1; }
//	else if (state == GLUT_NOT_VISIBLE){ isVisible = 0; }
//}
//
////////////////////////////
//// Mouse event handlers //
////////////////////////////
/** This function is called when the state of a mouse button is changed */

/** This function is called when one of the mouse buttons is depressed and the mouse is moved. */

///////////////////////////////
//// Keyboard event handlers //
///////////////////////////////
///** This function is called when the user presses a key. */
//void Window::KeyboardFunction(unsigned char c, int x, int y){
//	char temp[500];
//	//Image32 img;
//
//	if (prompt.isActive)
//	{
//		size_t len = strlen(prompt.string_value);
//		if (c == KEY_BS)
//		{
//			if (len>prompt.initial_length) prompt.string_value[len - 1] = 0;
//		}
//		else if (c == KEY_ENTER)
//		{
//			scene->graphic_objects[0]->HandleKeyboardEvent(prompt);
//			prompt.isActive = false;
//			prompt.string_value[0] = 0;
//			prompt.initial_length = 0;
//			prompt.active_command = 0;
//		}
//		else if (c == KEY_CTRL_C)
//		{
//			prompt.isActive = false;
//			prompt.string_value[0] = 0;
//			prompt.initial_length = 0;
//			prompt.active_command = 0;
//		}
//		else if (c >= 32 && c <= 126) // ' ' to '~'
//		{
//			prompt.string_value[len] = c;
//			prompt.string_value[len + 1] = 0;
//		}
//		//glutPostRedisplay();
//		//return;
//	}
//	else
//	{
//		prompt.active_command = c;
//		if (c == 'z'){
//			scene->camera->moveForward(radius / 250);
//		}
//		else if (c == 'x'){
//			scene->camera->moveForward(-radius / 250);
//		}
//		else if (c == 'w'){
//			scene->camera->rotateDirection(center, 0.03);
//		}
//		else{
//			scene->graphic_objects[0]->HandleKeyboardEvent(prompt);
//		}
//	}
//	glutPostRedisplay();
//}
///** This function is called when the user presses one of the special keys. */
//void Window::SpecialFunction(int key, int x, int y){
//	switch (key){
//	case GLUT_KEY_F1:
//		break;
//	case GLUT_KEY_F2:
//		break;
//	case GLUT_KEY_F3:
//		break;
//	case GLUT_KEY_F4:
//		break;
//	case GLUT_KEY_F5:
//		break;
//	case GLUT_KEY_F6:
//		break;
//	case GLUT_KEY_F7:
//		break;
//	case GLUT_KEY_F8:
//		break;
//	case GLUT_KEY_F9:
//		break;
//	case GLUT_KEY_F10:
//		break;
//	case GLUT_KEY_F11:
//		break;
//	case GLUT_KEY_F12:
//		break;
//	case GLUT_KEY_UP:
//		break;
//	case GLUT_KEY_DOWN:
//		break;
//	case GLUT_KEY_LEFT:
//		break;
//	case GLUT_KEY_RIGHT:
//		break;
//	case GLUT_KEY_PAGE_UP:
//		break;
//	case GLUT_KEY_PAGE_DOWN:
//		break;
//	case GLUT_KEY_HOME:
//		break;
//	case GLUT_KEY_END:
//		break;
//	case GLUT_KEY_INSERT:
//		break;
//	}
//	glutPostRedisplay();
//}
//
///////////////////////////
//// Menu event handlers //
///////////////////////////
//static bool drawWithOpenGL = true;
///** This function is called when the user updates the draw mode in the drop-down menu. */
//void Window::DrawModeMenu(int entry){
//	if (entry == GL_POINT || entry == GL_LINE || entry == GL_FILL)
//	{
//		drawWithOpenGL = true;
//		glPolygonMode(GL_FRONT_AND_BACK, entry);
//		glutPostRedisplay();
//	}
//	else {
//		drawWithOpenGL = false;
//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//		glutPostRedisplay();
//	}
//}
//
///** This function is called when the user updates the cull mode in the drop-down menu. */
//void Window::CullModeMenu(int entry){
//	if (entry == NO_CULL){ glDisable(GL_CULL_FACE); }
//	else{
//		glEnable(GL_CULL_FACE);
//		if (entry == CULL_BACK_FACE){ glCullFace(GL_BACK); }
//		if (entry == CULL_FRONT_FACE){ glCullFace(GL_FRONT); }
//	}
//	glutPostRedisplay();
//}
///** This function is called when the user updates the curve fitting method in the drop-down menu. */
////void Window::CurveFitMenu(int entry){
////	curveFit = entry;
////	startTime = GetTime();
////	glutPostRedisplay();
////}
///** This function is called when the user selects one of the main menu options in the drop-down menu. */
//void Window::MainMenu(int entry){
//	if (!entry){ exit(0); }
//}
//
///////////////////////////
//
///**  This function draws the OpenGL window. */
//void Window::DisplayFunction(void){
//	char temp[500];
//	double t;
//
//	// Clear the color and depth buffers
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	float d = radius + (scene->camera->position - center).norm();
//
//	// Draw the perspective projection (to get good front and back clipping planes, we need to know the rough
//	// size of the model)
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	gluPerspective(scene->camera->heightAngle*180.0 / PI, scene->camera->aspectRatio, .1*d, 2 * d);
//
//	// Draw the Scene
//	GLint drawMode[2];
//	glGetIntegerv(GL_POLYGON_MODE, drawMode);
//	if (drawMode[0] == GL_FILL){ scene->drawOpenGL(); }
//	else{
//		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//		glPolygonOffset(1, 2);
//		glDisable(GL_BLEND);
//		scene->drawOpenGL();
//		glDisable(GL_POLYGON_OFFSET_FILL);
//		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
//		glPolygonMode(GL_FRONT_AND_BACK, drawMode[0]);
//		glDisable(GL_BLEND);
//		glEnable(GL_LINE_SMOOTH);
//		glLineWidth(2);
//		scene->drawOpenGL();
//	}
//
//
//	// Update the frame rate value on the tenth frame
//	frameCount++;
//	if (frameCount == 10){
//		frameCount = 0;
//		t = frameCountStart;
//		frameCountStart = GetTime();
//		frameRate = 10 / (frameCountStart - t);
//	}
//	//sprintf(temp, "%.1f fs", frameRate);
//	WriteLeftString(1, 2, prompt.string_value);
//
//	// Write out the mouse position
//	sprintf(temp, "(%3d , %3d)", mouse.endX, mouse.endY);
//	WriteRightString(1, 2, temp);
//
//	// Swap the back and front buffers
//	glutSwapBuffers();
//}
//
///** This function is called when the window is resized. */
//void Window::ReshapeFunction(int width, int height)
//{
//	GLint viewPort[4];
//
//	glViewport(0, 0, width, height);
//	glGetIntegerv(GL_VIEWPORT, viewPort);
//	scene->camera->aspectRatio = (float)width / (float)height;
//	glutPostRedisplay();
//}

/** This function instantiates the OpenGL window, reading in the Scene from the specified file
* and setting the initial OpenGL window size. The function never returns! */

void Window::mainLoop()
{
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		scene->DrawOpenGL();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

//========================================================================
// Callback function for mouse button events
//========================================================================


void Window::Mouse_Callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS){
		mouse.any_button_pressed = true;
		if (button == GLFW_MOUSE_BUTTON_RIGHT)
			mouse.active_button = RIGHT_BUTTON;
		else if (button == GLFW_MOUSE_BUTTON_LEFT)
			mouse.active_button = LEFT_BUTTON;
		else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
			mouse.active_button = MIDDLE_BUTTON;
	}
	if (action == GLFW_RELEASE){
		mouse.any_button_pressed = false;
		mouse.active_button = NONE_BUTTON;
	}


}

void Window::Cursor_Callback(GLFWwindow* window, double x, double y)
{
	if (mouse.any_button_pressed)
	{
		double displacement_x = x - mouse.cursor_x;
		double displacement_y = y - mouse.cursor_y;

		if (mouse.active_button == LEFT_BUTTON)
		{
			scene->camera.rotateUp(center, 0.001f*displacement_x);
			scene->camera.rotateRight(center, 0.001f*displacement_y);
		}
		else if (mouse.active_button == MIDDLE_BUTTON)
		{
			scene->lights[0]->rotateX(0.001f*displacement_x);
			scene->lights[0]->rotateY(0.001f*displacement_y);
		}
		else if (mouse.active_button == RIGHT_BUTTON)
		{
			scene->camera.moveRight(-radius * displacement_x / 2500.f);
			scene->camera.moveUp(radius * displacement_y / 2500.f);
		}

	}

	mouse.cursor_x = x;
	mouse.cursor_y = y;
}
//void Window::MotionFunction(int x, int y){
//	Point<2> d = mouse.move(x, y);
//
//	if (mouse.middleDown){
//		scene->camera->rotateUp(center, 0.001*d[0]);
//		scene->camera->rotateRight(center, 0.001*d[1]);
//	}
//	if (mouse.scrollDown){
//		scene->camera->moveForward(radius / 15 * d[1]);
//	}
//	if (mouse.leftDown){
//		scene->camera->moveRight(-radius / 2500 * d[0]);
//		scene->camera->moveUp(radius / 2500 * d[1]);
//	}
//
//	glutPostRedisplay();
//}
///** This function is called when the mouse is moved moved but no buttons are depressed. */
//void Window::PassiveMotionFunction(int x, int y){
//	mouse.move(x, y);
//	glutPostRedisplay();
//}


//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
//{
//	if (button != GLFW_MOUSE_BUTTON_LEFT)
//		return;
//
//	if (action == GLFW_PRESS)
//	{
//		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//		locked = GL_TRUE;
//	}
//	else
//	{
//		locked = GL_FALSE;
//		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//	}
//}
//
//
////========================================================================
//// Callback function for cursor motion events
////========================================================================
//
//void cursor_position_callback(GLFWwindow* window, double x, double y)
//{
//	if (locked)
//	{
//		alpha += (GLfloat)(x - cursorX) / 10.f;
//		beta += (GLfloat)(y - cursorY) / 10.f;
//	}
//
//	cursorX = (int)x;
//	cursorY = (int)y;
//}

void Window::Initialize(Scene* s, int width, int height){
	

	//Initialize GL utilites: glfw
	if (!glfwInit()) exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	window = glfwCreateWindow(500, 500, "Hello", NULL, NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	// Initialize GL extensions : glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) fprintf(stderr, "glewInit failed. Exiting...\n");

	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	printf("GL Vendor            : %s\n", vendor);
	printf("GL Renderer          : %s\n", renderer);
	printf("GL Version (string)  : %s\n", version);
	printf("GL Version (integer) : %d.%d\n", major, minor);
	printf("GLSL Version         : %s\n", glslVersion);

	//GLint nExtensions;
	//glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
	//for (int i = 0; i < nExtensions; i++)
	//	printf("%s\n", glGetStringi(GL_EXTENSIONS, i));

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	scene = s;
	scene->SetupOpenGL();

	center = vec3(0.f, 0.f, 0.f);
	radius = 5.f;

	glfwSetMouseButtonCallback(window,Mouse_Callback);
	glfwSetCursorPosCallback(window,Cursor_Callback);

	// Enter the main loop
	double cursor_x;
	double cursor_y;
	glfwGetCursorPos(window, &cursor_x, &cursor_y);
	mouse.Initialize(cursor_x, cursor_y);
	mainLoop();



	// Close window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	// Exit program

	//int drawMenu;
	//int cullMenu;
	////	int curveFitMenu;
	//int argc = 1;
	//char* argv[] = { "foo" };
	//startTime = GetTime();

	//
	/////BoundingBox3D b = scene->group->setBoundingBox();
	////center = (b.p[0] + b.p[1]) / 2;
	////radius = (b.p[0] - b.p[1]).length() / 2;
	//center = Point<3>(0.f, 0.f, 0.f);
	//radius = 5.f;
	//frameCountStart = GetTime();

	//// Initialize the OpenGL context
	//glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	//glutCreateWindow("Subdivision Surfaces");

	//if (glewInit() != GLEW_OK) fprintf(stderr, "glewInit failed. Exiting...\n");

	//glutInitWindowSize(width, height);
	//glutInitWindowPosition(0, 0);
	//glClearColor(1.f, 1.f, 1.f, 1.0);
	//glutReshapeWindow(width, height);

	//// Initialize the event handlers
	//glutDisplayFunc(DisplayFunction);
	//glutReshapeFunc(ReshapeFunction);
	//glutKeyboardFunc(KeyboardFunction);
	//glutSpecialFunc(SpecialFunction);
	//glutMouseFunc(MouseFunction);
	//glutMotionFunc(MotionFunction);
	//glutPassiveMotionFunc(PassiveMotionFunction);
	//glutVisibilityFunc(VisibilityFunction);
	//glutIdleFunc(IdleFunction);

	//// Initialize the menus
	//drawMenu = glutCreateMenu(DrawModeMenu);
	//glutAddMenuEntry(" Polygons ", GL_FILL);
	//glutAddMenuEntry(" Lines ", GL_LINE);
	//glutAddMenuEntry(" Points ", GL_POINT);

	//cullMenu = glutCreateMenu(CullModeMenu);
	//glutAddMenuEntry(" None ", NO_CULL);
	//glutAddMenuEntry(" Back ", CULL_BACK_FACE);
	//glutAddMenuEntry(" Front ", CULL_FRONT_FACE);

	////curveFitMenu = glutCreateMenu(CurveFitMenu);
	////glutAddMenuEntry(" Linear Interpolation ", LINEAR);
	////glutAddMenuEntry(" Catmull-Rom Interpolation ", CATMULL_ROM);
	////glutAddMenuEntry(" Uniform Cubic B-Spline Approximation ", UNIFORM_CUBIC_B_SPLINE);

	//glutCreateMenu(MainMenu);
	//glutAddSubMenu(" Polygon mode ", drawMenu);
	//glutAddSubMenu(" Cull mode ", cullMenu);
	////glutAddSubMenu(" Animation Fitting ", curveFitMenu);
	//glutAddMenuEntry(" Quit ", 0);

	//// Attach the drop-down menu to the right button
	//glutAttachMenu(GLUT_RIGHT_BUTTON);

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_NORMALIZE);
	//glDepthMask(GL_TRUE);
	//glDisable(GL_BLEND);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	//// Set up all the necessary stuff for the RayScene
	//scene->setUpOpenGL(cplx);
	//// Fall into the main loop
	//glutMainLoop();
}
