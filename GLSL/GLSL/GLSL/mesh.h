
#ifndef MESH_INCLUDED
#define MESH_INCLUDED
#include "graphic-object.h"

class Vertex_Base{
public:
	Vertex_Base(int p_index, Point<3> p_position, Point<3> p_normal, Point<3> p_color = Point<3>(0.8f, 0.8f, 1.f)){
		vertex_index = p_index;
		position = p_position;
		normal = p_normal;
		color = p_color;
		edge = -1;
		poor_aspect_neighbourhood = false;
	}
	int vertex_index;
	Point<3> position;
	Point<3> normal;
	Point<3> color;
	bool poor_aspect_neighbourhood;
	int edge; // Pointing out
};

class HalfEdge_Base{
public:
	HalfEdge_Base(int p_index)
	{
		edge_index = p_index;
		previous_vertex = -1;
		next_vertex = -1;
		previous_edge = -1;
		next_edge = -1;
		opposite_edge = -1;
		face = -2;// -1 is reserved for the exterior face
		laplacian_weight = 1.f;
	}
	int edge_index;
	int previous_vertex;
	int next_vertex;
	int previous_edge;
	int next_edge;
	int opposite_edge;
	int face;
	double laplacian_weight;
};

class Face_Base{
public:
	Face_Base(int p_index)
	{
		face_index = p_index;
		edge = -1;
		poor_aspect_ratio = false;
	}
	int face_index;
	double area;
	double angles[3];
	Point<3> normal;
	bool poor_aspect_ratio;
	int edge;// Any edge of the face
	
};

enum generate_edge_list{ GENERATE_EDGE_LIST, DO_NOT_GENERATE_EDGE_LIST};

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
class Mesh : public GraphicObject {
public:
	Mesh(){}

	typedef std::vector<Face>  face_list;
	typedef std::vector<Vertex>  vertex_list;
	typedef std::vector<HalfEdge>  edge_list;

	typedef typename std::vector<Face>::iterator face_iterator;
	typedef typename std::vector<Vertex>::iterator vertex_iterator;
	typedef typename std::vector<HalfEdge>::iterator edge_iterator;

	typedef typename std::vector<Face>::const_iterator const_face_iterator;
	typedef typename std::vector<Vertex>::const_iterator const_vertex_iterator;
	typedef typename std::vector<HalfEdge>::const_iterator const_edge_iterator;

	typedef std::vector<Vertex *>  p_vertex_list;
	typedef std::vector<HalfEdge *>  p_edge_list;
	typedef std::vector<Face *>  p_face_list;

	face_list faces;
	vertex_list vertices;
	edge_list edges;
	Statistics statistics;
	Texture * texture;

	bool outward_oriented;
	//void drawOpenGL(double time);
	//void AnimateMesh(const double time, void(*VertexTransformation)(Vertex &, const double));
	//void ComputeVertexNormalFromPosition();
	void SetMeshOrientation();
	void NormalizeByDiameter();
	void NormalizeByExtremes();


	GLuint vertex_position_buffer;
	GLuint vertex_normal_buffer;
	//GLuint vertex_color_buffer;
	GLuint edge_index_buffer;
	GLuint face_index_buffer;

	// Subdivision Operators
	void AddSubdivisionFaces(Face & face, int pattern_size, int * pattern);
	void SplitEdge(HalfEdge & edge, HalfEdge & opposite_edge);
	HalfEdge & SplitHalfEdge(HalfEdge & edge, const int mid_vertex_index);
	Vertex & SetMidVertex(const HalfEdge edge);
	void Subdivision();
	HalfEdge & CompleteCycle(HalfEdge & previous_edge, HalfEdge & next_edge);
	int CloseEdgeCycle(HalfEdge & initial_edge, const int cycle_lenght);
	// Mesh Operators
	void Split_Edge_Operator(HalfEdge & edge);
	void AddVertex(Vertex vertex);
	void AddMedianTriangles(const int edge_index);

	// Operator Template
	using FaceOperator = void (Mesh<Vertex, HalfEdge, Face, Statistics>::*)(p_edge_list, Face &);
	using VertexOperator = void (Mesh<Vertex, HalfEdge, Face, Statistics>::*)(p_edge_list, Vertex &);
	using EdgeOperator = void(Mesh<Vertex, HalfEdge, Face, Statistics>::*)(HalfEdge &);

	// Assignation Template
	template<typename T> using VertexAssignation = void (Mesh<Vertex, HalfEdge, Face, Statistics>::*)(Vertex &, int, T);
	template<typename T> using FaceAssignation = void (Mesh<Vertex, HalfEdge, Face, Statistics>::*)(Face &, int, T);
	template<typename T> using EdgeAssignation = void (Mesh<Vertex, HalfEdge, Face, Statistics>::*)(HalfEdge &, int, T);

	// Assignation Template Realizations
	void VertexPositionAssignation(Vertex & vertex, int index, double * external_array);
	void VertexNormalAssignation(Vertex & vertex, int index, double * external_array);
	void VertexColorAssignation(Vertex & vertex, int index, double * external_array);


