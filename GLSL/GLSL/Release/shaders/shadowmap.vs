#version 400

// GPU gems 3 Chapter 10
// GPU gems 2 Chapter 17
// GPU gems Chapter ??
// NVIDIA Whitepaper


uniform mat4 eye_projection;
uniform mat4 world_to_eye;
uniform mat4 object_to_world;
uniform mat3 normal_matrix;
uniform mat4 light_projection;
uniform mat4 world_to_light;
uniform mat4 texture_coord_transform;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec3 vertex_normal;

out vec3 v_color;
out vec3 normal;
out vec4 shadow_coord;

void main()
{
    normal = normalize( normal_matrix * vertex_normal );
    shadow_coord =texture_coord_transform * light_projection * world_to_light * object_to_world * vec4(vertex_position,1.0);
    v_color = vertex_color;
	
    // Camera Clipping Coordinates
    gl_Position =  eye_projection * world_to_eye * object_to_world * vec4(vertex_position,1.0);
}
