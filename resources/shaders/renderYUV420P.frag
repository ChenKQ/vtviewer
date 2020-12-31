#version 330 core

in vec2 exTexCood;

out vec4 FragColor;

//uniform sampler2D ourtexture;
uniform sampler2D tex_y;
uniform sampler2D tex_u;
uniform sampler2D tex_v;

void main()
{
//    vec3 YUV;
//    vec3 RGB;
//    YUV.x = texture2D(tex_y, exTexCood).r;
//    YUV.y = texture2D(tex_u, exTexCood).r - 0.5;
//    YUV.z = texture2D(tex_v, exTexCood).r - 0.5;
////    RGB = mat3(1.0, 1.0, 1.0,
////               0.0, -0.39465, 2.03211,
////               1.13983, -0.58060, 0.0)*YUV;
//    RGB = mat3(1.0,    1.0,      1.0,
//               0.0,    -0.3455,  1.779,
//               1.4075, -0.7169,  0.0)*YUV;
//    FragColor = vec4(RGB, 1.0);

    // YUV-601 matrix
    vec4 YUV;
    YUV.x = texture2D(tex_y, exTexCood).r;
    YUV.y = texture2D(tex_u, exTexCood).r;
    YUV.z = texture2D(tex_v, exTexCood).r;
    YUV.w = 1.0;

    FragColor = mat4(1.0,     1.0,     1.0,     0,
                     0.0,     -0.3455, 1.779,   0,
                     1.4075,  -0.7169, 0.0,     0,
                     -0.70375,0.5312,  -0.8895, 1) * YUV;
}
