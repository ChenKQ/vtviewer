#version 330 core

layout(location=0) in vec3 aPos;

uniform vec2 border;
uniform vec2 offset;
uniform float zoomRate;

void main()
{
    float outx = (aPos.x * border.x )*zoomRate + offset.x;
    float outy = (aPos.y * border.y )*zoomRate + offset.y;
    gl_Position = vec4(outx, outy, aPos.z, 1.0f);
}
