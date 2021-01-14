#include "vtcore/render/glvideorender.h"
#include "vtcore/render/view.h"

#include <string>
#include <stdexcept>

namespace vtcore
{
namespace render
{

GLImageRender::GLImageRender()
{

}

GLImageRender::~GLImageRender()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void GLImageRender::initialize()
{
    initializeOpenGLFunctions();

    bool success = m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFile);
    if(!success)
    {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << m_program.log();
        return;
    }

    success = m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFile);
    if(!success)
    {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << m_program.log();
        return;
    }

    success = m_program.link();
    if(!success)
    {
        qDebug() << "shaderProgram addShaderFromSourceFile failed!" << m_program.log();
        return;
    }

    QOpenGLVertexArrayObject::Binder vaoBinder(&vao);

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);


    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenTextures(1, &texImage);
    glBindTexture(GL_TEXTURE_2D, texImage);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_program.bind();
    glUniform1i(m_program.uniformLocation("tex"), 0);
    m_program.release();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLImageRender::render(const unsigned char*pData, int width, int height, const View& view)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    if(pData == nullptr)
        return;

    m_program.bind();
    {
        int borderIndex = m_program.uniformLocation("border");
        glUniform2f(borderIndex, view.GLBorder.x, view.GLBorder.y);
        int offsetIndex = m_program.uniformLocation("offset");
        glUniform2f(offsetIndex, view.OffsetGL.x, view.OffsetGL.y);
        int zoomRateIndex= m_program.uniformLocation("zoomRate");
        glUniform1f(zoomRateIndex, view.ZoomRate);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texImage);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                     GL_BGR, GL_UNSIGNED_BYTE, pData);

        QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
    m_program.release();
}

}   // namespace render
}   // namespace vtcore


