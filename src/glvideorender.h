#ifndef GLVIDEORENDER_H
#define GLVIDEORENDER_H

#include "media/ffmpeg.h"

#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

template <brick::media::PixelFormat PIX_FMT> class GLVideoRender;

template <> class GLVideoRender<brick::media::PixelFormat::RGB24>: public QOpenGLFunctions
{
public:
    GLVideoRender();
    ~GLVideoRender();

    void initialize();
    void render(unsigned char**pData, int width, int height);
private:
    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject vao;
    unsigned int vbo, ebo;
    unsigned int texImage;

    QString vertexShaderFile {":/shaders/renderRGB.vert"};
    QString fragmentShaderFile {":/shaders/renderRGB.frag"};

    float vertices[20] = {
        // positions          // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 0.0f, // top right
         1.0f, -1.0f, 0.0f,   1.0f, 1.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 1.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   0.0f, 0.0f  // top left
    };

    unsigned int indices[6] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
};

template <> class GLVideoRender<brick::media::PixelFormat::YUV420P>: QOpenGLFunctions
{
public:
    GLVideoRender();
    ~GLVideoRender();

    void initialize();
    void render(unsigned char**pData, int width, int height);

private:
    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject vao;
    unsigned int vbo, ebo;
    unsigned int tex_image_yuv420_planes[3] = {0};

    QString vertexShaderFile {":/shaders/renderYUV420P.vert"};
    QString fragmentShaderFile{":/shaders/renderYUV420P.frag"};

    //    float vertices[20] = {
    //        // positions          // texture coords
    //         1.0f,  1.0f, 0.0f,   1.0f, 0.0f, // top right
    //         1.0f, -1.0f, 0.0f,   1.0f, 1.0f, // bottom right
    //        -1.0f, -1.0f, 0.0f,   0.0f, 1.0f, // bottom left
    //        -1.0f,  1.0f, 0.0f,   0.0f, 0.0f  // top left
    //    };

    float vertices[20] = {
        // positions          // texture coords
         20.0f,  20.0f, 0.0f,   1.0f, 0.0f, // top right
         20.0f, -20.0f, 0.0f,   1.0f, 1.0f, // bottom right
        -20.0f, -20.0f, 0.0f,   0.0f, 1.0f, // bottom left
        -20.0f,  20.0f, 0.0f,   0.0f, 0.0f  // top left
    };

    unsigned int indices[6] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
};

#endif // GLVIDEORENDER_H
