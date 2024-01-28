#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;

uniform mat4 u_MVP;

out vec4 o_color;

void main()
{
	o_color = color;
	gl_Position = u_MVP * vec4(position, 1.0f, 1.0f);
};


#shader fragment
#version 330 core

in vec4 o_color;

out vec4 FragColor;


void main()
{
	FragColor = o_color;
};
