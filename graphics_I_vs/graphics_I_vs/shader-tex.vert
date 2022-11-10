#version 330
 
in vec3 a_vertex;
in vec2 uvs;
in vec3 normal;

out vec2 v_uvs;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	v_uvs = uvs;
	gl_Position = u_projection * u_view * u_model * vec4(a_vertex, 1.0);



}

