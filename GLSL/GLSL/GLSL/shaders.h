#include "GL\glew.h"

class ShaderTools{
public:
static int SetShaderSource(const char *  shader_file, GLuint & shader);
static int CompileShader(const char *  shader_file, GLuint  & shader);
static int InitializeShaders(const char *  vertex_shader_file,const char *  fragment_shader_file, GLuint & programHandle);
static const char * getTypeString(GLenum type);
};