#version 330 core

layout(location=0) in vec3 aPos;

uniform vec2 offset;

void main()
{
    vec4 totalOffset = vec4(offset.x, offset.y, 0.0f, 0.0f);
    gl_Position = vec4(aPos, 1.0f) + totalOffset;
}
