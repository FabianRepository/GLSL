#include "scene.h"
#include<iostream>
void ShadingGroup::DrawOpenGL(Scene * scene){

	//GLuint location = glGetUniformLocation(shading_program.getHandle(), "mvp");
	//if (location >= 0){
	//	glUniformMatrix4fv(location, 1, GL_FALSE, &scene->mvp[0][0]);
	//}
	//location = glGetUniformLocation(shading_program.getHandle(), "world_to_camera");
	//if (location >= 0){
	//	glUniformMatrix4fv(location, 1, GL_FALSE, &scene->world_to_camera[0][0]);
	//}
	//location = glGetUniformLocation(shading_program.getHandle(), "light_direction");
	//if (location >= 0){
	//	glUniform3f(location, scene->directional_lights[0]->direction[0], scene->directional_lights[0]->direction[1], scene->directional_lights[0]->direction[2]);
	//}
	//location = glGetUniformLocation(shading_program.getHandle(), "light_color");
	//if (location >= 0){
	//	glUniform3f(location, scene->directional_lights[0]->color[0], scene->directional_lights[0]->color[1], scene->directional_lights[0]->color[2]);
	//}

	shading_program.setUniform("mvp", scene->mvp);
	shading_program.setUniform("world_to_camera", scene->world_to_camera);
	shading_program.setUniform("light_direction", scene->directional_lights[0]->direction);
	shading_program.setUniform("light_color", scene->directional_lights[0]->color);


	GLuint pass1Index = glGetSubroutineIndex(shading_program.getHandle(), GL_FRAGMENT_SHADER, "recordDepth");
	GLuint pass2Index = glGetSubroutineIndex(shading_program.getHandle(), GL_FRAGMENT_SHADER, "shadeWithShadow");

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

	for (int i = 0; i < graphic_objects.size(); i++)
	{
		graphic_objects[i]->SetupOpenGL();
		graphic_objects[i]->glsl_program = &shading_program;
	}
}