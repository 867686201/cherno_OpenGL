#shader vertex
#version 330 core

layout(location = 0) in vec4 position; 
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position; 
	v_TexCoord = texCoord;
};


#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;


void main()
{
	vec4 color = texture(u_Texture, v_TexCoord);
	FragColor = color;
};
