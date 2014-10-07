#include "mesh.h"
#include "ply_io.h"


class VertexPair
{
public:
	VertexPair(){}
	VertexPair(int p_vertex_from, int p_vertex_to)
	{
		vertex_from = p_vertex_from;
		vertex_to = p_vertex_to;
	}
	int vertex_from;
	int vertex_to;
	int edge_index;
};

class VertexPairComparison
{
public:
	bool operator()(VertexPair p1, VertexPair p2)
	{
		if (p1.vertex_from < p2.vertex_from)
			return true;
		else if (p1.vertex_from > p2.vertex_from)
			return false;
		else if (p1.vertex_from == p2.vertex_from && p1.vertex_to < p2.vertex_to)
			return true;
		else
			return false;
	}
};

typedef std::set<VertexPair, VertexPairComparison> vertex_pair_list;
typedef std::set<VertexPair, VertexPairComparison>::const_iterator const_vertex_pair_iterator;

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::ConstructMeshFromPlyFile(const char * file_name) // This assumes oriented surfaces.
{
	printf("CONSTRUCTING MESH FROM PLY...\n");
	// Read the input file // Use Greg Turk methods
	int  color_type[1];
	ply_Mesh * ply_mesh = read_ply_file(file_name, color_type);
	int ply_nverts = ply_mesh->nverts;
	ply_Vertex **vlist = ply_mesh->vlist;
	int ply_nfaces = ply_mesh->nfaces;
	ply_Face **flist = ply_mesh->flist;

	//Mesh<Vertex, HalfEdge, Face, Statistics> * mesh = new Mesh<Vertex, HalfEdge, Face, Statistics>();
	vertices.reserve(ply_nverts);
	for (int i = 0; i < ply_nverts; i++)
	{
		ply_Vertex * current_vertex = vlist[i];
		Point<3> position = Point<3>(current_vertex->x, current_vertex->y, current_vertex->z);
		Point<3> normal = Point<3>(current_vertex->nx, current_vertex->ny, current_vertex->nz);
		Point<3> color = Point<3>(current_vertex->r, current_vertex->g, current_vertex->b);
		if (color_type[0] == 4) color /= 255.f;
		vertices.push_back(Vertex(i, position, normal, color));
	}

	// Set Connectivity 
	edges.reserve(6 * ply_nfaces); // Must change for quads
	vertex_pair_list vertex_pairs;
printf("%d * %ld = %ld\n", ply_nfaces, sizeof(Face), sizeof(Face)*ply_nfaces);
	faces.reserve(ply_nfaces);

	bool require_update_orientation = false;

	for (int i = 0; i < ply_nfaces; i++)
	{
		ply_Face * ply_face = flist[i];
		int nverts = ply_face->nverts;
		int * verts = ply_face->verts;
		int initial_num_edges = edges.size();

		faces.push_back(Face(i));
		Face & current_face = faces.back();
		current_face.edge = initial_num_edges;

		for (int j = 0; j < nverts; j++)
		{
			int vertex_from = verts[j];
			int vertex_to = j < nverts - 1 ? verts[j + 1] : verts[0];
			int new_edge_index = initial_num_edges + j;
			if (edges.size() != new_edge_index)
			{
				printf("Bad Count On Edge Number \n");
			}

			edges.push_back(HalfEdge(new_edge_index));
			HalfEdge  & current_edge = edges.back();
			current_edge.face = i;
			current_edge.previous_vertex = vertex_from;
			current_edge.next_vertex = vertex_to;
			current_edge.previous_edge = j > 0 ? new_edge_index - 1 : initial_num_edges + nverts - 1;
			current_edge.next_edge = j < nverts - 1 ? new_edge_index + 1 : initial_num_edges;

			VertexPair edge0(vertex_from, vertex_to);
			VertexPair edge1(vertex_to, vertex_from);
			const_vertex_pair_iterator pointer_edge0 = vertex_pairs.find(edge0); // opposite edge could have been added with either same or different vertex order.
			const_vertex_pair_iterator pointer_edge1 = vertex_pairs.find(edge1);
			if (pointer_edge0 == vertex_pairs.end() && pointer_edge1 == vertex_pairs.end())
			{
				edge0.edge_index = new_edge_index;
				vertex_pairs.insert(edge0);
				current_edge.opposite_edge = -1;
			}
			else if (pointer_edge0 == vertex_pairs.end() && pointer_edge1 != vertex_pairs.end())
			{
				edge1 = *pointer_edge1;
				current_edge.opposite_edge = edge1.edge_index;
				edges[edge1.edge_index].opposite_edge = current_edge.edge_index;
				vertex_pairs.erase(pointer_edge1);
			}
			else if (pointer_edge0 != vertex_pairs.end() && pointer_edge1 == vertex_pairs.end())
			{
				require_update_orientation = true;
				edge0 = *pointer_edge0;
				current_edge.opposite_edge = edge0.edge_index;
				edges[edge0.edge_index].opposite_edge = current_edge.edge_index;
				vertex_pairs.erase(pointer_edge0);
			}
			else{
				printf("ERROR \n");
			}
		}
	}

	int num_interior_edge_index = edges.size();

	// INVARIANTS
	// (1) Edges in vertex_pairs are those whose opposite is an exterior halfedge. These are also the edges in edges with opposite = -1
	// (2) All the halfedges belonging to a same face define a cycle

	if (require_update_orientation)
	{
		// Propagate Orientation

		bool * already_processed_edge = new bool[num_interior_edge_index];
		memset(already_processed_edge, false, num_interior_edge_index*sizeof(bool));

		bool * need_to_change_orientation = new bool[num_interior_edge_index];
		memset(need_to_change_orientation, false, num_interior_edge_index*sizeof(bool));

		std::queue<HalfEdge *> edges_queue;
		edges_queue.push(&edges[0]);

		int processed_elements = 0;
		while ((!edges_queue.empty()) && (processed_elements < num_interior_edge_index))
		{
			HalfEdge * current = edges_queue.front();
			bool current_need_to_change = need_to_change_orientation[current->edge_index];

			// Look at next // Next is always defined
			if (current_need_to_change)
			{
				HalfEdge * next = &edges[current->next_edge];
				if (!already_processed_edge[next->edge_index])
				{
					need_to_change_orientation[next->edge_index] = true;
					edges_queue.push(next);
				}
			}

			// Look at opposite // Opposite may not be defined
			if (current->opposite_edge != -1)
			{
				HalfEdge * opposite = &edges[current->opposite_edge];
				if (!already_processed_edge[opposite->edge_index])
				{
					bool opposite_direction = (current->previous_vertex == opposite->next_vertex);
					if (opposite_direction && current_need_to_change)
					{
						need_to_change_orientation[opposite->edge_index] = true;
						edges_queue.push(opposite);
					}
				}
			}

			// Change direction if required
			if (current_need_to_change)
			{
				int old_previuos_vertex = current->previous_vertex;
				int old_previuos_edge = current->previous_edge;
				current->previous_edge = current->next_edge;
				current->next_edge = old_previuos_vertex;
				current->previous_vertex = current->next_vertex;
				current->next_vertex = old_previuos_vertex;
			}

			//Update queue and list states
			already_processed_edge[current->edge_index] = true;
			edges_queue.pop();
			processed_elements++;
		}
	}


	// Create the exterior halfedges
	for (const_vertex_pair_iterator vertex_pair_iter = vertex_pairs.begin(); vertex_pair_iter != vertex_pairs.end(); vertex_pair_iter++)
	{
		VertexPair current_pair = *vertex_pair_iter;
		int exterior_edge_index = edges.size();
		edges.push_back(HalfEdge(exterior_edge_index));
		HalfEdge  & exterior_edge = edges.back();
		exterior_edge.opposite_edge = current_pair.edge_index;
		exterior_edge.previous_vertex = current_pair.vertex_to;
		exterior_edge.next_vertex = current_pair.vertex_from;
		exterior_edge.face = -1;
		edges[current_pair.edge_index].opposite_edge = exterior_edge_index;
	}

	int last_exterior_edge_index = edges.size();

	// Set exterior halfedges connectivity

	for (int i = num_interior_edge_index; i < last_exterior_edge_index; i++)
	{
		HalfEdge & exterior_edge = edges[i];
		int current_edge_index = exterior_edge.opposite_edge;
		int iter = 0;
		int max_iter = 20;
		while (edges[current_edge_index].face != -1 && iter < max_iter)
		{
			current_edge_index = edges[edges[current_edge_index].next_edge].opposite_edge;
			iter++;
		}
		if (iter == max_iter)
		{
			printf("ERROR : EXTERIOR HALFEDGE NON-MATCHED \n");
		}
		else
		{
			HalfEdge & previous_exterior_edge = edges[current_edge_index];
			AssignNextPreviousRelation(previous_exterior_edge, exterior_edge);
		}
	}

	// Add vertex pointers
	for (const_edge_iterator edge_iter = edges.begin(); edge_iter != edges.end(); edge_iter++)
	{
		vertices[edge_iter->previous_vertex].edge = edge_iter->edge_index;
	}

	//NormalizeByExtremes();
	NormalizeByDiameter();
	Centralize();
	SetMeshOrientation();
	//ComputeVertexNormalFromPosition();

	//writePlyFile(*mesh, "triangle2.ply");

	if (VerifyMeshCorrectness())
	{
		printf("Correct Mesh Construction \n");
	}
	else
	{
		printf("Uncorrect Mesh Construction \n");
	}


	//Deallocate Memory

	for (int i = 0; i < ply_nverts; i++)
	{
		free(vlist[i]);
	}
	for (int i = 0; i < ply_nfaces; i++)
	{
		free(flist[i]);
	}
	free(vlist);
	free(flist);

	printf("DONE!\n");
	//return mesh;

}

