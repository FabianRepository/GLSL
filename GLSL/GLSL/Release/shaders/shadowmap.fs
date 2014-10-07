#version 400

uniform vec3 light_direction;
uniform vec3 light_color;
uniform vec3 ambient_light;

uniform sampler2DShadow shadow_map;
uniform sampler3D offset_tex;
uniform ivec3 offset_tex_size; // (width, height, depth)
uniform float radius;

in vec3 v_color;
in vec3 normal;
in vec4 shadow_coord;

layout (location = 0) out vec4 FragColor;

vec3 DiffuseModel()
{
	vec3 position_to_light = normalize(-light_direction);
	vec3 intensity = light_color * v_color * max(dot(normalize(normal),position_to_light), 0.0);
    return intensity;
}

subroutine void RenderPassType();
subroutine uniform RenderPassType RenderPass;

subroutine (RenderPassType)
void shadeWithShadow()
{
  vec3 diffuse = DiffuseModel();
  
  ivec3 offset_coord;
  offset_coord.xy = ivec2( mod( gl_FragCoord.xy,offset_tex_size.xy) );

  float sum = 0.0;
  int samples_div2 = int(offset_tex_size.z);
  vec4 sc = shadow_coord;

  for( int i = 0 ; i< 4; i++ ) {
    offset_coord.z = i;
    vec4 offsets = texelFetch(offset_tex,offset_coord,0) * radius * shadow_coord.w;

    sc.xy = shadow_coord.xy + offsets.xy;
    sum += textureProj(shadow_map, sc);
    sc.xy = shadow_coord.xy + offsets.zw;
    sum += textureProj(shadow_map, sc);
  }
  float shadow = sum / 8.0;

  if( shadow != 1.0 && shadow != 0.0 ) {
    for( int i = 4; i< samples_div2; i++ ) {
        offset_coord.z = i;
        vec4 offsets = texelFetch(offset_tex,offset_coord,0) * radius * shadow_coord.w;

        sc.xy = shadow_coord.xy + offsets.xy;
        sum += textureProj(shadow_map, sc);
        sc.xy = shadow_coord.xy + offsets.zw;
        sum += textureProj(shadow_map, sc);
    }
    shadow = sum / float(samples_div2 * 2.0);
  }

    // If the fragment is in shadow, use ambient light only.
   FragColor = vec4( (diffuse  * shadow + ambient_light)*v_color, 1.0);

    // Gamma correct
    //FragColor = pow( FragColor, vec4(1.0 / 2.2) );
}

subroutine (RenderPassType)
void recordDepth()
{
    // Do nothing, depth will be written automatically
}

void main() {
    // This will call either shadeWithShadow or recordDepth
    RenderPass();
}
