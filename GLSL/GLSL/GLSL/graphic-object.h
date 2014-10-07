#ifndef GRAPHIC_OBJECT_INCLUDE
#define GRAPHIC_OBJECT_INCLUDE

#include "geometry.h"
//#include "shaders.h"
#include "GLSLProgram.h"

class Scene;

class Prompt{
public:
	Prompt()
	{
		isActive = false;
		string_value[0] = 0;
		initial_length = 0;
		active_command = 0;
	}
	bool isActive;
	char string_value[500];
	int initial_length;
	unsigned char active_command;
};

class Material {
public:
	GLfloat ambient[3];
	GLfloat diffuse[3];
	GLfloat specular[3];
	GLfloat emissive[3];
	float shininess;
	void drawOpenGL();
};

class GraphicObject {
public:
	virtual void SetupOpenGL() = 0;
	virtual void DrawOpenGL(Scene * scene) = 0;
	virtual void HandleKeyboardEvent(Prompt  & prompt) = 0;
	mat4 object_to_world;
	mat3 normal_matrix;
	//GLuint program_handle;
	GLSLProgram * glsl_program;
	GLuint vaoHandle;
	Material * material;
};


class Statistics_Base{
public:
	Statistics_Base()
	{
		poor_aspect_faces = 0;
		poor_aspect_vertices = 0;
	}
	double average_edge_lenght;
	int poor_aspect_faces;
	int poor_aspect_vertices;
};

class Statistics_Advection : public Statistics_Base
{
public:
	Statistics_Advection(int p_num_percentiles = 10) : Statistics_Base()
	{
		num_percentiles = p_num_percentiles;
		gradient_percentiles = new double[num_percentiles];
		advection_percentiles = new double[num_percentiles];
		accumulation_percentiles = new double[num_percentiles];
	}

	int num_percentiles;
	double max_gradient_vector_norm;
	double mean_gradient_vector_norm;
	double * gradient_percentiles;

	double max_advection_vector_norm;
	double mean_advection_vector_norm;
	double * advection_percentiles;

	double max_accumulation_vector_norm;
	double mean_accumulation_vector_norm;
	double * accumulation_percentiles;
};

class Statistics_Curve_Advection : public Statistics_Base
{
public:
	double max_gradient_vector_norm;
	double max_advection_vector_norm;

	double max_accumulation_vector_norm;
	double max_curvature_vector_norm;
};



enum ParameterMode{ INPUT, OUTPUT };
enum PropertyMode { COLOR, NORMAL };
enum FieldMode { GRADIENT, ADVECTION, ACCUMULATION, NONE, CURVATURE };
enum StateMode { INITIAL, CURRENT };
enum RegularizationMode { INITIAL_REGULARIZATION, UNIFORM_REGULARIZATION };
enum LaplacianMode { UNIFORM, COTANGENT };
enum ValueMode { ALL_TO_COLOR, ALL_TO_TEST, ALL_TO_TEXTURE, CURRENT_TO_EXTERNAL, SMOOTH_CURRENT, ALL_TO_NORMAL };
enum NormalWeightMode { UNIFORM_NORMAL_WEIGHT, ANGLE_NORMAL_WEIGHT };
#endif //GRAPHIC_OBJECT_INCLUDE