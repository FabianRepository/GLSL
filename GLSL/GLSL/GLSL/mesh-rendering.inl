#include "mesh.h"

#define PI 3.1415926

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::SetupOpenGL()
{
	ShaderTools::InitializeShaders("vertex_diffuse_reflection_shader", "fragment_simple_shader", programHandle);

	SetupVertexPositionBuffer();
	SetupVertexNormalBuffer();
	////SetupVertexColorBuffer();
	SetupEdgeIndexBuffer();
	SetupFaceIndexBuffer();

	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	glEnableVertexAttribArray(0);  // Vertex position
	glBindVertexBuffer(0, vertex_position_buffer , 0, sizeof(GLdouble)* 3);
	glVertexAttribFormat(0, 3, GL_DOUBLE, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);

	//glEnableVertexAttribArray(1);  // Vertex color
	//glBindVertexBuffer(1, colorBufferHandle, 0, sizeof(GLfloat)* 3);
	//glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
	//glVertexAttribBinding(1, 1);

	glVertexAttrib3f(1, 1.f, 1.f, 0.f);

	glEnableVertexAttribArray(2);
	glBindVertexBuffer(2, vertex_normal_buffer, 0, sizeof(GLdouble)* 3);
	glVertexAttribFormat(2, 3, GL_DOUBLE, GL_FALSE, 0);
	glVertexAttribBinding(2, 2);

	object_to_world = mat4(1.f);
	normal_matrix = glm::inverse(glm::transpose(mat3(object_to_world)));

}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::DrawOpenGL(Scene * scene){
	
	GLuint location = glGetUniformLocation(programHandle, "mvp");
	if (location >= 0){
		glUniformMatrix4fv(location, 1, GL_FALSE, &scene->mvp[0][0]);
	}
	location = glGetUniformLocation(programHandle, "world_to_camera");
	if (location >= 0){
		glUniformMatrix4fv(location, 1, GL_FALSE, &scene->world_to_camera[0][0]);
	}
	location = glGetUniformLocation(programHandle, "light_position");
	if (location >= 0){
		glUniform4f(location, scene->lights[0]->position[0], scene->lights[0]->position[1], scene->lights[0]->position[2], scene->lights[0]->position[3]);
	}
	location = glGetUniformLocation(programHandle, "light_color");
	if (location >= 0){
		glUniform3f(location, scene->lights[0]->color[0], scene->lights[0]->color[1], scene->lights[0]->color[2]);
	}
	location = glGetUniformLocation(programHandle, "object_to_world");
	if (location >= 0){
		glUniformMatrix4fv(location, 1, GL_FALSE, &object_to_world[0][0]);
	}
	location = glGetUniformLocation(programHandle, "normal_matrix");
	if (location >= 0){
		glUniformMatrix3fv(location, 1, GL_FALSE, &normal_matrix[0][0]);
	}
	location = glGetUniformLocation(programHandle, "shininess");
	if (location >= 0){
		glUniform1f(location, material->shininess);
	}
	glBindVertexArray(vaoHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face_index_buffer);
	glUseProgram(programHandle);
	glDrawElements(GL_TRIANGLES, 3 * faces.size(), GL_UNSIGNED_INT, 0);
}


