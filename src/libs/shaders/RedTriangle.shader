#shader vertex
#version 310 es

layout(location = 0) in mediump vec4 position;
layout(location = 1) in mediump vec3 color;

out mediump vec4 v_Color;

void main()
{
	v_Color = vec4(color, 1.0f); 
	gl_Position = position;
};

#shader fragment
#version 310 es

layout(location = 0) out mediump vec4 o_Color;

in mediump vec4 v_Color;

void main()
{
   o_Color = v_Color; 
};

