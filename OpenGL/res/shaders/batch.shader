#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_uvCoord;
layout(location = 3) in float a_slot;

uniform mat4 u_MVP;

out vec4 v_color;
out vec2 v_uvCoord;
out float v_slot;

void main()
{
	v_uvCoord = a_uvCoord;
	v_color = a_color;
	v_slot = a_slot;
	gl_Position = u_MVP * vec4(position, 1.0f, 1.0f);
};


#shader fragment
#version 330 core

in vec4 v_color;
in vec2 v_uvCoord;
in float v_slot;

out vec4 FragColor;

uniform sampler2D u_Textures[2];

void main()
{
	int idx = int(v_slot);
	vec4 color = texture(u_Textures[idx], v_uvCoord);
	FragColor = color;
};
