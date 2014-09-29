#include "mesh.h"

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::SmoothVertexColor()
{
	VertexProperty<Point<3>> vertex_color_filter = &Mesh<Vertex, HalfEdge, Face, Statistics>::VertexColorProperty;
	GlobalVertexFilterTraverse<Point<3>>(vertex_color_filter);
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::VertexColorProperty(Vertex & vertex, Point<3> & color_value, ParameterMode mode)
{
	if (mode == OUTPUT)
	{
		color_value = vertex.color;
	}
	else if (mode == INPUT)
	{
		vertex.color= color_value;
	}
}

//template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
//Point<3> Mesh<Vertex, HalfEdge, Face, Statistics>::Value_VertexPosition(p_edge_list list, int index)
//{
//	return vertices[list[index]->next_vertex].position;
//}
//
//template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
//double  Mesh<Vertex, HalfEdge, Face, Statistics>::Weigth_Uniform(p_edge_list list, int index)
//{
//	return 1.f;
//}
//
//template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
//void Mesh<Vertex, HalfEdge, Face, Statistics>::Vertex_Filtering(p_edge_list list, Vertex & vertex)
//{
//	WeightFunction weight_func = &Mesh<Vertex, HalfEdge, Face, Statistics>::Weigth_Uniform;
//	ValueFunction<Point<3>> value_func = &Mesh<Vertex, HalfEdge, Face, Statistics>::Value_VertexPosition;
//
//	vertex.tmp_position = Filtering<Point<3>>(list, weight_func, value_func);
//}
//
//template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
//void Mesh<Vertex, HalfEdge, Face, Statistics>::Vertex_Update(p_edge_list list, Vertex & vertex)
//{
//	vertex.position = vertex.tmp_position;
//}
//
//template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
//void Mesh<Vertex, HalfEdge, Face, Statistics>::Smoothing()
//{
//	VertexOperator vertex_operator = &Mesh<Vertex, HalfEdge, Face, Statistics>::Vertex_Filtering;
//	VertexOperatorTraverse(vertex_operator, GENERATE_EDGE_LIST);
//	vertex_operator = &Mesh<Vertex, HalfEdge, Face, Statistics>::Vertex_Update;
//	VertexOperatorTraverse(vertex_operator, DO_NOT_GENERATE_EDGE_LIST);
//}
//
//////// Filtering Template Definition
//
//template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
//template<typename T>
//T Mesh<Vertex, HalfEdge, Face, Statistics>::Filtering(p_edge_list list, WeightFunction weight_func, ValueFunction<T> value_func)
//{
//	double cummulative_weight = 0.f;
//	T cummulative_value;
//	for (int i = 0; i < list.size(); i++)
//	{
//		double weight = (this->*weight_func)(list, i);
//		T value = (this->*value_func)(list, i);
//
//		cummulative_value += value*weight;
//		cummulative_weight += weight;
//	}
//	double inv_weight = 1.f / cummulative_weight;
//	cummulative_value *= inv_weight;
//	return cummulative_value;
//}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::SetFaceGeometry(p_edge_list list, Face & face)
{
	Point<3> p1 = vertices[list[0]->previous_vertex].position;
	Point<3> p2 = vertices[list[1]->previous_vertex].position;
	Point<3> p3 = vertices[list[2]->previous_vertex].position;


	Point<3> d1 = p2 - p1;
	double d1_norm = d1.norm();
	Point<3> d2 = p3 - p1;
	double d2_norm = d2.norm();
	Point<3> d3 = p3 - p2;
	double d3_norm = d3.norm();

	if (d1_norm == 0.f || d2_norm == 0.f || d3_norm == 0.f) face.poor_aspect_ratio = true;
	//else{
	//	if (abs(d1.dot(d2)) / (d1_norm*d2_norm) > 0.995f) face.poor_aspect_ratio = true;
	//	face.angles[0] = acos(d1.dot(d2) / (d1_norm*d2_norm));
	//	if (abs(d1.dot(d3)) / (d1_norm*d3_norm) > 0.995f) face.poor_aspect_ratio = true;
	//	face.angles[1] = acos(d3.dot(d1)*(-1.f) / (d1_norm*d3_norm));
	//	if (abs(d2.dot(d3)) / (d2_norm*d3_norm) > 0.995f) face.poor_aspect_ratio = true;
	//	face.angles[2] = acos(d2.dot(d3) / (d2_norm*d3_norm));
	//}

	if (!face.poor_aspect_ratio){
		Point<3> d1_cross_d2 = d1.cross(d2);
		face.area = d1_cross_d2.norm();
		face.normal = d1_cross_d2.normalvector();
	}
}

NormalWeightMode normal_weight_mode = UNIFORM_NORMAL_WEIGHT;

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::SetVertexNormal(p_edge_list list, Vertex & vertex)
{
	//int valid_faces = 0;
	Point<3> cummulative_normal;
	double cummulative_weight = 0.f;
	for (int i = 0; i < list.size(); i++)
	{
		int face_index = list[i]->face;
		if (face_index != -1){
			Face & face = faces[face_index];
			if (!face.poor_aspect_ratio){

				if (normal_weight_mode == UNIFORM_NORMAL_WEIGHT){
					double weight = 1.f;
					cummulative_normal += faces[face_index].normal *weight;
					cummulative_weight += weight;
				}
				else if (normal_weight_mode == ANGLE_NORMAL_WEIGHT)
				{
					int face_vertex_index = VertexIndexInFace(vertex, face);
					if (face_vertex_index = -1){ printf("Error: Vertex not matching face! \n"); }
					double weight = 1.f - abs(1.f - (face.angles[face_vertex_index] * 2.f / PI));
					if (weight < 0.f){ printf("Error: Negative weight! \n"); }
					cummulative_normal += faces[face_index].normal *weight;
					cummulative_weight += weight;
				}

			}
		}
	}
	if (cummulative_weight == 0.f){
		//printf("Zero valid faces at vertex %d \n", vertex.vertex_index);
		vertex.poor_aspect_neighbourhood = true;
		vertex.normal = Point<3>();
	}
	else
	{
		vertex.normal = cummulative_normal.normalvector();
	}
}


template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::GeometryInitialization()
{
	printf("GEOMETRY INITIALIZATION... \n");
	FaceOperator set_face_geometry = &Mesh<Vertex, HalfEdge, Face, Statistics>::SetFaceGeometry;
	FaceOperatorTraverse(set_face_geometry, GENERATE_EDGE_LIST);

	VertexOperator set_vertex_normal = &Mesh<Vertex, HalfEdge, Face, Statistics>::SetVertexNormal;
	VertexOperatorTraverse(set_vertex_normal, GENERATE_EDGE_LIST);

	//face_normal_buffer_updated = false;
	//vertex_normal_buffer_updated = false;
	SetBasicMeshStatistics();
	SetPoorAspectFacesCount();
	SetPoorAspectVerticesCount();
	printf("DONE! \n");
}