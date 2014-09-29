#include "scene.h"
#include "GL\glut.h" 

void DirectionalLight::drawOpenGL(int index)
{
	GLfloat buf[4];
	buf[0] = -direction[0];
	buf[1] = -direction[1];
	buf[2] = -direction[2];
	buf[3] = 0;
	glLightfv(GL_LIGHT0 + index, GL_POSITION, buf);

	memset(buf, 0, 4 * sizeof(GLfloat));
	glLightfv(GL_LIGHT0 + index, GL_AMBIENT, buf);
	for (int i = 0; i < 3; ++i) buf[i] = color[i];
	glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, buf);
	for (int i = 0; i < 4; ++i) buf[i] = 1.0;
	glLightfv(GL_LIGHT0 + index, GL_SPECULAR, buf);
	glLightf(GL_LIGHT0 + index, GL_SPOT_EXPONENT, 0);
	glLightf(GL_LIGHT0 + index, GL_SPOT_CUTOFF, 180);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0 + index);
}

void Camera::drawOpenGL(void)
{
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//double topBar = sin(heightAngle / 2.0);
	//glFrustum(-aspectRatio * topBar, aspectRatio * topBar, -topBar, topBar, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(position[0], position[1], position[2], position[0] + direction[0], position[1] + direction[1], position[2] + direction[2], up[0], up[1], up[2]);
}

void Camera::rotateUp(Point<3> center, float angle)
{
	right = CentralizedRotation(Point<3>(), up, right, angle);
	direction = CentralizedRotation(Point<3>(), up, direction, angle);
	position = CentralizedRotation(center, up, position, angle);
}

void Camera::rotateDirection(Point<3> center, float angle)
{
	right = CentralizedRotation(Point<3>(), direction, right, angle);
	up = CentralizedRotation(Point<3>(), direction, up, angle);
	position = CentralizedRotation(center,direction, position, angle);
}

void Camera::rotateRight(Point<3> center, float angle)
{
	up = CentralizedRotation(Point<3>(), right, up, angle);
	direction = CentralizedRotation(Point<3>(), right, direction, angle);
	position = CentralizedRotation(center, right, position, angle);
}
void Camera::moveForward(float dist){
	position += (direction*dist);
}
void Camera::moveRight(float dist){
	position += (right*dist);
}
void Camera::moveUp(float dist){
	position += (up*dist);
}

void Material::drawOpenGL(void)
{
	GLfloat buf[3] = { ambient[0], ambient[1], ambient[2] };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, buf);
	for (int i = 0; i < 3; ++i) buf[i] = diffuse[i];
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, buf);
	for (int i = 0; i < 3; ++i) buf[i] = specular[i];
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, buf);
	for (int i = 0; i < 3; ++i) buf[i] = emissive[i];
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, buf);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, specularFallOff);

	//if (tex)
	//{
	//	glEnable(GL_TEXTURE_2D);
	//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//	glBindTexture(GL_TEXTURE_2D, tex->openGLHandle);
	//}
	//else {
	//	glDisable(GL_TEXTURE_2D);
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}
}

void Scene::drawOpenGL(void){
	camera->drawOpenGL();

	for (int i = 0; i < num_graphic_objects; i++)
	{
		graphic_objects[i]->StaticDrawOpenGL();
	}

	
}

void Scene::setUpOpenGL(int cplx){

	for (int i = 0; i < num_graphic_objects; i++)
	{
		graphic_objects[i]->SetupBuffers();
	}
	
}

void Scene::setCurrentTime(double t)
{
	double velocity = 0.25f;
	t *= velocity;
	double residue = t - floor(t);
	current_time = residue;
}

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


