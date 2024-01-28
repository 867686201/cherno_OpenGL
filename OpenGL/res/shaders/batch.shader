#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
};


#shader fragment
#version 330 core

out vec4 FragColor;


void main()
{
	vec4 color = vec4(1.0f, 0.1f, 0.2f, 1.0f);
	FragColor = color;
};
