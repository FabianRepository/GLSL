#include "scene.h"
#include<iostream>
void ShadingGroup::DrawOpenGL(Scene * scene){

	GLuint location = glGetUniformLocation(shading_program.getHandle(), "mvp");
	if (location >= 0){
		glUniformMatrix4fv(location, 1, GL_FALSE, &scene->mvp[0][0]);
	}
	location = glGetUniformLocation(shading_program.getHandle(), "world_to_camera");
	if (location >= 0){
		glUniformMatrix4fv(location, 1, GL_FALSE, &scene->world_to_camera[0][0]);
	}
	location = glGetUniformLocation(shading_program.getHandle(), "light_direction");
	if (location >= 0){
		glUniform3f(location, scene->directional_lights[0]->direction[0], scene->directional_lights[0]->direction[1], scene->directional_lights[0]->direction[2]);
	}
	location = glGetUniformLocation(shading_program.getHandle(), "light_color");
	if (location >= 0){
		glUniform3f(location, scene->directional_lights[0]->color[0], scene->directional_lights[0]->color[1], scene->directional_lights[0]->color[2]);
	}

	glUseProgram(shading_program.getHandle());
	for (int i = 0; i < graphic_objects.size(); i++)
	{
		graphic_objects[i]->DrawOpenGL(scene);
	}
}

void ShadingGroup::SetupOpenGL(){

	try {
		shading_program.compileShader(vertex_shader);
		shading_program.compileShader(fragment_shader);
		shading_program.link();
	}
	catch (GLSLProgramException &e) {
		std::cerr << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	program_handle = shading_program.getHandle();
	for (int i = 0; i < graphic_objects.size(); i++)
	{
		graphic_objects[i]->SetupOpenGL();
		graphic_objects[i]->program_handle = program_handle;
	}
}