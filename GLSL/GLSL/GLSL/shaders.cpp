#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using std::string;
#include "shaders.h"

const char* vertex_simple_shader = {
	"#version 430 core\n"
	"uniform mat4 mvp;"
	"uniform mat4 object_to_world;"
	"layout(location = 0) in vec3 vertex_position;"
	"layout(location = 1) in vec3 vertex_color;"
	"out vec3 color;"
	"void main()"
	"{"
		"color = vertex_color;"
		"gl_Position = mvp*object_to_world*vec4(vertex_position, 1);"
	"}"
};

const char* vertex_diffuse_reflection_shader = {
	"#version 430 core\n"
	"uniform mat4 mvp;"
	"uniform mat4 object_to_world;"
	"uniform mat3 normal_matrix;"
	"uniform vec4 light_position;"
	"uniform vec3 light_color;"
	"layout(location = 0) in vec3 vertex_position;"
	"layout(location = 1) in vec3 vertex_color;"
	"layout(location = 2) in vec3 vertex_normal;"
	"out vec3 color;"
	"void main()"
	"{"
	"vec3 transformed_normal = normalize(normal_matrix * vertex_normal);"
	"vec3 position_to_light = vec3(light_position);"
	"vec4 world_coord_position = object_to_world*vec4(vertex_position, 1);"
	"if(light_position[3]==1.f) {"
	"position_to_light -= vec3(world_coord_position)/world_coord_position[3];"
	"}"
	"position_to_light = normalize(position_to_light);"
	"color = light_color * vertex_color * max(dot(transformed_normal,position_to_light), 0.0);"
	"gl_Position = mvp*object_to_world*vec4(vertex_position, 1);"
	"}"
};

const char* vertex_specular_reflection_shader = {
	"#version 430 core\n"
	"uniform mat4 mvp;"
	"uniform mat4 object_to_world;"
	"uniform mat4 world_to_camera;"
	"uniform mat3 normal_matrix;"
	"uniform vec4 light_position;"
	"uniform vec3 light_color;"
	"uniform float shininess;"
	"layout(location = 0) in vec3 vertex_position;"
	"layout(location = 1) in vec3 vertex_color;"
	"layout(location = 2) in vec3 vertex_normal;"
	"out vec3 color;"
	"void main()"
	"{"
	"vec3 transformed_normal = normalize(normal_matrix * vertex_normal);"
	"vec3 position_to_light = vec3(light_position);"
	"vec4 world_coord_position = object_to_world*vec4(vertex_position, 1);"
	"if(light_position[3]==1.f) {"
	"position_to_light -= vec3(world_coord_position)/world_coord_position[3];"
	"}"
	"position_to_light = normalize(position_to_light);"
	"float spec = 0.f;"
	"float s_dot_l = dot(transformed_normal,position_to_light);"
	"if (s_dot_l > 0.f){"
		"vec4 camera_coord_position = world_to_camera*object_to_world*vec4(vertex_position, 1);"
		"vec3 position_to_camera = normalize(vec3(camera_coord_position)/camera_coord_position[3]);"
		"vec3 reflected_position_to_light = transformed_normal*2.f*s_dot_l - position_to_light;"
		"spec = pow(max(dot(position_to_camera, reflected_position_to_light), 0.0), shininess);"
	"}"
	"color = light_color * vertex_color * spec;"
	"gl_Position = mvp*object_to_world*vec4(vertex_position, 1);"
	"}"
};

const char* fragment_simple_shader = {
	"#version 430 core\n"
	"in vec3 color;"
	"out vec4 fColor;"
	"void main()"
	"{"
	"    fColor = vec4(color,1);"
	"}"
};

int ShaderTools::SetShaderSource(const char *  shader_file, GLuint & shader)
{
	const GLchar * shaderCode;
	if (strcmp(shader_file, "vertex_simple_shader") == 0){
		shaderCode = vertex_simple_shader;
	}
	else if (strcmp(shader_file, "vertex_diffuse_reflection_shader") == 0){
		shaderCode = vertex_diffuse_reflection_shader;
	}
	else if (strcmp(shader_file, "vertex_specular_reflection_shader") == 0){
		shaderCode = vertex_specular_reflection_shader;
	}
	else if (strcmp(shader_file, "fragment_simple_shader") == 0){
		shaderCode = fragment_simple_shader;
	}

	else{
		std::ifstream inFile(shader_file);
		if (!inFile){
			printf("Error opening file: %s", shader_file);
			return 0;
		}

		std::stringstream code;
		code << inFile.rdbuf();
		inFile.close();
		string codeStr(code.str());

		shaderCode = codeStr.c_str();
	}
	const GLchar* codeArray[] = { shaderCode };
	glShaderSource(shader, 1, codeArray, NULL);
	return 1;
}


