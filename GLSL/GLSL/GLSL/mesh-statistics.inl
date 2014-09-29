#include "mesh.h"

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::EdgeCummulativeLenght(HalfEdge & edge, int index, double & cummulative_value)
{
	cummulative_value += (vertices[edge.next_vertex].position - vertices[edge.previous_vertex].position).norm();
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::SetBasicMeshStatistics()
{
	double edge_cummulative_length_value = 0.f;
	EdgeAssignation<double &> edge_cummulative_lenght = &Mesh::EdgeCummulativeLenght;
	EdgeAssignationTraverse<double &>(edge_cummulative_lenght, edge_cummulative_length_value);
	statistics.average_edge_lenght = edge_cummulative_length_value / static_cast<double>(edges.size());
	printf("Average Edge Length = %f \n", statistics.average_edge_lenght);
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::PoorAspectFacesCount(Face & face, int index, int & count)
{
	if (face.poor_aspect_ratio) count++;
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::SetPoorAspectFacesCount()
{
	int count = 0;
	FaceAssignation<int &> poor_aspect_count = &Mesh<Vertex, HalfEdge, Face, Statistics>::PoorAspectFacesCount;
	FaceAssignationTraverse<int &>(poor_aspect_count, count);
	statistics.poor_aspect_faces = count;
	printf("Poor Aspect Faces %d of %d \n", statistics.poor_aspect_faces, faces.size());
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::PoorAspectVerticesCount(Vertex & vertex, int index, int & count)
{
	if (vertex.poor_aspect_neighbourhood) count++;
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::SetPoorAspectVerticesCount()
{
	int count = 0;
	VertexAssignation<int &> poor_aspect_count = &Mesh<Vertex, HalfEdge, Face, Statistics>::PoorAspectVerticesCount;
	VertexAssignationTraverse<int &>(poor_aspect_count, count);
	statistics.poor_aspect_vertices = count;
	printf("Poor Aspect Vertices %d of %d \n", statistics.poor_aspect_vertices, vertices.size());
}

