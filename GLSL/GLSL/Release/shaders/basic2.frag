#version 400

uniform vec3 light_direction;
uniform vec3 light_color;

in vec3 v_color;
in vec3 normal;

layout (location = 0) out vec4 FragColor;

void main() {
    vec3 position_to_light = normalize(light_direction);
    FragColor = vec4(light_color * v_color * max(dot(normalize(normal),position_to_light), 0.0),1.f);
}
