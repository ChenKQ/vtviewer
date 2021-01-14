#version 330 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inTexCood;

uniform vec2 border;
uniform vec2 offset;
uniform float zoomRate;

out vec2 exTexCood;

void main()
{
    float outx = (inPos.x * border.x )*zoomRate + offset.x;
    float outy = (inPos.y * border.y )*zoomRate + offset.y;
    gl_Position = vec4(outx, outy, inPos.z, 1.0f);
//    gl_Position = vec4(inPos, 1.0f);
    exTexCood = inTexCood;
}