template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::SetupEdgeIndexBuffer()
{
	int num_edges = edges.size();
	if (!glIsBuffer(edge_index_buffer))
	{
		glGenBuffers(1, &edge_index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edge_index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_edges*sizeof(unsigned int), NULL, GL_STATIC_DRAW);
	}

	// This is the double of undirected edges
	unsigned int *edges_vertex_indices = new  unsigned int[num_edges];
	bool * already_processed_edges = new bool[num_edges]; for (int i = 0; i < num_edges; i++){ already_processed_edges[i] = false; }
	int edge_iteraror = 0;
	for (const_edge_iterator edge_iter = edges.begin(); edge_iter != edges.end(); edge_iter++)
	{
		const HalfEdge edge = *edge_iter;
		if (!already_processed_edges[edge.edge_index])
		{
			const HalfEdge opposite_edge = edges[edge.opposite_edge];
			edges_vertex_indices[edge_iteraror] = (unsigned int)edge.previous_vertex;
			edges_vertex_indices[edge_iteraror + 1] = (unsigned int)edge.next_vertex;
			edge_iteraror += 2;
			already_processed_edges[edge.edge_index] = true;
			already_processed_edges[opposite_edge.edge_index] = true;
		}
	}
	for (int i = 0; i < num_edges; i++){ if (already_processed_edges[i] == false) printf("Non Processed Edge %d \n", i); }
	delete already_processed_edges;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edge_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_edges*sizeof(unsigned int), edges_vertex_indices, GL_STATIC_DRAW);

	delete edges_vertex_indices;
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::SetupFaceIndexBuffer()
{
	int num_faces = faces.size();
	if (!glIsBuffer(face_index_buffer))
	{
		glGenBuffers(1, &face_index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face_index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * num_faces*sizeof(unsigned int), NULL, GL_STATIC_DRAW);
	}

	unsigned int * faces_vertex_indices = new  unsigned int[3 * num_faces];
	double * face_normal_coordinates = new double[3 * num_faces];
	double * face_centroid_coordinates = new double[3 * num_faces];
	int face_iteraror = 0;

	for (const_face_iterator face_iter = faces.begin(); face_iter != faces.end(); face_iter++)
	{
		const Face face = *face_iter;
		int initial_edge = face.edge;
		faces_vertex_indices[face_iteraror] = edges[initial_edge].previous_vertex;
		face_iteraror++;
		int next_edge = edges[initial_edge].next_edge;
		while (next_edge != initial_edge)
		{
			faces_vertex_indices[face_iteraror] = edges[next_edge].previous_vertex;
			face_iteraror++;
			next_edge = edges[next_edge].next_edge;
		}
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, face_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * num_faces*sizeof(unsigned int), faces_vertex_indices, GL_STATIC_DRAW);

	delete faces_vertex_indices;
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::VertexPositionAssignation(Vertex & vertex, int index, double * external_array)
{
	const Point<3> position = vertex.position;
	external_array[3 * index] = position[0];
	external_array[3 * index + 1] = position[1];
	external_array[3 * index + 2] = position[2];
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::SetupVertexPositionBuffer()
{
	int num_vertices = vertices.size();
	if (!glIsBuffer(vertex_position_buffer))
	{
		glGenBuffers(1, &vertex_position_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_position_buffer);
		glBufferData(GL_ARRAY_BUFFER, 3 * num_vertices*sizeof(double), NULL, GL_STATIC_DRAW);
	}

	double * vertex_coordinates = new  double[3 * num_vertices];
	VertexAssignation<double *> vertex_position_assignation = &Mesh<Vertex, HalfEdge, Face, Statistics>::VertexPositionAssignation;
	VertexAssignationTraverse<double *>(vertex_position_assignation, vertex_coordinates);

	glBindBuffer(GL_ARRAY_BUFFER, vertex_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * num_vertices*sizeof(double), vertex_coordinates, GL_STATIC_DRAW);
	delete vertex_coordinates;
}


template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::VertexNormalAssignation(Vertex & vertex, int index, double * external_array)
{
	 external_array[3 * index] = vertex.normal[0];
	 external_array[3 * index + 1] = vertex.normal[1];
	 external_array[3 * index + 2] = vertex.normal[2];
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::SetupVertexNormalBuffer()
{
	int num_vertices = vertices.size();
	if (!glIsBuffer(vertex_normal_buffer))
	{
		glGenBuffers(1, &vertex_normal_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_normal_buffer);
		glBufferData(GL_ARRAY_BUFFER, 3 * num_vertices*sizeof(double), NULL, GL_STATIC_DRAW);
	}
	double * normal_coordinates = new  double[3 * num_vertices];

	VertexAssignation<double *> vertex_normal_assignation = &Mesh<Vertex, HalfEdge, Face, Statistics>::VertexNormalAssignation;
	VertexAssignationTraverse<double *>(vertex_normal_assignation, normal_coordinates);

	glBufferData(GL_ARRAY_BUFFER, 3 * num_vertices*sizeof(double), normal_coordinates, GL_STATIC_DRAW);
	delete normal_coordinates;
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::VertexColorAssignation(Vertex & vertex, int index, double * external_array)
{
	const Point<3> color = vertex.color;
	external_array[3 * index] = color[0];
	external_array[3 * index + 1] = color[1];
	external_array[3 * index + 2] = color[2];
}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::SetupVertexColorBuffer()
{
	if (glIsBuffer(vertex_color_buffer))
	{
		glDeleteBuffers(1, &vertex_color_buffer);
	}
	int num_vertices = vertices.size();
	double * color_coordinates = new  double[3 * num_vertices];

	VertexAssignation<double *> vertex_color_assignation = &Mesh<Vertex, HalfEdge, Face, Statistics>::VertexColorAssignation;
	VertexAssignationTraverse<double *>(vertex_color_assignation, color_coordinates);

	glGenBuffers(1, &vertex_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * num_vertices*sizeof(double), color_coordinates, GL_STATIC_DRAW);
	delete color_coordinates;
}