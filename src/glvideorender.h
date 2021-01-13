#ifndef GLVIDEORENDER_H
#define GLVIDEORENDER_H

#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

#include <memory>

namespace vtviewer
{
struct View;
}

class GLImageRender: public QOpenGLFunctions
{
public:
    GLImageRender();
    ~GLImageRender();

    void initialize();
    void render(const unsigned char*pData, int width, int height, const vtviewer::View& view);
private:
    QOpenGLShaderProgram m_program;
    QOpenGLVertexArrayObject vao;
    unsigned int vbo, ebo;
    unsigned int texImage;

    QString vertexShaderFile {":/shaders/renderRGB.vert"};
    QString fragmentShaderFile {":/shaders/renderRGB.frag"};

    float vertices[20] =
    {
        // positions          // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 0.0f, // top right
         1.0f, -1.0f, 0.0f,   1.0f, 1.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 1.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   0.0f, 0.0f  // top left
    };

    unsigned int indices[6] =
    {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
};

#endif // GLVIDEORENDER_H
