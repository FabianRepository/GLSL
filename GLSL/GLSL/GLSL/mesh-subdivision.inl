#include "mesh.h"
#include "time.h"
#define PI 3.1415926

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::SetLatticeMesh(const int subdivision_level)
{
	printf("CONSTRUCTING LATTICE MESH...\n");
	vertices.push_back(Vertex(0, Point<3>(0.f, 0.f, 0.f), Point<3>(0.f, 0.f, 1.f)));
	vertices.push_back(Vertex(1, Point<3>(1.f, 0.f, 0.f), Point<3>(0.f, 0.f, 1.f)));
	vertices.push_back(Vertex(2, Point<3>(0.f, 1.f, 0.f), Point<3>(0.f, 0.f, 1.f)));
	vertices.push_back(Vertex(3, Point<3>(1.f, 1.f, 0.f), Point<3>(0.f, 0.f, 1.f)));
	vertices[0].edge = 0;
	vertices[1].edge = 2;
	vertices[2].edge = 4;
	vertices[3].edge = 8;

	edges.push_back(HalfEdge(0));
	edges.push_back(HalfEdge(1));
	edges.push_back(HalfEdge(2));
	edges.push_back(HalfEdge(3));
	edges.push_back(HalfEdge(4));
	edges.push_back(HalfEdge(5));
	edges.push_back(HalfEdge(6));
	edges.push_back(HalfEdge(7));
	edges.push_back(HalfEdge(8));
	edges.push_back(HalfEdge(9));

	AssignVertices(edges[0], 0, 1);
	AssignVertices(edges[1], 1, 0);
	AssignVertices(edges[2], 1, 2);
	AssignVertices(edges[3], 2, 1);
	AssignVertices(edges[4], 2, 0);
	AssignVertices(edges[5], 0, 2);
	AssignVertices(edges[6], 1, 3);
	AssignVertices(edges[7], 3, 1);
	AssignVertices(edges[8], 3, 2);
	AssignVertices(edges[9], 2, 3);

	AssignNextPreviousRelation(edges[0], edges[2]);
	AssignNextPreviousRelation(edges[2], edges[4]);
	AssignNextPreviousRelation(edges[4], edges[0]);

	AssignNextPreviousRelation(edges[3], edges[6]);
	AssignNextPreviousRelation(edges[6], edges[8]);
	AssignNextPreviousRelation(edges[8], edges[3]);

	AssignNextPreviousRelation(edges[1], edges[5]);
	AssignNextPreviousRelation(edges[5], edges[9]);
	AssignNextPreviousRelation(edges[9], edges[7]);
	AssignNextPreviousRelation(edges[7], edges[1]);

	AssignOppositeRelation(edges[0], edges[1]);
	AssignOppositeRelation(edges[2], edges[3]);
	AssignOppositeRelation(edges[4], edges[5]);
	AssignOppositeRelation(edges[6], edges[7]);
	AssignOppositeRelation(edges[8], edges[9]);

	faces.push_back(Face(0));
	faces[0].edge = 0;
	faces.push_back(Face(1));
	faces[1].edge = 3;

	edges[0].face = 0;
	edges[2].face = 0;
	edges[4].face = 0;

	edges[3].face = 1;
	edges[6].face = 1;
	edges[8].face = 1;

	edges[1].face = -1;
	edges[5].face = -1;
	edges[9].face = -1;
	edges[7].face = -1;

	outward_oriented = true;
	for (int i = 0; i <subdivision_level; i++){ Subdivision(); }
	printf("DONE!\n");
}


