#shader vertex
#version 330 core

layout(location=0) in vec4 position;

void main()
{
   gl_Position = position;
} // unlike the tutorial, having semi-colon here results in unsuccessful compilation of shaders


#shader fragment
#version 330 core

layout(location=0) out vec4 color;

uniform vec4 u_Color;

void main()
{
   color = u_Color;    // vec4(0.2, 0.3, 0.8, 1.0);
} // unlike the tutorial, having semi-colon here results in unsuccessful compilation of shaders