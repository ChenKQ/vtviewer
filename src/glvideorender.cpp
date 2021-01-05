#include "glvideorender.h"
#include "utils.h"

#include <string>
#include <stdexcept>

GLImageRender<brick::media::PixelFormat::RGB24>::GLImageRender()
{

}

GLImageRender<brick::media::PixelFormat::RGB24>::~GLImageRender()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void GLImageRender<brick::media::PixelFormat::RGB24>::initialize()
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

void GLImageRender<brick::media::PixelFormat::RGB24>::render(unsigned char **pData, int width, int height, const vtviewer::View& view)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    if(pData == nullptr || pData[0]==nullptr)
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
                     GL_RGB, GL_UNSIGNED_BYTE, pData[0]);

        QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
    m_program.release();
}


GLImageRender<brick::media::PixelFormat::YUV420P>::GLImageRender()
{

}

GLImageRender<brick::media::PixelFormat::YUV420P>::~GLImageRender()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}


void GLImageRender<brick::media::PixelFormat::YUV420P>::initialize()
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


    for(auto& texImage : tex_image_yuv420_planes)
    {
        glGenTextures(1, &texImage);
        glBindTexture(GL_TEXTURE_2D, texImage);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    m_program.bind();
    glUniform1i(m_program.uniformLocation("tex_y"), 0);
    glUniform1i(m_program.uniformLocation("tex_u"), 1);
    glUniform1i(m_program.uniformLocation("tex_v"), 2);
    m_program.release();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLImageRender<brick::media::PixelFormat::YUV420P>::render(unsigned char **pData, int width, int height, const vtviewer::View& view)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    if(pData == nullptr || pData[0]==nullptr)
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
        glBindTexture(GL_TEXTURE_2D, tex_image_yuv420_planes[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0,
                     GL_RED, GL_UNSIGNED_BYTE, pData[0]);



        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex_image_yuv420_planes[1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width/2, height/2, 0,
                     GL_RED, GL_UNSIGNED_BYTE, pData[1]);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, tex_image_yuv420_planes[2]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width/2, height/2, 0,
                     GL_RED, GL_UNSIGNED_BYTE, pData[2]);

        QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
    m_program.release();
}



std::unique_ptr<ImageRender> ImageRender::CreateInstance(brick::media::PixelFormat pixelFormat)
{
    ImageRender* pRender = nullptr;
    if(brick::media::PixelFormat::YUV420P == pixelFormat)
    {
        pRender = new GLImageRender<brick::media::PixelFormat::YUV420P>();
    }
    else if(brick::media::PixelFormat::RGB24 == pixelFormat)
    {
        pRender = new GLImageRender<brick::media::PixelFormat::RGB24>();
    }
//    else
//    {
//        throw std::runtime_error(std::string("non-supported pixelf format"));
//    }
    return std::unique_ptr<ImageRender>(pRender);
}
