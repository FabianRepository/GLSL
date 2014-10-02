#include "scene.h" 

//void DirectionalLight::drawOpenGL(int index)
//{
//	GLfloat buf[4];
//	buf[0] = -direction[0];
//	buf[1] = -direction[1];
//	buf[2] = -direction[2];
//	buf[3] = 0;
//	glLightfv(GL_LIGHT0 + index, GL_POSITION, buf);
//
//	memset(buf, 0, 4 * sizeof(GLfloat));
//	glLightfv(GL_LIGHT0 + index, GL_AMBIENT, buf);
//	for (int i = 0; i < 3; ++i) buf[i] = color[i];
//	glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, buf);
//	for (int i = 0; i < 4; ++i) buf[i] = 1.0;
//	glLightfv(GL_LIGHT0 + index, GL_SPECULAR, buf);
//	glLightf(GL_LIGHT0 + index, GL_SPOT_EXPONENT, 0);
//	glLightf(GL_LIGHT0 + index, GL_SPOT_CUTOFF, 180);
//
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0 + index);
//}

void Scene::SetWorldToCamera()
{
	world_to_camera = glm::lookAt(camera.position, camera.position + camera.direction, camera.up);
}

void Scene::Initialize(const int width, const int height, const int p_shadow_map_width, const int p_shadow_map_height, const float p_radius, const vec3 p_center)
{
	camera.heightAngle = PI / 6.f;
	camera.aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	camera.direction = vec3(0.f, 0.f, -1.f);
	camera.position = vec3(0.f, 0.f, 6.f);
	camera.right = vec3(1.f, 0.f, 0.f);// right=direction x up
	camera.up = vec3(0.f, 1.f, 0.f);

	radius = p_radius;
	center = p_center;

	shadow_map_width = p_shadow_map_width;
	shadow_map_height = p_shadow_map_height;

	//num_graphic_objects = 0;
}

void Camera::rotateUp(vec3 center, float angle)
{
	right = Point3toVec3(CentralizedRotation(Point<3>(), Vec3toPoint3(up), Vec3toPoint3(right), angle));
	direction = Point3toVec3(CentralizedRotation(Point<3>(), Vec3toPoint3(up), Vec3toPoint3(direction), angle));
	position = Point3toVec3(CentralizedRotation(Vec3toPoint3(center), Vec3toPoint3(up), Vec3toPoint3(position), angle));
}
//
//void Camera::rotateDirection(Point<3> center, float angle)
//{
//	right = CentralizedRotation(Point<3>(), direction, right, angle);
//	up = CentralizedRotation(Point<3>(), direction, up, angle);
//	position = CentralizedRotation(center,direction, position, angle);
//}
//
void Camera::rotateRight(vec3 center, float angle)
{
	up = Point3toVec3(CentralizedRotation(Point<3>(), Vec3toPoint3(right), Vec3toPoint3(up), angle));
	direction = Point3toVec3(CentralizedRotation(Point<3>(), Vec3toPoint3(right), Vec3toPoint3(direction), angle));
	position = Point3toVec3(CentralizedRotation(Vec3toPoint3(center), Vec3toPoint3(right), Vec3toPoint3(position), angle));
}
//void Camera::moveForward(float dist){
//	position += (direction*dist);
//}
void Camera::moveRight(float dist){
	position += (right*dist);
}
void Camera::moveUp(float dist){
	position += (up*dist);
}
//
//void Material::drawOpenGL(void)
//{
//	GLfloat buf[3] = { ambient[0], ambient[1], ambient[2] };
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, buf);
//	for (int i = 0; i < 3; ++i) buf[i] = diffuse[i];
//	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, buf);
//	for (int i = 0; i < 3; ++i) buf[i] = specular[i];
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, buf);
//	for (int i = 0; i < 3; ++i) buf[i] = emissive[i];
//	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, buf);
//	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, specularFallOff);
//
//	//if (tex)
//	//{
//	//	glEnable(GL_TEXTURE_2D);
//	//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//	//	glBindTexture(GL_TEXTURE_2D, tex->openGLHandle);
//	//}
//	//else {
//	//	glDisable(GL_TEXTURE_2D);
//	//	glBindTexture(GL_TEXTURE_2D, 0);
//	//}
//}
//