template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::AddSubdivisionFaces(Face & face, int pattern_size, int * pattern) // Think in a while loop formulation!
{
	int initial_pivot_index = edges[face.edge].next_edge;
	int pivot_index = initial_pivot_index;
	for (int i = 0; i < pattern_size; i++)
	{
		HalfEdge & pivot = edges[pivot_index];
		pivot_index = CloseEdgeCycle(pivot, pattern[i]);
	}
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
HalfEdge & Mesh<Vertex, HalfEdge, Face, Statistics>::SplitHalfEdge(HalfEdge & edge, const int mid_vertex_index)// This does not set opposite relations
{
	edges.push_back(HalfEdge(edges.size()));
	HalfEdge  & next_edge = edges.back();

	next_edge.previous_vertex = mid_vertex_index;
	next_edge.next_vertex = edge.next_vertex;
	AssignNextPreviousRelation(next_edge, edges[edge.next_edge]);
	next_edge.face = edge.face;

	edge.next_vertex = mid_vertex_index;
	AssignNextPreviousRelation(edge, next_edge);

	return next_edge;
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
Vertex & Mesh<Vertex, HalfEdge, Face, Statistics>::SetMidVertex(const HalfEdge edge) // This does not set the edge pointed by the new vertex
{
	Vertex & vertex_begin = vertices[edge.previous_vertex];
	Vertex & vertex_end = vertices[edge.next_vertex];

	Point<3> mid_normal = (vertex_begin.normal + vertex_end.normal).normalize();
	Point<3> edge_direction = vertex_end.position - vertex_begin.position;
	Point<3> support_plane_normal = mid_normal.cross(edge_direction);

	Point<3> vertex_begin_normaL_projection = vertex_begin.normal - support_plane_normal*(vertex_begin.normal.dot(support_plane_normal));
	Point<3> vertex_begin_plane_velocity = support_plane_normal.cross(vertex_begin_normaL_projection);

	Point<3> vertex_end_normaL_projection = vertex_end.normal - support_plane_normal*(vertex_end.normal.dot(support_plane_normal));
	Point<3> vertex_end_plane_velocity = support_plane_normal.cross(vertex_end_normaL_projection);

	// Cardinal Bspline
	Point<3> new_position = (vertex_begin.position + vertex_end.position)*0.5f + (vertex_begin_plane_velocity - vertex_end_plane_velocity)*0.125f;

	vertices.push_back(Vertex(vertices.size(), new_position, mid_normal));
	Vertex  & vertex_mid = vertices.back();
	return vertex_mid;
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::SplitEdge(HalfEdge & edge, HalfEdge & opposite_edge)
{
	Vertex & vertex_mid = SetMidVertex(edge);
	HalfEdge & next_edge = SplitHalfEdge(edge, vertex_mid.vertex_index);
	HalfEdge & next_edge_opposite = SplitHalfEdge(opposite_edge, vertex_mid.vertex_index);

	AssignOppositeRelation(edge, next_edge_opposite);
	AssignOppositeRelation(opposite_edge, next_edge);

	vertex_mid.edge = next_edge.edge_index;
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::Subdivision()
{
	double t_start = GetTime();

	int old_num_edges = edges.size();
	int old_num_vertices = vertices.size();
	int old_num_faces = faces.size();

	edges.reserve(2 * old_num_edges + 6 * old_num_faces);
	vertices.reserve(old_num_vertices + old_num_edges);

	int edge_list_position = old_num_edges;
	int vertex_list_position = old_num_vertices;
	bool * already_processed_edges = new bool[old_num_edges]; for (int i = 0; i < old_num_edges; i++){ already_processed_edges[i] = false; }
	edge_iterator edge_iter = edges.begin();
	for (int i = 0; i < old_num_edges; i++)
	{
		HalfEdge & edge = *edge_iter;
		if (!already_processed_edges[edge.edge_index])
		{
			HalfEdge & opposite_edge = edges[edge.opposite_edge];
			SplitEdge(edge, opposite_edge);
			already_processed_edges[edge.edge_index] = true;
			already_processed_edges[opposite_edge.edge_index] = true;
			edge_list_position += 2;
			vertex_list_position++;
		}
		edge_iter++;
	}

	for (int i = 0; i < old_num_edges; i++){ if (already_processed_edges[i] == false) printf("Non Processed Edge %d \n", i); }
	delete already_processed_edges;

	int pattern_size = 3;
	int pattern[3] = { 2, 2, 2 };

	faces.reserve(4 * old_num_faces);
	int old_num_edges_x2 = old_num_edges * 2;
	face_iterator face_iter = faces.begin();
	for (int i = 0; i < old_num_faces; i++)
	{
		Face & face = *face_iter;
		AddSubdivisionFaces(face, pattern_size, pattern);
		face_iter++;
	}

	double t_end = GetTime();
	printf("Subdivision Time %f seconds\n", t_end - t_start);
}

// This does not assign opposite to the new edge 
template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
HalfEdge & Mesh<Vertex, HalfEdge, Face, Statistics>::CompleteCycle(HalfEdge & previous_edge, HalfEdge & next_edge)
{

	edges.push_back(HalfEdge(edges.size()));
	HalfEdge  & previous_to_next = edges.back();

	AssignVertices(previous_to_next, previous_edge.next_vertex, next_edge.previous_vertex);
	AssignNextPreviousRelation(previous_edge, previous_to_next);
	AssignNextPreviousRelation(previous_to_next, next_edge);

	if (previous_edge.face != next_edge.face)
	{
		printf("Face index should be equal! \n");
	}

	previous_to_next.face = previous_edge.face;
	faces[previous_edge.face].edge = previous_edge.edge_index; // To guarantee that face keep pointing to a valid element

	return previous_to_next;
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
int Mesh<Vertex, HalfEdge, Face, Statistics>::CloseEdgeCycle(HalfEdge & initial_edge, const int cycle_lenght)
{
	faces.push_back(Face(faces.size()));
	Face  & new_face = faces.back();

	int next_edge_index = initial_edge.edge_index;
	initial_edge.face = new_face.face_index;
	for (int i = 0; i < cycle_lenght - 1; i++)
	{
		next_edge_index = edges[next_edge_index].next_edge;
		edges[next_edge_index].face = new_face.face_index;
	}
	HalfEdge & final_edge = edges[next_edge_index];

	HalfEdge & previous_edge = edges[initial_edge.previous_edge];
	HalfEdge & next_edge = edges[final_edge.next_edge];

	HalfEdge  & previous_to_next = CompleteCycle(previous_edge, next_edge);
	HalfEdge  & final_to_initial = CompleteCycle(final_edge, initial_edge);

	AssignOppositeRelation(previous_to_next, final_to_initial);

	return next_edge.edge_index;
}

// Mesh Operators
template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::AddMedianTriangles(const int edge_index)
{
	vertices.reserve(vertices.size() + 1); // REMEMBER RESERVE BEFORE ANY ADD OPERATION
	edges.reserve(edges.size() + 6);
	faces.reserve(faces.size() + 2);

	HalfEdge & edge = edges[edge_index];
	HalfEdge & opposite_edge = edges[edge.opposite_edge];
	if (edge.face == -1 && opposite_edge.face == -1)
	{
		printf("Unvalid Edge : Both Exterior Faces \n");
	}

	SplitEdge(edge, opposite_edge);
	if (edge.face != -1)
	{
		HalfEdge & pivot = edges[edge.previous_edge];
		CloseEdgeCycle(pivot, 2);
	}
	if (opposite_edge.face != -1)
	{
		HalfEdge & pivot = edges[opposite_edge.previous_edge];
		CloseEdgeCycle(pivot, 2);
	}
}