#include "scene.h"
#include<iostream>




void ShadingGroup::DrawOpenGL(Scene * scene){

	//glUseProgram(shading_program.getHandle()); //WORKS
	//shading_program.setUniform("camera_projection", scene->projection_matrix);
	//shading_program.setUniform("world_to_camera", scene->world_to_camera);
	//shading_program.setUniform("shadow_matrix", scene->directional_lights[0]->biased_light_view_projection);

	//shading_program.setUniform("light_direction", scene->directional_lights[0]->direction);
	//shading_program.setUniform("light_color", scene->directional_lights[0]->color);
	//shading_program.setUniform("ambient_light", scene->ambient_light);
	//
	//
	//for (int i = 0; i < graphic_objects.size(); i++){
	//	graphic_objects[i]->DrawOpenGL(scene);
	//}

	//glUseProgram(shading_program.getHandle());

	//shading_program.setUniform("camera_projection", scene->projection_matrix);
	//shading_program.setUniform("world_to_camera", scene->world_to_camera);
	//shading_program.setUniform("light_view_projection", scene->directional_lights[0]->light_view_projection);
	//shading_program.setUniform("shadow_matrix", scene->directional_lights[0]->biased_light_view_projection);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);

	//for (int i = 0; i < graphic_objects.size(); i++){
	//	graphic_objects[i]->DrawOpenGL(scene);
	//}

	glUseProgram(shading_program.getHandle());

	GLuint pass1Index = glGetSubroutineIndex(shading_program.getHandle(), GL_FRAGMENT_SHADER, "recordDepth");
	GLuint pass2Index = glGetSubroutineIndex(shading_program.getHandle(), GL_FRAGMENT_SHADER, "shadeWithShadow");


	//// First Pass: Renders the depth of the scene respect to the light

	glBindFramebuffer(GL_FRAMEBUFFER, scene->shadow_frame_buffer_handle);
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, scene->shadow_map_width,scene->shadow_map_height);

	shading_program.setUniform("shadow_map", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, scene->shadow_depth_buffer);

	shading_program.setUniform("offset_tex", 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_3D, scene->offset_buffer);

	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass1Index);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	shading_program.setUniform("eye_projection", scene->directional_lights[0]->light_projection);
	shading_program.setUniform("world_to_eye", scene->directional_lights[0]->light_view);

	shading_program.setUniform("light_projection", scene->directional_lights[0]->light_projection);
	shading_program.setUniform("world_to_light", scene->directional_lights[0]->light_view);
	shading_program.setUniform("texture_coord_transform", texture_coord_transform);

	shading_program.setUniform("light_direction", scene->directional_lights[0]->direction);
	shading_program.setUniform("light_color", scene->directional_lights[0]->color);
	shading_program.setUniform("ambient_light", scene->ambient_light);

	shading_program.setUniform("radius", scene->shadow_sampling_radius);
	shading_program.setUniform("offset_tex_size", ivec3(scene->offset_buffer_side, scene->offset_buffer_side, (scene->offset_buffer_u*scene->offset_buffer_v)/2));


	for (int i = 0; i < graphic_objects.size(); i++){
		graphic_objects[i]->DrawOpenGL(scene);
	}
	glFlush();
	glFinish();

	// Second Pass : Compute illumination using light depth field
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, scene->width, scene->height);
	glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass2Index);
	glDisable(GL_CULL_FACE);

	shading_program.setUniform("eye_projection", scene->projection_matrix);
	shading_program.setUniform("world_to_eye", scene->world_to_camera);

	for (int i = 0; i < graphic_objects.size(); i++){
		graphic_objects[i]->DrawOpenGL(scene);
	}
}

void ShadingGroup::SetupOpenGL(){


	try {
		printf("Compiling and Linking Shaders \n");
		printf("Vertex Shader : %s \n", vertex_shader);
		printf("Fragment Shader : %s \n", vertex_shader);
		shading_program.compileShader(vertex_shader);
		shading_program.compileShader(fragment_shader);
		shading_program.link();

		shading_program.printActiveAttribs();
		shading_program.printActiveUniforms();
		shading_program.printActiveUniformBlocks();
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