int ShaderTools::CompileShader(const char *  shader_file, GLuint  & shader)
{
	// Create the shader object
	if (SetShaderSource(shader_file, shader) == 0){
		return 0;
	}
	glCompileShader(shader);
	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (GL_FALSE == result)
	{
		printf("Shader compilation failed!\n");
		GLint logLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

		if (logLen > 0)
		{
			char * log = new char[logLen];

			GLsizei written;
			glGetShaderInfoLog(shader, logLen, &written, log);

			printf("Shader log:\n%s", log);
			delete[] log;
		}
	}
	return 1;
}

const char * ShaderTools::getTypeString(GLenum type) {
	// There are many more types than are covered here, but
	// these are the most common in these examples.
	switch (type) {
	case GL_FLOAT:
		return "float";
	case GL_FLOAT_VEC2:
		return "vec2";
	case GL_FLOAT_VEC3:
		return "vec3";
	case GL_FLOAT_VEC4:
		return "vec4";
	case GL_DOUBLE:
		return "double";
	case GL_INT:
		return "int";
	case GL_UNSIGNED_INT:
		return "unsigned int";
	case GL_BOOL:
		return "bool";
	case GL_FLOAT_MAT2:
		return "mat2";
	case GL_FLOAT_MAT3:
		return "mat3";
	case GL_FLOAT_MAT4:
		return "mat4";
	default:
		return "?";
	}
}

int ShaderTools::InitializeShaders(const char *  vertex_shader_file,const char *  fragment_shader_file, GLuint & programHandle)
{
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	if (0 == vertShader) {
		printf("Error creating vertex shader.\n");
		return 0;
	}
	if (CompileShader(vertex_shader_file, vertShader) == 0){
		printf("Error compiling vertex shader.\n");
		return 0;
	}
	else{
		printf("Succesfull vertex shader compilation.\n");
	}

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (0 == fragShader) {
		printf("Error creating fragment shader.\n");
	}
	if (CompileShader(fragment_shader_file, fragShader) == 0){
		printf("Error compiling fragment shader.\n");
		return 0;
	}
	else{
		printf("Succesfull fragment shader compilation.\n");
	}

	programHandle = glCreateProgram();

	if (0 == programHandle)
	{
		printf("Error creating program object.\n");
		return 0;
	}

	glAttachShader(programHandle, vertShader);
	glAttachShader(programHandle, fragShader);

	glLinkProgram(programHandle);
	GLint status;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &status);
	if (GL_FALSE == status) {

		printf("Failed to link shader program!\n");
		GLint logLen;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char * log = new char[logLen];
			GLsizei written;
			glGetProgramInfoLog(programHandle, logLen, &written, log);
			printf("Program log: \n%s", log);
			delete[] log;
		}
	}
	else{
		printf("Succesfull program linking.\n");
	}
	GLint numAttribs;
	glGetProgramInterfaceiv(programHandle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs);
	GLenum attrib_properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };

	printf("Active attributes:\n");
	for (int i = 0; i < numAttribs; ++i) {
		GLint results[3];
		glGetProgramResourceiv(programHandle, GL_PROGRAM_INPUT, i, 3, attrib_properties, 3, NULL, results);

		GLint nameBufSize = results[0] + 1;
		char * name = new char[nameBufSize];
		glGetProgramResourceName(programHandle, GL_PROGRAM_INPUT, i, nameBufSize, NULL, name);
		printf("%-5d %s (%s)\n", results[2], name, getTypeString(results[1]));
		delete[] name;
	}

	GLint numUniforms = 0;
	glGetProgramInterfaceiv(programHandle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
	GLenum uniform_properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };

	printf("Active uniforms:\n");
	for (int i = 0; i < numUniforms; ++i) {
		GLint results[4];
		glGetProgramResourceiv(programHandle, GL_UNIFORM, i, 4, uniform_properties, 4, NULL, results);
		if (results[3] != -1)
			continue;       // Skip uniforms in blocks 
		GLint nameBufSize = results[0] + 1;
		char * name = new char[nameBufSize];
		glGetProgramResourceName(programHandle, GL_UNIFORM, i, nameBufSize, NULL, name);
		printf("%-5d %s (%s)\n", results[2], name, getTypeString(results[1]));
		delete[] name;
	}
	return 1;
}
