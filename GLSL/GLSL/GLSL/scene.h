#ifndef SCENE_INCLUDE
#define SCENE_INCLUDE

#include "graphic-object.h"

class  Light : public GraphicObject{
public:
	vec3 color;
	vec4 position;
	void rotateX(float angle);
	void rotateY(float angle);
	void SetupOpenGL();
	void DrawOpenGL(Scene * scene);
	GLuint position_buffer;
	void HandleKeyboardEvent(Prompt  & prompt){}
};

//
//class Material {
//public:
//	int index;
//	Point<3> ambient;
//	Point<3> diffuse;
//	Point<3> specular;
//	Point<3> emissive;
//	Point<3> transparent;
//	double specularFallOff;
//	//double refind;
//	//class Texture *tex;
//	void drawOpenGL(void);
//};

//class Texture{
//public:
//	RayTexture(void);
//
//	int index;
//	Image32 *img;
//	GLuint openGLHandle;
//	void setUpOpenGL(void);
//};

class Camera {
public:
	float heightAngle;
	float aspectRatio;
	vec3 position;
	vec3 direction;
	vec3 up;
	vec3 right;
	
	

	///** This call rotates the camera, about the axis which is parallel to the up direction of the camera, and passes
	//* through the specified point. */
	void rotateUp(vec3 center, float angle);
	///** This call rotates the camera, about the axis which is parallel to the right direction of the camera, and passes
	//* through the specified point. */
	void rotateRight(vec3 center, float angle);
	//void rotateDirection(Point<3> center, float angle);
	///** This call moves the camera in the forward direction by the specified distance.*/
	//void moveForward(float dist);
	///** This call moves the camera in the right direction by the specified distance.*/
	void moveRight(float dist);
	///** This call moves the camera in the up direction by the specified distance.*/
	void moveUp(float dist);
};

//class TestObject : public GraphicObject
//{
//public:
//	void SetupOpenGL();
//	void DrawOpenGL(const mat4 mvp);
//	void HandleKeyboardEvent(Prompt  & prompt) {}
//	GLuint programHandle;
//	GLuint vaoHandle;
//};

class Scene{
public:
	//Material ** materials;
	//int materialNum;
	////Texture* textures;
	////int textureNum;
	////RayFile* rayFiles;
	////int rayFileNum;
	////RayKeyFile* keyFiles;
	////RayKeyData* keyData;
	////int keyFileNum;

	////void ParseError(const char *);
	////void ParseLineError(int, const char *);
	////void ParseGroup(FILE* fp, int& cmndCtr, class RayGroup* current);
	////void ParseJoint(FILE* fp, int& cmndCtr, class RayGroup* current);
	////int  ParseShape(FILE* fp, const char* keyword, int cmndCtr, class RayGroup* current);

	//double current_time;
	//
	void Initialize(const int width, const int height);
	Camera camera;
	void SetWorldToCamera();
	void SetProjectionMatrix();
	void SetMVP();
	void SetupOpenGL();
	void DrawOpenGL();

	Light** lights;
	int lightNum;
	GraphicObject ** graphic_objects;
	int num_graphic_objects;

	mat4 projection_matrix;
	mat4 world_to_camera;
	mat4 mvp;
	////MeshObject * mesh;
	///** The root of the scene-graph */
	////class StaticRayGroup* group;

	//Scene(void){}
	////~Scene(void);

	///** This method returns the material with the specified index.*/
	//Material* getMaterial(int index);

	////////////////////
	//// OpenGL stuff //
	////////////////////

	////void RayView(const int& width, const int& height, const int& cplx, const int& factor = RayKeyData::MATRIX);

	///** This method calls the OpenGL commands for drawing the scene. */
	//void drawOpenGL(void);

	///** This method class the OpenGL commands to set up everything that needs to be set up prior to rendering */
	//void setUpOpenGL(int cplx);

	///** This method updates the current time, changing the parameter values as needed */
	//void setCurrentTime(double t);
};

#endif // #SCENE_INCLUDE