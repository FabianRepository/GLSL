#include "window.h"
#include "mesh.h"
#include "curve.h"

#define PI 3.1415926
static int DEFAULT_COMPLEXITY = 10;
static int DEFAULT_RECurrent = 500;

void Visualization(const char * input_mesh_name , const char * input_image)
{
	int cplx = DEFAULT_COMPLEXITY;
	int width = DEFAULT_RECurrent;
	int height = DEFAULT_RECurrent;


	//Mesh<Vertex_Base, HalfEdge_Base, Face_Base,Statistics_Base> * mesh = new Mesh<Vertex_Base, HalfEdge_Base, Face_Base,Statistics_Base>();
	const int dimension = 3;
	Mesh_Advection<Vertex_Advection<dimension>, HalfEdge_Base, Face_Advection<dimension>, Statistics_Advection, dimension> * mesh = new Mesh_Advection<Vertex_Advection<dimension>, HalfEdge_Base, Face_Advection<dimension>, Statistics_Advection, dimension>();
	mesh->ConstructMeshFromPlyFile(input_mesh_name);
	//mesh->SetLatticeMesh(8);

	mesh->GeometryInitialization();
	//mesh->SetVertexValue(ALL_TO_NORMAL);
	mesh->SetVertexValue(ALL_TO_COLOR);
	
	//Image * img = ReadImagePNG(input_image);
	//Texture * tex = new Texture;
	//tex->img = img;
	//mesh->texture = tex;
	//mesh->SetVertexValue(ALL_TO_TEXTURE);

	mesh->SetVertexValue(SMOOTH_CURRENT);
	//mesh->SetVertexValue(ALL_TO_TEST);
	// delete img;
	//tex->img = 0;
	//mesh->texture = 0;

	mesh->GradientFieldComputation();
	mesh->advection_field_scale = mesh->statistics.average_edge_lenght;
	mesh->AdvectionFieldComputation();
	mesh->Advect();
	mesh->Integrate();
	
	//mesh->GeometricPositionRecompuation();
	
	//scene.mesh->InitializeLinearSystem();
	//scene.mesh->SolveCG(0.000000001f,100,true,true);
	//scene.mesh->_SetVertexColorValue(CURRENT);
	//scene.mesh->InitializeAdvectionField();
	//scene.mesh->_SetFaceNormalAndCentroid();
	//scene.mesh->_EdgeAverageLenght();
	//scene.mesh->InitializeAdvectionField();


	Scene scene;

	scene.graphic_objects = new  GraphicObject * [1];
	scene.graphic_objects[0] = mesh;
	scene.num_graphic_objects = 1;

	Camera * camera = new Camera;
	camera->heightAngle = PI / 6.f;
	camera->aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	camera->direction = Point<3>(0.f, 0.f, -1.f);
	camera->position = Point<3>(0.f, 0.f, 6.f);
	camera->right = Point<3>(1.f, 0.f, 0.f);// right=direction x up
	camera->up = Point<3>(0.f, 1.f, 0.f);
	scene.camera = camera;

	DirectionalLight * light = new DirectionalLight;
	light->color = Point<3>(1.f, 1.f, 1.f);
	light->direction = Point<3>(0.f, 0.f, -1.f);

	Light ** scene_lights = new  Light *[1];
	scene_lights[0] = light;
	scene.lights = scene_lights;
	scene.lightNum = 1;

	Material * material = new Material();
	material->index = 0;
	material->ambient = Point<3>(0.f, 0.f, 0.f);
	material->diffuse = Point<3>(1.f, 0.f, 0.f);
	material->specular = Point<3>(0.f, 0.f, 0.f);
	material->emissive = Point<3>(0.f, 0.f, 0.f);
	material->transparent = Point<3>(0.f, 0.f, 0.f);
	material->specularFallOff = 10.f;

	Material ** scene_materials = new  Material *[1];
	scene_materials[0] = material;
	scene.materials = scene_materials;
	scene.materialNum = 1;

	//if (input_mesh_name)
	//{
		//scene.mesh = ConstructMeshFromPlyFile<Vertex_Base, HalfEdge_Base, Face_Base>(input_mesh_name);
		//scene.mesh->_SetVertexColorValue(INITIAL);
		//scene.mesh->Initialization();
		//scene.mesh->FieldsComputation();
		//scene.mesh->_AdvectFaceGradient();
		//scene.mesh->SetAdvectionFieldStatistics();
		//scene.mesh->InitializeLinearSystem();
		//scene.mesh->SolveCG(0.000000001f,100,true,true);
		//scene.mesh->_SetVertexColorValue(CURRENT);
		//scene.mesh->InitializeAdvectionField();
		//scene.mesh->_SetFaceNormalAndCentroid();
		//scene.mesh->_EdgeAverageLenght();
		//scene.mesh->InitializeAdvectionField();

	//}
	//else
	//{
	//	scene.mesh = GetMesh();
	//}
	//scene.read(In.value);
	Window::View(&scene, width, height, cplx);

}


