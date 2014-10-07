#include "mesh.h"

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::SetMeshOrientation() // Use highest vertex criteria
{
	int highest_vertex_index = -1;
	double maximum_height = -DBL_MAX;
	for (vertex_iterator vertex_iter = vertices.begin(); vertex_iter != vertices.end(); vertex_iter++)
	{
		Vertex & vertex = *vertex_iter;
		if (vertex.position[2] > maximum_height)
		{
			maximum_height = vertex.position[2];
			highest_vertex_index = vertex.vertex_index;
		}
	}

	Vertex & vertex = vertices[highest_vertex_index];
	const Point<3> vertex_position = vertex.position;
	Point<3> cummulative_normal;
	int initial_edge_index = vertex.edge;

	int previous_edge_index = initial_edge_index;
	HalfEdge previous_edge = edges[previous_edge_index];
	Point<3> previous_edge_direction = vertices[previous_edge.next_vertex].position - vertex_position;
	previous_edge_direction.normalize();

	do{

		int next_edge_index = edges[previous_edge.opposite_edge].next_edge;
		HalfEdge next_edge = edges[next_edge_index];
		Point<3> next_edge_direction = vertices[next_edge.next_vertex].position - vertex_position;
		next_edge_direction.normalize();

		Point<3> cross_edges = next_edge_direction.cross(previous_edge_direction);
		cummulative_normal += cross_edges;

		previous_edge_index = next_edge_index;
		previous_edge = next_edge;
		previous_edge_direction = next_edge_direction;

	} while (previous_edge_index != initial_edge_index);

	outward_oriented = cummulative_normal[2] > 0.f ? true : false;
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::Centralize(Point<3> new_center)
{
	Point<3> current_center = Point<3>();
	for (vertex_iterator vertex_iter = vertices.begin(); vertex_iter != vertices.end(); vertex_iter++)
	{
		Vertex & vertex = *vertex_iter;
		current_center += vertex.position;
	}
	current_center /= static_cast<double>(vertices.size());
	Point<3> displacement = new_center - current_center;
	for (vertex_iterator vertex_iter = vertices.begin(); vertex_iter != vertices.end(); vertex_iter++)
	{
		Vertex & vertex = *vertex_iter;
		vertex.position += displacement;
	}
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::NormalizeByDiameter()
{
	int lowest_vertex_index = -1;
	double minimum_height = DBL_MAX;
	for (vertex_iterator vertex_iter = vertices.begin(); vertex_iter != vertices.end(); vertex_iter++)
	{
		Vertex & vertex = *vertex_iter;
		if (vertex.position[2] < minimum_height)
		{
			minimum_height = vertex.position[2];
			lowest_vertex_index = vertex.vertex_index;
		}
	}

	Point<3> lowest_vertex_position = vertices[lowest_vertex_index].position;
	double approximate_diameter = 0.f;
	for (vertex_iterator vertex_iter = vertices.begin(); vertex_iter != vertices.end(); vertex_iter++)
	{
		const Vertex vertex = *vertex_iter;
		Point<3> difference = vertex.position - lowest_vertex_position;
		double difference_norm = difference.norm();
		if (difference_norm > approximate_diameter)
		{
			approximate_diameter = difference_norm;
		}
	}

	double inv_diameter = 1.f / approximate_diameter;

	for (vertex_iterator vertex_iter = vertices.begin(); vertex_iter != vertices.end(); vertex_iter++)
	{
		Vertex & vertex = *vertex_iter;
		vertex.position -= lowest_vertex_position;
		vertex.position *= inv_diameter;
	}
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::NormalizeByExtremes()
{
	int lowest_vertex_index = -1;
	double minimum_height = DBL_MAX;

	int highest_vertex_index = -1;
	double maximum_height = -DBL_MAX;

	for (vertex_iterator vertex_iter = vertices.begin(); vertex_iter != vertices.end(); vertex_iter++)
	{
		Vertex & vertex = *vertex_iter;
		if (vertex.position[1] < minimum_height)
		{
			minimum_height = vertex.position[1];
			lowest_vertex_index = vertex.vertex_index;
		}
		if (vertex.position[1] > maximum_height)
		{
			maximum_height = vertex.position[1];
			highest_vertex_index = vertex.vertex_index;
		}
	}

	Point<3> lowest_vertex_position = vertices[lowest_vertex_index].position;
	double mesh_height = maximum_height - minimum_height;

	Point<3> new_lowest_vertex_position = Point<3>(0.f, 0.f, 0.f);
	double new_mesh_height = 1.f;
	double scale_factor = new_mesh_height / mesh_height;

	for (vertex_iterator vertex_iter = vertices.begin(); vertex_iter != vertices.end(); vertex_iter++)
	{
		Vertex & vertex = *vertex_iter;
		vertex.position -= lowest_vertex_position;
		vertex.position *= scale_factor;
		vertex.position += new_lowest_vertex_position;
	}
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
bool Mesh<Vertex, HalfEdge, Face, Statistics>::VerifyMeshCorrectness()
{
	// Edge Correctnes
	for (const_edge_iterator edge_iter = edges.begin(); edge_iter != edges.end(); edge_iter++)
	{
		const HalfEdge & edge = *edge_iter;
		if (edges[edge.opposite_edge].opposite_edge != edge.edge_index)
		{
			printf("Unsatisfied opposite relation at edge %d", edge.edge_index);
		}
		if (edges[edge.next_edge].previous_edge != edge.edge_index)
		{
			printf("Unsatisfied next edge relation at edge %d", edge.edge_index);
		}
		if (edges[edge.next_edge].previous_vertex != edge.next_vertex)
		{
			printf("Unsatisfied next vertex relation at edge %d", edge.edge_index);
		}
		if (edges[edge.next_edge].face != edge.face)
		{
			printf("Unsatisfied next face relation at edge %d", edge.edge_index);
		}
		if (edges[edge.previous_edge].next_edge != edge.edge_index)
		{
			printf("Unsatisfied previous relation at edge %d", edge.edge_index);
		}
		if (edges[edge.previous_edge].next_vertex != edge.previous_vertex)
		{
			printf("Unsatisfied previous vertex relation at edge %d", edge.edge_index);
		}
		if (edges[edge.previous_edge].face != edge.face)
		{
			printf("Unsatisfied previous face relation at edge %d", edge.edge_index);
		}
	}
	// Vertex Correctnes
	for (const_vertex_iterator vertex_iter = vertices.begin(); vertex_iter != vertices.end(); vertex_iter++)
	{
		const Vertex & vertex = *vertex_iter;
		if (edges[vertex.edge].previous_vertex != vertex.vertex_index)
		{
			printf("Unsatisfied edge relation at vertex %d", vertex.vertex_index);
		}
	}

	// Face Correctnes
	for (const_face_iterator face_iter = faces.begin(); face_iter != faces.end(); face_iter++)
	{
		const Face & face = *face_iter;
		if (edges[face.edge].face != face.face_index)
		{
			printf("Unsatisfied edge relation at face %d", face.face_index);
		}
		const HalfEdge & edge = edges[face.edge];
		if (edges[edges[edge.next_edge].next_edge].next_edge != edge.edge_index)
		{
			printf("Unsatisfied triangular edge sequence at face %d", face.face_index);
		}
		if (edges[edges[edge.previous_edge].previous_edge].previous_edge != edge.edge_index)
		{
			printf("Unsatisfied triangular edge sequence at face %d", face.face_index);
		}
	}
	return true;
}