	//Property Template
	template<typename T> using VertexProperty = void (Mesh<Vertex, HalfEdge, Face, Statistics>::*)(Vertex &, T &, ParameterMode mode);
	//template<typename T> using FaceProperty = T(Mesh<Vertex, HalfEdge, Face, Statistics>::*)(Face &, T &, ParameterMode mode);

	//void FaceNormalAssignation(Face & face, int index, double * external_array);
	//void FaceNormalSegmentAssignation(Face & face, int index, double * external_array);
	//void FaceCentroidAssignation(Face & face, int index, double * external_array);

	// Mesh Traversing Template
	template<typename T> void FaceAssignationTraverse(FaceAssignation<T> face_assignation, T destiny);
	template<typename T> void VertexAssignationTraverse(VertexAssignation<T> vertex_assignation, T destiny);
	template<typename T> void EdgeAssignationTraverse(EdgeAssignation<T> edge_assignation, T destiny);

	// Mesh Traversing Template
	void FaceOperatorTraverse(FaceOperator face_operator, generate_edge_list mode);
	void VertexOperatorTraverse(VertexOperator vertex_operator, generate_edge_list mode);
	void EdgeOperatorTraverse(EdgeOperator edge_operator);

	// Mesh Traversing Template
	template<typename T> void VertexFilterTraverse(p_edge_list list, Vertex & vertex, int index, T * external_array, ParameterMode mode, VertexProperty<T> vertex_property);
	//template<typename T> void FaceFilterTraverse(p_edge_list list, Face & face, int index, T * external_array, ParameterMode mode, FaceProperty<T> face_property);

	// Mesh Traversing Template
	//template<typename T> void GlobalFaceFilterTraverse(FaceFilterTraverse<T> face_filter, FaceProperty<T> face_property);
	template<typename T> void GlobalVertexFilterTraverse(VertexProperty<T> vertex_property);

	void SmoothVertexColor();
	void VertexColorProperty(Vertex & vertex, Point<3> & color_value, ParameterMode mode);

	//// Filtering Templates
	//using WeightFunction = double (Mesh::*)(p_edge_list, int);
	//template<typename T> using ValueFunction = T (Mesh::*)(p_edge_list, int);
	//template<typename T> T Filtering(p_edge_list list, WeightFunction weight_func, ValueFunction<T> value_func);
	//
	////Value Realizations
	//Point<3> Value_VertexPosition(p_edge_list list, int index);

	////Weight Realizations
	//double Weigth_Uniform(p_edge_list list, int index);

	//// Vertex Operations
	//void Vertex_Filtering(p_edge_list list, Vertex & vertex);
	//void Vertex_Update(p_edge_list list, Vertex & vertex);
	//void Smoothing();

	void SetVertexColorValueToTexture(p_edge_list list, Vertex & vertex);
	void _SetVertexColorValueToTexture();

	// Statistical Methods
	void EdgeCummulativeLenght(HalfEdge & edge, int index, double & cummulative_value);
	void SetBasicMeshStatistics();

	/////////////////////////////
	void SetupOpenGL();
	void SetupVertexPositionBuffer();
	void SetupVertexNormalBuffer();
	//void SetupVertexColorBuffer();
	void SetupEdgeIndexBuffer();
	void SetupFaceIndexBuffer();
	//void SetupFaceNormalBuffer();
	//void SetupFaceNormalSegmentBuffer();
	//void SetupFaceCentroidBuffer();
	void DrawOpenGL(Scene * scene);
	void HandleKeyboardEvent(Prompt  & prompt){}

	//////////////////////////// Output

	//void writeVertices(const vertex_list & vertices, std::ofstream & plyFile);
	//void writeFaces(const face_list & faces, const edge_list & edges, std::ofstream & plyFile);
	//void writePlyFile(const Mesh & mesh, const char * fileName);

	void ConstructMeshFromPlyFile(const char * file_name);
	void SetLatticeMesh(const int subdivision_level);
	///////////////////////////////
	
	bool VerifyMeshCorrectness();

	inline void AssignNextPreviousRelation(HalfEdge & previous_edge, HalfEdge & next_edge);
	inline void AssignVertices(HalfEdge & edge, int previous_vertex_index, int next_vertex_index);
	inline void AssignOppositeRelation(HalfEdge & edge, HalfEdge & opposite);


	void GeometryInitialization();
	void SetVertexNormal(p_edge_list list, Vertex & vertex);
	void SetFaceGeometry(p_edge_list list, Face & face);

	void PoorAspectFacesCount(Face & face, int index, int & count);
	void SetPoorAspectFacesCount();
	void PoorAspectVerticesCount(Vertex & vertex, int index, int & count);
	void SetPoorAspectVerticesCount();

	int VertexIndexInFace(const Vertex & vertex, const Face & face);
};


#include "mesh-util.inl"
#include "mesh-io.inl"
#include "mesh-traverse.inl"
#include "mesh-rendering.inl"
#include "mesh-subdivision.inl"
#include "mesh-filtering.inl"
#include "mesh-statistics.inl"

#endif // MESH_INCLUDED

