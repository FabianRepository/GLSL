#include "mesh.h"
class  Light{
public:
	Point<3> color;
	virtual void drawOpenGL(int index) = 0;
};

class DirectionalLight : public Light{
public:
	Point<3> direction;
	void drawOpenGL(int index);
};

class Material {
public:
	int index;
	Point<3> ambient;
	Point<3> diffuse;
	Point<3> specular;
	Point<3> emissive;
	Point<3> transparent;
	double specularFallOff;
	//double refind;
	//class Texture *tex;
	void drawOpenGL(void);
};

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
	double heightAngle;
	double aspectRatio;
	Point<3> position;
	Point<3> direction;
	Point<3> up;
	Point<3> right;

	void drawOpenGL(void);

	/** This call rotates the camera, about the axis which is parallel to the up direction of the camera, and passes
	* through the specified point. */
	void rotateUp(Point<3> center, float angle);
	/** This call rotates the camera, about the axis which is parallel to the right direction of the camera, and passes
	* through the specified point. */
	void rotateRight(Point<3> center, float angle);
	void rotateDirection(Point<3> center, float angle);
	/** This call moves the camera in the forward direction by the specified distance.*/
	void moveForward(float dist);
	/** This call moves the camera in the right direction by the specified distance.*/
	void moveRight(float dist);
	/** This call moves the camera in the up direction by the specified distance.*/
	void moveUp(float dist);
};

class Scene{
public:
	Material ** materials;
	int materialNum;
	//Texture* textures;
	//int textureNum;
	//RayFile* rayFiles;
	//int rayFileNum;
	//RayKeyFile* keyFiles;
	//RayKeyData* keyData;
	//int keyFileNum;

	//void ParseError(const char *);
	//void ParseLineError(int, const char *);
	//void ParseGroup(FILE* fp, int& cmndCtr, class RayGroup* current);
	//void ParseJoint(FILE* fp, int& cmndCtr, class RayGroup* current);
	//int  ParseShape(FILE* fp, const char* keyword, int cmndCtr, class RayGroup* current);

	double current_time;
	

	Camera* camera;
	Light** lights;
	int lightNum;
	GraphicObject ** graphic_objects;
	int num_graphic_objects;


	//MeshObject * mesh;
	/** The root of the scene-graph */
	//class StaticRayGroup* group;

	Scene(void){}
	//~Scene(void);

	/** This method returns the material with the specified index.*/
	Material* getMaterial(int index);

	//////////////////
	// OpenGL stuff //
	//////////////////

	//void RayView(const int& width, const int& height, const int& cplx, const int& factor = RayKeyData::MATRIX);

	/** This method calls the OpenGL commands for drawing the scene. */
	void drawOpenGL(void);

	/** This method class the OpenGL commands to set up everything that needs to be set up prior to rendering */
	void setUpOpenGL(int cplx);

	/** This method updates the current time, changing the parameter values as needed */
	void setCurrentTime(double t);
};