int main(int argc, char * argv[])
{
	
	if (argc !=  3)
	{
		printf("SubdivisionSurfaces <mesh_name> <image_name>\n");
		return 0;
	}
	else
	{
		Visualization(argv[1], argv[2]);
	}
	//Mesh * mesh = ConstructMeshFromPlyFile(argv[1]);
	return 0;
}


void CurveVisualization()
{
	int cplx = DEFAULT_COMPLEXITY;
	int width = DEFAULT_RECurrent;
	int height = DEFAULT_RECurrent;

	const int dimension = 2;
	Curve_Advection<Vertex_Curve_Advection<dimension>, Statistics_Curve_Advection, dimension> * curve = new Curve_Advection<Vertex_Curve_Advection<dimension>, Statistics_Curve_Advection, dimension>();
	int num_vertices = 100;
	curve->ConstructTestCurve(num_vertices);
	curve->GeometryInitialization();
	curve->GradientFieldComputation();
	curve->advection_field_scale = curve->statistics.average_edge_lenght;
	curve->AdvectionFieldComputation();
	curve->Advect();
	curve->Integrate();
	Scene scene;

	scene.graphic_objects = new  GraphicObject *[1];
	scene.graphic_objects[0] = curve;
	scene.num_graphic_objects = 1;

	Camera * camera = new Camera;
	camera->heightAngle = PI / 6.f;
	camera->aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	camera->direction = Point<3>(0.f, 0.f, -1.f);
	camera->position = Point<3>(0.f, 0.f, 6.f);
	camera->right = Point<3>(1.f, 0.f, 0.f);// right=direction x up
	camera->up = Point<3>(0.f, 1.f, 0.f);
	scene.camera = camera;

	DirectionalLight * light = new DirectionalLight;
	light->color = Point<3>(1.f, 1.f, 1.f);
	light->direction = Point<3>(0.f, 0.f, -1.f);

	Light ** scene_lights = new  Light *[1];
	scene_lights[0] = light;
	scene.lights = scene_lights;
	scene.lightNum = 1;

	Material * material = new Material();
	material->index = 0;
	material->ambient = Point<3>(0.f, 0.f, 0.f);
	material->diffuse = Point<3>(1.f, 0.f, 0.f);
	material->specular = Point<3>(0.f, 0.f, 0.f);
	material->emissive = Point<3>(0.f, 0.f, 0.f);
	material->transparent = Point<3>(0.f, 0.f, 0.f);
	material->specularFallOff = 10.f;

	Material ** scene_materials = new  Material *[1];
	scene_materials[0] = material;
	scene.materials = scene_materials;
	scene.materialNum = 1;

	//if (input_mesh_name)
	//{
	//scene.mesh = ConstructMeshFromPlyFile<Vertex_Base, HalfEdge_Base, Face_Base>(input_mesh_name);
	//scene.mesh->_SetVertexColorValue(INITIAL);
	//scene.mesh->Initialization();
	//scene.mesh->FieldsComputation();
	//scene.mesh->_AdvectFaceGradient();
	//scene.mesh->SetAdvectionFieldStatistics();
	//scene.mesh->InitializeLinearSystem();
	//scene.mesh->SolveCG(0.000000001f,100,true,true);
	//scene.mesh->_SetVertexColorValue(CURRENT);
	//scene.mesh->InitializeAdvectionField();
	//scene.mesh->_SetFaceNormalAndCentroid();
	//scene.mesh->_EdgeAverageLenght();
	//scene.mesh->InitializeAdvectionField();

	//}
	//else
	//{
	//	scene.mesh = GetMesh();
	//}
	//scene.read(In.value);
	Window::View(&scene, width, height, cplx);

}


//int main(int argc, char * argv[])
//{
//	
//	if (argc !=  1)
//	{
//		printf("SubdivisionSurfaces \n");
//		return 0;
//	}
//	else
//	{
//		CurveVisualization();
//	}
//	//Mesh * mesh = ConstructMeshFromPlyFile(argv[1]);
//	return 0;
//}
