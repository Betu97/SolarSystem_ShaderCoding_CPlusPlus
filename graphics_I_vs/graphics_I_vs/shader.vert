#version 330
 
in vec3 a_vertex;
in vec2 uvs;
in vec3 normal;

out vec2 v_uvs;
out vec3 v_normal;
out vec3 v_world_vertex_pos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	// position of the vertex
	v_world_vertex_pos = (u_model * vec4( a_vertex, 1.0)).xyz;
	gl_Position = u_projection * u_view * u_model * vec4(a_vertex, 1.0);
	v_uvs = uvs;
	v_normal = normal;

}

