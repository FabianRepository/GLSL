#version 400

// GPU gems 3 Chapter 10
// GPU gems 2 Chapter 17
// GPU gems Chapter ??
// NVIDIA Whitepaper


//uniform mat4 camera_projection;
//uniform mat4 world_to_camera;
uniform mat4 object_to_world;
//uniform mat4 shadow_matrix;

uniform mat4 light_view_projection;

layout(location = 0) in vec3 vertex_position;

//out vec4 shadow_coord;

void main()
{
    //shadow_coord = shadow_matrix* vec4(vertex_position,1.0);
    // Camera Clipping Coordinates
    //gl_Position =  camera_projection * world_to_camera * object_to_world * vec4(vertex_position,1.0);
	gl_Position =  light_view_projection * object_to_world * vec4(vertex_position,1.0);
}
