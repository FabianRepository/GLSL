#ifndef SCENE_INCLUDE
#define SCENE_INCLUDE

#include "graphic-object.h"
#include "GLSLProgram.h"

const mat4 shadowBias = mat4(vec4(0.5f, 0.0f, 0.0f, 0.0f),
							  vec4(0.0f, 0.5f, 0.0f, 0.0f),
							  vec4(0.0f, 0.0f, 0.5f, 0.0f),
							  vec4(0.5f, 0.5f, 0.5f, 1.0f));

class  Light{ //: public GraphicObject{
public:
	vec3 color;
	mat4 biased_light_view_projection; // directed towards the scene center
	virtual void rotateX(float angle) = 0;
	virtual void rotateY(float angle) = 0;
	virtual void SetLightViewProjection(vec3 scene_center, float scene_radius) = 0;
	//void SetupOpenGL();
	//void DrawOpenGL(Scene * scene);
	//GLuint position_buffer;
	//void HandleKeyboardEvent(Prompt  & prompt){}
};


class PointLight : public Light{
public:
	vec3 position;
	void rotateX(float angle);
	void rotateY(float angle);
	void SetLightViewProjection(vec3 scene_center, float scene_radius);
};

class DirectionalLight : public Light{
public:
	vec3 direction;
	void rotateX(float angle);
	void rotateY(float angle);
	void SetLightViewProjection(vec3 scene_center, float scene_radius);
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

typedef std::vector<GraphicObject *> GraphicObjectsList;

class ShadingGroup : public GraphicObject{
public: 
	ShadingGroup(const char * p_vertex_shader, const char * p_fragment_shader){
		sprintf(vertex_shader, "%s", p_vertex_shader);
		sprintf(fragment_shader, "%s", p_fragment_shader);
	}
	GLSLProgram shading_program;
	GLuint program_handle;
	char vertex_shader[100];
	char fragment_shader[100];
	GraphicObjectsList graphic_objects;
	void SetupOpenGL();
	void DrawOpenGL(Scene * scene);
	void HandleKeyboardEvent(Prompt  & prompt){}
};

typedef std::vector<ShadingGroup *> ShadingGroupList;
typedef std::vector<PointLight *> PointLightList;
typedef std::vector<DirectionalLight *> DirectionalLightList;
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
	void Initialize(const int width, const int height, const float p_radius, const vec3 p_center);
	Camera camera;
	void SetWorldToCamera();
	void SetProjectionMatrix();
	void SetMVP();
	void SetupOpenGL();
	void DrawOpenGL();

	PointLightList point_lights;
	DirectionalLightList directional_lights;
	//GraphicObject ** graphic_objects;
	//int num_graphic_objects;
	ShadingGroupList shading_groups;


	mat4 projection_matrix;
	mat4 world_to_camera;
	mat4 mvp;

	vec3 center;
	float radius;
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