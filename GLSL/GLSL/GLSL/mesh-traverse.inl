#include "mesh.h"

////// Asignation Templates Definition

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
inline void Mesh<Vertex, HalfEdge, Face, Statistics>::AssignNextPreviousRelation(HalfEdge & previous_edge, HalfEdge & next_edge)
{
	previous_edge.next_edge = next_edge.edge_index;
	next_edge.previous_edge = previous_edge.edge_index;
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
inline void Mesh<Vertex, HalfEdge, Face, Statistics>::AssignVertices(HalfEdge & edge, int previous_vertex_index, int next_vertex_index)
{
	edge.previous_vertex = previous_vertex_index;
	edge.next_vertex = next_vertex_index;
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
inline void Mesh<Vertex, HalfEdge, Face, Statistics>::AssignOppositeRelation(HalfEdge & edge, HalfEdge & opposite)
{
	edge.opposite_edge = opposite.edge_index;
	opposite.opposite_edge = edge.edge_index;
}

////// Asignation Traverse Definition

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
template<typename T>
void Mesh<Vertex, HalfEdge, Face, Statistics>::FaceAssignationTraverse(FaceAssignation<T> face_assignation, T destiny)
{
	int index = 0;
	for (face_iterator face_iter = faces.begin(); face_iter != faces.end(); face_iter++)
	{
		Face & face = *face_iter;
		(this->*face_assignation)(face, index, destiny);
		index++;
	}
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
template<typename T>
void Mesh<Vertex, HalfEdge, Face, Statistics>::VertexAssignationTraverse(VertexAssignation<T> vertex_assignation, T destiny)
{
	int index = 0;
	for (vertex_iterator vertex_iter = vertices.begin(); vertex_iter != vertices.end(); vertex_iter++)
	{
		Vertex & vertex = *vertex_iter;
		(this->*vertex_assignation)(vertex, index, destiny);
		index++;
	}
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
template<typename T>
void Mesh<Vertex, HalfEdge, Face, Statistics>::EdgeAssignationTraverse(EdgeAssignation<T> edge_assignation, T destiny)
{
	int index = 0;
	for (edge_iterator edge_iter = edges.begin(); edge_iter != edges.end(); edge_iter++)
	{
		HalfEdge & edge = *edge_iter;
		(this->*edge_assignation)(edge, index, destiny);
		index++;
	}
}

////// Operator Traverse Definition

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::FaceOperatorTraverse(FaceOperator face_operator, generate_edge_list mode)
{
	for (face_iterator face_iter = faces.begin(); face_iter != faces.end(); face_iter++)
	{
		Face & face = *face_iter;
		p_edge_list face_edges;
		if (mode == GENERATE_EDGE_LIST)
		{
			face_edges.reserve(3);

			int initial_edge = face.edge;
			int edge = initial_edge;
			do{
				HalfEdge * edge_pointer = &edges[edge];
				face_edges.push_back(edge_pointer);
				edge = edge_pointer->next_edge;
			} while (edge != initial_edge);
		}
		(this->*face_operator)(face_edges, face);
	}
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::VertexOperatorTraverse(VertexOperator vertex_operator, generate_edge_list mode)
{
	for (vertex_iterator vertex_iter = vertices.begin(); vertex_iter != vertices.end(); vertex_iter++)
	{
		Vertex & vertex = *vertex_iter;
		p_edge_list vertex_edges;
		if (mode == GENERATE_EDGE_LIST)
		{
			vertex_edges.reserve(6);

			int initial_edge = vertex.edge;
			int edge = initial_edge;
			do{
				HalfEdge * edge_pointer = &edges[edge];
				vertex_edges.push_back(edge_pointer);

				edge = edges[edge_pointer->opposite_edge].next_edge;
			} while (edge != initial_edge);
		}
		(this->*vertex_operator)(vertex_edges, vertex);
	}
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::EdgeOperatorTraverse(EdgeOperator edge_operator)
{
	for (edge_iterator edge_iter = edges.begin(); edge_iter != edges.end(); edge_iter++)
	{
		HalfEdge & edge = *edge_iter;
		(this->*edge_operator)(edge);
	}
}

////// Global Filter Traverse Template

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
template<typename T>
void Mesh<Vertex, HalfEdge, Face, Statistics>::GlobalVertexFilterTraverse(VertexProperty<T> vertex_property)
{
	T * external_array = new T[vertices.size()];
	int index = 0;
	for (vertex_iterator vertex_iter = vertices.begin(); vertex_iter != vertices.end(); vertex_iter++)
	{
		Vertex & vertex = *vertex_iter;
		p_edge_list vertex_edges;
		vertex_edges.reserve(6);

		int initial_edge = vertex.edge;
		int edge = initial_edge;
		do{
			HalfEdge * edge_pointer = &edges[edge];
			vertex_edges.push_back(edge_pointer);

			edge = edges[edge_pointer->opposite_edge].next_edge;
		} while (edge != initial_edge);
		VertexFilterTraverse<T>(vertex_edges, vertex, index, external_array, OUTPUT, vertex_property);
		index++;
	}
	index = 0;
	for (vertex_iterator vertex_iter = vertices.begin(); vertex_iter != vertices.end(); vertex_iter++)
	{
		Vertex & vertex = *vertex_iter;
		p_edge_list vertex_edges;

		VertexFilterTraverse<T>(vertex_edges, vertex, index, external_array, INPUT, vertex_property);
		index++;
	}
}


//template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
//template<typename T>
//void Mesh<Vertex, HalfEdge, Face, Statistics>::GlobalFaceFilterTraverse(FaceFilterTraverse<T> face_filter_traverse, FaceProperty<T> face_property)
//{
//	T * external_array = new T[faces.size()];
//	int index = 0;
//	for (face_iterator face_iter = faces.begin(); face_iter != faces.end(); face_iter++)
//	{
//		Face & face = *face_iter;
//		p_edge_list face_edges;
//		face_edges.reserve(3);
//
//		int initial_edge = face.edge;
//		int edge = initial_edge;
//		do{
//			HalfEdge * edge_pointer = &edges[edge];
//			face_edges.push_back(edge_pointer);
//			edge = edge_pointer->next_edge;
//		} while (edge != initial_edge);
//		(this->*face_filter_traverse)(face_edges, face, index, external_array, OUTPUT, face_property);
//		index++;
//	}
//	index = 0;
//	for (face_iterator face_iter = faces.begin(); face_iter != faces.end(); face_iter++)
//	{
//		Face & face = *face_iter;
//		p_edge_list face_edges;
//		(this->*face_filter_traverse)(face_edges, face, index, external_array, INPUT, face_property);
//		index++;
//	}
//}

////// Global Filter Traverse Template

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
template<typename T>
void Mesh<Vertex, HalfEdge, Face, Statistics>::VertexFilterTraverse(p_edge_list list, Vertex & vertex, int index, T * external_array, ParameterMode mode, VertexProperty<T> vertex_property)
{
	if (mode == OUTPUT){
		double cummulative_weight = 0.f;
		T cummulative_value;
		T value;
		double weight = static_cast<double>(list.size());
		(this->*vertex_property)(vertex, value, OUTPUT);
		cummulative_value += value*weight;
		cummulative_weight += weight;
		for (int i = 0; i < list.size(); i++)
		{
			Vertex & neighbour = vertices[list[i]->next_vertex];
			weight = 1.f;
			(this->*vertex_property)(neighbour, value, OUTPUT);
			cummulative_value += value*weight;
			cummulative_weight += weight;
		}

		cummulative_value /= cummulative_weight;
		external_array[index]=cummulative_value;
	}
	else if(mode == INPUT)
	{
		(this->*vertex_property)(vertex, external_array[index], INPUT);
	}
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
int Mesh<Vertex, HalfEdge, Face, Statistics>::VertexIndexInFace(const Vertex & vertex,const Face & face)
{
	int face_vertex_index = -1;
	int initial_edge = face.edge;
	int edge = initial_edge;
	int iter = 0;
	do{
		HalfEdge * edge_pointer = &edges[edge];
		if (edge_pointer->previous_vertex == vertex.vertex_index) face_vertex_index = iter;
		edge = edge_pointer->next_edge;
		iter++;
	} while (edge != initial_edge && face_vertex_index != -1);
	return face_vertex_index;
}
