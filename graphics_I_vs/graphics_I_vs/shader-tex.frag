#version 330

in vec2 v_uvs;


out vec4 fragColor;


uniform sampler2D u_texture;



void main(void)
{

	vec3 texture_color = texture2D(u_texture, v_uvs).xyz;

	fragColor =  vec4(texture_color, 1.0);
}
