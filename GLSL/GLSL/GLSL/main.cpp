#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "window2.h"
#include "mesh.h"
using std::string;

const int width = 500;
const int height = 500;
const int shadowMapWidth = 512;
const int shadowMapHeight = 512;
Window main_window;
Scene scene;

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("GLSL <mesh_name>\n");
		return 0;
	}

	//Initialize Scene
	scene.Initialize(width, height, shadowMapWidth, shadowMapHeight, 5.f, vec3(0.f, 0.f, 0.f));
	Mesh<Vertex_Base, HalfEdge_Base, Face_Base, Statistics_Base> * mesh = new Mesh<Vertex_Base, HalfEdge_Base, Face_Base, Statistics_Base>();
	mesh->ConstructMeshFromPlyFile(argv[1]);
	mesh->GeometryInitialization();

	Material * material = new Material();
	material->ambient[0] = 0.0f; material->ambient[1] = 0.0f; material->ambient[2] = 0.0f;
	material->diffuse[0] = 0.0f; material->diffuse[1] = 0.0f; material->diffuse[2] = 0.0f;
	material->specular[0] = 0.8f; material->specular[1] = 0.8f; material->specular[2] = 0.8f;
	material->emissive[0] = 0.0f; material->emissive[1] = 0.0f; material->emissive[2] = 0.0f;
	material->shininess = 10.f;

	mesh->material = material;

	//ShadingGroup * shading_group = new ShadingGroup("shaders/shadowmap.vs", "shaders/shadowmap.fs");
	ShadingGroup * shading_group = new ShadingGroup("shaders/diffuse_reflection.vert", "shaders/basic.frag");
	shading_group->graphic_objects.push_back(mesh);

	scene.shading_groups.push_back(shading_group);

	//scene.graphic_objects = new  GraphicObject *[1];
	//scene.graphic_objects[0] = mesh;
	//scene.num_graphic_objects = 1;
	
	DirectionalLight * directional_light = new DirectionalLight;
	directional_light->color = vec3(1.f, 1.f, 1.f);
	directional_light->direction = vec3(-1.f, -1.f, -1.f);

	scene.directional_lights.push_back(directional_light);

	//Initialize Window
	main_window.Initialize(&scene, width, height);

	return 0;
}