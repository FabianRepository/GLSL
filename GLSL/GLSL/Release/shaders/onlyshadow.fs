#version 400

//uniform sampler2DShadow shadow_map;

//in vec4 shadow_coord;
layout (location = 0) out vec4 FragColor;

void main() {
    //float shadow = textureProj(shadow_map,shadow_coord);
    //FragColor = vec4(vec3(1.0,1.0,1.0)*shadow,1.0);
    FragColor = vec4(1.0,1.0,1.0,1.0);
}
