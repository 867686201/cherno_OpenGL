#shader vertex
#version 330 core

layout(location = 0) in vec4 a_position; 
layout(location = 1) in vec3 a_color;

out vec3 v_color;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * a_position; 
	v_color = a_color;
};


#shader fragment
#version 330 core

out vec4 FragColor;

in vec3 v_color;

uniform sampler2D u_Texture;


void main()
{
	FragColor = vec4(v_color, 1.0f);
};
