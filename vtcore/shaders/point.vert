#version 330 core

layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aColor;

uniform vec2 offset;
out vec3 exColor;

void main()
{
    exColor = aColor;

    vec4 totalOffset = vec4(offset.x, offset.y, 0.0f, 0.0f);
    gl_Position = vec4(aPos, 1.0f) + totalOffset;
    gl_PointSize = 5.0f;
}