void Scene::SetMVP(){
	SetProjectionMatrix();
	SetWorldToCamera();
	mvp = projection_matrix * world_to_camera;

}

void Scene::DrawOpenGL(){
	SetMVP();
	//lights[0]->DrawOpenGL(this);
	for (int i = 0; i < shading_groups.size(); i++)
	{
		shading_groups[i]->DrawOpenGL(this);
	}
}

void Scene::SetShadowBuffer()
{
	GLfloat border[] = { 1.0f, 0.0f, 0.0f, 0.0f };
	// The depth buffer texture
	GLuint depthTex;
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, shadow_map_width, shadow_map_height);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

	// Assign the depth buffer texture to texture channel 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTex);

	// Create and set up the FBO
	glGenFramebuffers(1, &shadow_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow_buffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, depthTex, 0);

	GLenum drawBuffers[] = { GL_NONE };
	glDrawBuffers(1, drawBuffers);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result == GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer is complete.\n");
	}
	else {
		printf("Framebuffer is not complete.\n");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::SetupOpenGL()
{
	//ShaderTools::InitializeShaders("vertex_simple_shader", "fragment_simple_shader", programHandle);
	
	//ShaderTools::InitializeShaders("vertex_specular_reflection_shader", "fragment_simple_shader", programHandle);
	//lights[0]->SetupOpenGL();
	for (int i = 0; i < shading_groups.size(); i++){
		shading_groups[i]->SetupOpenGL();
	}
	//SetShadowBuffer();
}
//
//void Scene::setCurrentTime(double t)
//{
//	double velocity = 0.25f;
//	t *= velocity;
//	double residue = t - floor(t);
//	current_time = residue;
//}

//void Mesh::drawOpenGL()
//{
//
//	static GLfloat vertices[] = { 1.f, 0.f, 0.f, 0.f, 0.f, 0.f,
//							      1.f, 0.f, 0.f, 1.f, 0.f, 0.f,
//								  1.f, 0.f, 0.f, 0.f, 1.f, 0.f };
//	//glEnableClientState(GL_VERTEX_ARRAY);
//	//glVertexPointer(3, GL_FLOAT, 0, vertices);
//
//	glInterleavedArrays(GL_C3F_V3F, 0, vertices);
//
//	//static GLubyte indices[] = { 0, 1, 1 , 2, 2 , 0 };
//	glDrawArrays(GL_TRIANGLES, 0, 3);
//
//}

void Scene::SetProjectionMatrix()
{
	vec3 center(0.f, 0.f, 0.f);
	float radius = 5.f;
	float d = radius + glm::length((camera.position - center));
	projection_matrix = glm::perspective(float(camera.heightAngle*180.0f / PI), camera.aspectRatio, .1f*d, 2.f * d);
}

//void TestObject::SetupOpenGL()
//{
//	
//	float positionData[] = {
//			-0.5f, -0.5f, 0.0f,
//			0.5f, -0.5f, 0.0f,
//			0.5f,  0.5f, 0.0f };
//	float colorData[] = {
//			0.0f,  0.0f, 0.0f,
//			1.0f,  0.0f, 0.0f,
//			1.0f,  1.0f, 0.0f};
//		
//	// Create and populate the buffer objects
//	GLuint vboHandles[2];
//	glGenBuffers(2, vboHandles);
//	GLuint positionBufferHandle = vboHandles[0];
//	GLuint colorBufferHandle = vboHandles[1];
//		
//	// Populate the position buffer
//	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
//	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);
//	// Populate the color buffer
//	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
//	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);
//			
//	glGenVertexArrays(1,&vaoHandle);
//	glBindVertexArray(vaoHandle);
//		
//	// Enable the vertex attribute arrays
//	glEnableVertexAttribArray(0);  // Vertex position
//	//glEnableVertexAttribArray(1);  // Vertex color
//		
//	glBindVertexBuffer(0, positionBufferHandle, 0, sizeof(GLfloat)* 3);
//	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
//	glVertexAttribBinding(0, 0);
//		
//	//glBindVertexBuffer(1, colorBufferHandle, 0, sizeof(GLfloat)* 3);
//	//glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
//	//glVertexAttribBinding(1, 1);
//
//	glVertexAttrib3f(1, 1.f, 1.f, 0.f);
//
//	object_to_world = mat4(1.f);
//}
//
//void TestObject::DrawOpenGL(const mat4 mvp){
//	glUseProgram(programHandle);
//
//	mat4 geometry_transform = mvp*object_to_world;
//	GLuint location = glGetUniformLocation(programHandle, "geometry_transform");
//	if (location >= 0){
//		glUniformMatrix4fv(location, 1, GL_FALSE, &geometry_transform[0][0]);
//	}
//	glBindVertexArray(vaoHandle);
//	glDrawArrays(GL_TRIANGLES, 0, 3);
//}

void PointLight::rotateX(float angle)
{
	Point<3> previous_pos = Point<3>(position[0], position[1], position[2]);
	Point<3> new_pos = CentralizedRotation(Point<3>(), Point<3>(1.f, 0.f, 0.f), previous_pos, angle);
	position[0] = new_pos[0];
	position[1] = new_pos[1]; 
	position[2] = new_pos[2];
}

void PointLight::rotateY(float angle)
{
	Point<3> previous_pos = Point<3>(position[0], position[1], position[2]);
	Point<3> new_pos = CentralizedRotation(Point<3>(), Point<3>(0.f, 1.f, 0.f), previous_pos, angle);
	position[0] = new_pos[0];
	position[1] = new_pos[1];
	position[2] = new_pos[2];
}

void PointLight::SetLightViewProjection(vec3 scene_center, float scene_radius)
{
	mat4 light_view = glm::lookAt(position, scene_center, vec3(0.f, 1.f, 0.f));
	mat4 light_proj = glm::frustum(-scene_radius, scene_radius, -scene_radius, scene_radius, 0.01f*scene_radius, 2.f*scene_radius);

	biased_light_view_projection = shadowBias * light_proj * light_view;

}

void DirectionalLight::rotateX(float angle)
{
	Point<3> previous_pos = Point<3>(direction[0], direction[1], direction[2]);
	Point<3> new_pos = CentralizedRotation(Point<3>(), Point<3>(1.f, 0.f, 0.f), previous_pos, angle);
	direction[0] = new_pos[0];
	direction[1] = new_pos[1];
	direction[2] = new_pos[2];
}

void DirectionalLight::rotateY(float angle)
{
	Point<3> previous_pos = Point<3>(direction[0], direction[1], direction[2]);
	Point<3> new_pos = CentralizedRotation(Point<3>(), Point<3>(0.f, 1.f, 0.f), previous_pos, angle);
	direction[0] = new_pos[0];
	direction[1] = new_pos[1];
	direction[2] = new_pos[2];
}

void DirectionalLight::SetLightViewProjection(vec3 scene_center, float scene_radius)
{
	vec3 artificial_position = glm::normalize(-direction)*(float)scene_radius;
	mat4 light_view = glm::lookAt(artificial_position, scene_center, vec3(0.f, 1.f, 0.f));
	mat4 light_proj = glm::ortho(-scene_radius, scene_radius, -scene_radius, scene_radius, 0.01f*scene_radius, 2.f*scene_radius);

	biased_light_view_projection = shadowBias * light_proj * light_view;

}


//void Light::SetupOpenGL()
//{
//	ShaderTools::InitializeShaders("vertex_simple_shader", "fragment_simple_shader", programHandle);
//
//	glGenBuffers(1, &position_buffer);
//	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
//	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
//
//	glGenVertexArrays(1, &vaoHandle);
//	glBindVertexArray(vaoHandle);
//
//	glEnableVertexAttribArray(0);  // Vertex position
//	glBindVertexBuffer(0, position_buffer, 0, sizeof(GLfloat)* 3);
//	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
//	glVertexAttribBinding(0, 0);
//
//	glVertexAttrib3f(1, 0.f, 0.f, 0.f);
//
//}
//
//void Light::DrawOpenGL(Scene * scene){
//
//	glBindVertexArray(vaoHandle);
//	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
//	vec4 temp = scene->world_to_camera*vec4(position);
//	vec3 position_to_camera = vec3(temp) / temp[3];
//	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), &position_to_camera[0], GL_STATIC_DRAW);
//	glVertexPointer(3, GL_DOUBLE, 0, 0);
//	glPointSize(3.f);
//	glEnable(GL_POINT_SMOOTH);
//	glColor3f(0.f, 0.f, 0.f);
//	glUseProgram(programHandle);
//	glDrawArrays(GL_POINTS, 0,1);
//
//}