//
//void writeVertices(const vertex_list & vertices, std::ofstream & plyFile)
//{
//	for (const_vertex_iterator vertex_iter = vertices.begin(); vertex_iter != vertices.end(); vertex_iter++)
//	{
//		const Vertex vertex = *vertex_iter;
//		const Point<3> position = vertex.position;
//		const Point<3> normal = vertex.normal;
//		plyFile << position[0] << " " << position[1] << " " << position[2] << " " << normal[0] << " " << normal[1] << " " << normal[2] << "\n";
//	}
//}
//
//void writeFaces(const face_list & faces, const edge_list & edges, std::ofstream & plyFile)
//{
//	for (const_face_iterator face_iter = faces.begin(); face_iter != faces.end(); face_iter++)
//	{
//		const Face face = *face_iter;
//		int initial_edge = face.edge;
//		plyFile << 3 << " " << edges[initial_edge].previous_vertex;
//		int next_edge = edges[initial_edge].next_edge;
//		while (next_edge != initial_edge)
//		{
//			plyFile << " " << edges[next_edge].previous_vertex;
//			next_edge = edges[next_edge].next_edge;
//		}
//		plyFile << "\n";
//	}
//}
//
//
//void writePlyFile(const Mesh & mesh, const char * fileName)
//{
//	std::ofstream plyFile;
//	plyFile.open(fileName);
//
//	plyFile << "ply \n" << "format ascii 1.0 \n" << "element vertex " << mesh.vertices.size() << "\n";
//	plyFile << "property double x \n" << "property double y \n" << "property double z \n" << "property double nx \n" << "property double ny \n" << "property double nz \n";
//	plyFile << "element face " << mesh.faces.size() << "\n" << "property list int int vertex_indices \n" << "end_header \n";
//
//	writeVertices(mesh.vertices, plyFile);
//	writeFaces(mesh.faces, mesh.edges, plyFile);
//
//	plyFile.close();
//}



template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::SetVertexColorValueToTexture(p_edge_list list, Vertex & vertex)
{
	Point<4> texture_sample = texture->BilinearSample(vertex.position[0], vertex.position[1]);
	vertex.color = Point<3>(texture_sample[0], texture_sample[1], texture_sample[2]);
}
//
//template<typename Vertex, typename HalfEdge, typename Face, typename Statistics,int dimension>
//void Mesh_Advection<Vertex, HalfEdge, Face, Statistics,dimension>::SetVertexCurrentColorValue(p_edge_list list, Vertex & vertex)
//{
//	vertex.current_color = Point<3>(0.f, vertex.scalar_value, 0.f);
//}

template<typename Vertex, typename HalfEdge, typename Face, typename Statistics>
void Mesh<Vertex, HalfEdge, Face, Statistics>::_SetVertexColorValueToTexture()
{
	if (texture == 0)
	{
		printf("Undefined Texture \n");
	}
	VertexOperator set_vertex_color_value = &Mesh<Vertex, HalfEdge, Face, Statistics>::SetVertexColorValueToTexture;
	VertexOperatorTraverse(set_vertex_color_value, DO_NOT_GENERATE_EDGE_LIST);
}