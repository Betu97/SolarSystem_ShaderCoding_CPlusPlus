#version 330

in vec2 v_uvs;
in vec3 v_normal;
in vec3 v_world_vertex_pos;

out vec4 fragColor;

uniform vec3 u_light_color;
uniform vec3 u_ambient;
uniform vec3 u_cam_pos;
uniform float u_shininess = 4;
uniform sampler2D u_texture;
uniform vec3 u_light_dir;


void main(void)
{
	// We're just going to paint the interpolated colour from the vertex shader
	vec3 N = normalize(v_normal);
	vec3 L = normalize(u_light_dir - u_cam_pos);
	vec3 R = reflect(L, N);
	vec3 E = normalize(u_cam_pos - v_world_vertex_pos);
	

	vec3 texture_color = texture2D(u_texture, v_uvs).xyz;
	vec3 ambient_color = texture_color * u_ambient;
	float NdotL = max( dot(N,L), 0.0);
	vec3 diffuse_color = texture_color * NdotL;
	float RdotE = pow(max(dot(R, -E), 0.0), u_shininess);
	vec3 specular_color = u_light_color * RdotE;
	fragColor =  vec4((specular_color + diffuse_color), 1.0);
}
