#shader vertex
#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec2 textureCoordinates;

// transferring data from vertex shader to fragment shader
out vec2 varying_textureCoordinates;

void main()
{
   gl_Position = position;
   varying_textureCoordinates = textureCoordinates;
} // unlike the tutorial, having semi-colon here results in unsuccessful compilation of shaders


#shader fragment
#version 330 core

layout(location=0) out vec4 color;

// transferring data from fragment shader to vertex shader
in vec2 varying_textureCoordinates;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
   vec4 texColor = texture(u_Texture, varying_textureCoordinates);
   color = texColor;    // vec4(0.2, 0.3, 0.8, 1.0);
} // unlike the tutorial, having semi-colon here results in unsuccessful compilation of shaders