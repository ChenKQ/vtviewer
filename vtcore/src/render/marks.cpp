#include "vtcore/render/marks.h"
#include "vtcore/render/view.h"

namespace vtcore
{
namespace render
{

ScaleMarks::ScaleMarks()
{

}

void ScaleMarks::initialize()
{
//    initialVerticalScale();

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

    initialHorizontalScaler();
    initialVerticalScaler();
}

void ScaleMarks::render()
{
    m_program.bind();
    {
        QOpenGLVertexArrayObject::Binder vaoBinderHorizontalScaler(&vaoHorizontalScaler);
        glDrawArrays(GL_LINES, 0, HorizontalPointNumber);
        vaoBinderHorizontalScaler.release();

        QOpenGLVertexArrayObject::Binder vaoBinderVerticalScaler(&vaoVerticalScaler);
        glDrawArrays(GL_LINES, 0, VerticalPointNumber);
        vaoBinderVerticalScaler.release();
    }
    m_program.release();
}

void ScaleMarks::initialHorizontalScaler()
{
    int index = 0;
    HorizontalPointNumber = index;
    // 左半段无效角度
    drawHorizontalLine(verticesHorizontalScaler, index, HorizontalScaler::bottomMargin, HorizontalScaler::leftMargin,
                       HorizontalScaler::value2Position(HorizontalNegativeLimit),
                       HorizontalScaler::invalidColor);

    // 中间段有效角度
    drawHorizontalLine(verticesHorizontalScaler, index, HorizontalScaler::bottomMargin,
                       HorizontalScaler::value2Position(HorizontalNegativeLimit),
                       HorizontalScaler::value2Position(HorizontalPostiveLimit),
                       HorizontalScaler::validColor);

    // 右半段无效角度
    drawHorizontalLine(verticesHorizontalScaler, index, HorizontalScaler::bottomMargin,
                       HorizontalScaler::value2Position(HorizontalPostiveLimit),
                       HorizontalScaler::rightMargin,
                       HorizontalScaler::invalidColor);

    // 主刻度
    float majorStep = HorizontalScaler::majorStep;
    int numberMajorMarks = 1+
            (HorizontalScaler::postiveLimit-HorizontalScaler::negativeLimit)/HorizontalScaler::majorUnit;
    int startMajorMarkIndex = (HorizontalNegativeLimit+HorizontalScaler::majorUnit-1
            -HorizontalScaler::negativeLimit)/HorizontalScaler::majorUnit;
    int endMajorMarkIndex =(HorizontalPostiveLimit-HorizontalScaler::negativeLimit)/HorizontalScaler::majorUnit;
    for(int i=0; i< numberMajorMarks; ++i)
    {
        const float* color = HorizontalScaler::validColor;
        if(i<startMajorMarkIndex || i>endMajorMarkIndex)
        {
            color = HorizontalScaler::invalidColor;
        }
        drawVerticalLine(verticesHorizontalScaler, index, HorizontalScaler::leftMargin+i*majorStep,
                         HorizontalScaler::majorHeight, HorizontalScaler::bottomMargin,
                         color);
    }

    // 次刻度
    float minorStep = HorizontalScaler::minorStep;
    int numberMinorMarks = 1+
            (HorizontalScaler::postiveLimit-HorizontalScaler::negativeLimit)/HorizontalScaler::minorUnit;
    int startMinorMarkIndex = (HorizontalNegativeLimit+HorizontalScaler::minorUnit-1
            -HorizontalScaler::negativeLimit)/HorizontalScaler::minorUnit;
    int endMinorMarkIndex = (HorizontalPostiveLimit-HorizontalScaler::negativeLimit)
            /HorizontalScaler::minorUnit;
    for(int i=0; i<numberMinorMarks; ++i)
    {
        const float* color = HorizontalScaler::validColor;
        if(i<startMinorMarkIndex || i>endMinorMarkIndex)
        {
            color = HorizontalScaler::invalidColor;
        }
        drawVerticalLine(verticesHorizontalScaler, index, HorizontalScaler::leftMargin+i*minorStep,
                         HorizontalScaler::minorHeight, HorizontalScaler::bottomMargin,
                         color);
    }

    HorizontalPointNumber = index;

    QOpenGLVertexArrayObject::Binder vaoBinder(&vaoHorizontalScaler);

    glGenBuffers(1, &vboHorizontalScaler);

    glBindBuffer(GL_ARRAY_BUFFER, vboHorizontalScaler);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesHorizontalScaler),
                 verticesHorizontalScaler, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ScaleMarks::initialVerticalScaler()
{
    int index = 0;
    VerticalPointNumber = index;
    // 下半段无效角度
    drawVerticalLine(verticesVerticalScaler, index, VerticalScaler::rightMargin,
                     VerticalScaler::bottomMargin,
                     VerticalScaler::value2Position(VerticalNegativeLimit),
                     VerticalScaler::invalidColor);
    // 中间段有效角度
    drawVerticalLine(verticesVerticalScaler, index, VerticalScaler::rightMargin,
                     VerticalScaler::value2Position(VerticalNegativeLimit),
                     VerticalScaler::value2Position(VerticalPostiveLimit),
                     VerticalScaler::validColor);
    // 上半段无效角度
    drawVerticalLine(verticesVerticalScaler, index, VerticalScaler::rightMargin,
                     VerticalScaler::value2Position(VerticalPostiveLimit),
                     VerticalScaler::topMargin,
                     VerticalScaler::invalidColor);

    // 主刻度
    float majorStep = VerticalScaler::majorStep;
    int numberMajorMarks = 1+
            (VerticalScaler::postiveLimit-VerticalScaler::negativeLimit)/VerticalScaler::majorUnit;
    int startMajorMarkIndex = (VerticalNegativeLimit+VerticalScaler::majorUnit-1
            -VerticalScaler::negativeLimit)/VerticalScaler::majorUnit;
    int endMajorMarkIndex =(VerticalPostiveLimit-VerticalScaler::negativeLimit)/VerticalScaler::majorUnit;
    for(int i =0; i<numberMajorMarks; ++i)
    {
        const float* color = VerticalScaler::validColor;
        if(i<startMajorMarkIndex || i>endMajorMarkIndex)
        {
            color = VerticalScaler::invalidColor;
        }
        drawHorizontalLine(verticesVerticalScaler, index, VerticalScaler::bottomMargin + i*majorStep,
                           VerticalScaler::majorWidth, VerticalScaler::rightMargin, color);
    }

    // 次刻度
    float minorStep = VerticalScaler::minorStep;
    int numberMinorMarks = 1+
            (VerticalScaler::postiveLimit-VerticalScaler::negativeLimit)/VerticalScaler::minorUnit;
    int startMinorMarkIndex = (VerticalNegativeLimit+VerticalScaler::minorUnit-1
            -VerticalScaler::negativeLimit)/VerticalScaler::minorUnit;
    int endMinorMarkIndex = (VerticalPostiveLimit-VerticalScaler::negativeLimit)
            /VerticalScaler::minorUnit;
    for(int i=0; i<numberMinorMarks; ++i)
    {
        const float* color = VerticalScaler::validColor;
        if(i<startMinorMarkIndex || i>endMinorMarkIndex)
        {
            color = VerticalScaler::invalidColor;
        }
        drawHorizontalLine(verticesVerticalScaler, index, VerticalScaler::bottomMargin + i*minorStep,
                         VerticalScaler::minorWidth, VerticalScaler::rightMargin, color);
    }

    VerticalPointNumber = index;

    QOpenGLVertexArrayObject::Binder vaoBinder(&vaoVerticalScaler);

    glGenBuffers(1, &vboVerticalScaler);

    glBindBuffer(GL_ARRAY_BUFFER, vboVerticalScaler);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesVerticalScaler),
                 verticesVerticalScaler, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ScaleMarks::drawHorizontalLine(float* vertices, int &index, float bottomMargin, float leftMargin, float rightMargin, const float *color)
{
    vertices[index++] = leftMargin;
    vertices[index++] = bottomMargin;
    vertices[index++] = 0.0f;
    vertices[index++] = color[0];
    vertices[index++] = color[1];
    vertices[index++] = color[2];

    vertices[index++] = rightMargin;
    vertices[index++] = bottomMargin;
    vertices[index++] = 0.0f;
    vertices[index++] = color[0];
    vertices[index++] = color[1];
    vertices[index++] = color[2];
}

void ScaleMarks::drawVerticalLine(float* vertices, int &index, float rightMargin, float topMargin, float bottomMargin, const float *color)
{
    vertices[index++] = rightMargin;
    vertices[index++] = bottomMargin;
    vertices[index++] = 0.0f;
    vertices[index++] = color[0];
    vertices[index++] = color[1];
    vertices[index++] = color[2];

    vertices[index++] = rightMargin;
    vertices[index++] = topMargin;
    vertices[index++] = 0.0f;
    vertices[index++] = color[0];
    vertices[index++] = color[1];
    vertices[index++] = color[2];
}

int ScaleMarks::getVerticalPointNumber() const
{
    return VerticalPointNumber;
}

void ScaleMarks::setVerticalPointNumber(int value)
{
    VerticalPointNumber = value;
}

int ScaleMarks::getVerticalPostiveLimit() const
{
    return VerticalPostiveLimit;
}

void ScaleMarks::setVerticalPostiveLimit(int value)
{
    VerticalPostiveLimit = value;
}

int ScaleMarks::getVerticalNegativeLimit() const
{
    return VerticalNegativeLimit;
}

void ScaleMarks::setVerticalNegativeLimit(int value)
{
    VerticalNegativeLimit = value;
}

int ScaleMarks::getHorizontalPointNumber() const
{
    return HorizontalPointNumber;
}

void ScaleMarks::setHorizontalPointNumber(int value)
{
    HorizontalPointNumber = value;
}

int ScaleMarks::getHorizontalPostiveLimit() const
{
    return HorizontalPostiveLimit;
}

void ScaleMarks::setHorizontalPostiveLimit(int value)
{
    HorizontalPostiveLimit = value;
}

int ScaleMarks::getHorizontalNegativeLimit() const
{
    return HorizontalNegativeLimit;
}

void ScaleMarks::setHorizontalNegativeLimit(int value)
{
    HorizontalNegativeLimit = value;
}

float HorizontalScaler::value2Position(float value)
{
    return HorizontalScaler::leftMargin+
            ((value-HorizontalScaler::negativeLimit)*length
             /(HorizontalScaler::postiveLimit-HorizontalScaler::negativeLimit));
}

float VerticalScaler::value2Position(float value)
{
    return VerticalScaler::bottomMargin +
            ((value-VerticalScaler::negativeLimit)*length
             /(VerticalScaler::postiveLimit-VerticalScaler::negativeLimit));
}

void IndicatorMark::initialize()
{
    initializeOpenGLFunctions();
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
//    glEnable(GL_LINE_SMOOTH);
//    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

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

    initializeHorizontalIndicator();
    initializeVerticalIndicator();
}

void IndicatorMark::render()
{
    m_program.bind();
    {
       QOpenGLVertexArrayObject::Binder vaoBinderH(&vaoIndicatorH);
       int moveX = m_program.uniformLocation("offset");
       glUniform2f(moveX, offsetX, 0.0f);
       glDrawArrays(GL_TRIANGLES, 0, 3);
       vaoBinderH.release();

       QOpenGLVertexArrayObject::Binder vaoBinderV(&vaoIndicatorV);
       int moveY = m_program.uniformLocation("offset");
       glUniform2f(moveY, 0.0f, offsetY);
       glDrawArrays(GL_TRIANGLES, 0, 3);
       vaoBinderV.release();

    }
    m_program.release();
}

void IndicatorMark::updateAngle(float angleX, float angleY)
{
    offsetX = HorizontalScaler::length * angleX/
            (HorizontalScaler::postiveLimit-HorizontalScaler::negativeLimit);
    offsetY = VerticalScaler::length * angleY/
            (VerticalScaler::postiveLimit-VerticalScaler::negativeLimit);
}

void IndicatorMark::initializeHorizontalIndicator()
{
    QOpenGLVertexArrayObject::Binder vaoBinder(&vaoIndicatorH);

    glGenBuffers(1, &vboIndicatorH);

    glBindBuffer(GL_ARRAY_BUFFER, vboIndicatorH);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesIndicatorH),
                 verticesIndicatorH, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    int move = m_program.uniformLocation("offset");
    glUniform2f(move, offsetX, 0.0f);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void IndicatorMark::initializeVerticalIndicator()
{
    QOpenGLVertexArrayObject::Binder vaoBinder(&vaoIndicatorV);

    glGenBuffers(1, &vboIndicatorV);

    glBindBuffer(GL_ARRAY_BUFFER, vboIndicatorV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesIndicatorV),
                 verticesIndicatorV, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    int move = m_program.uniformLocation("offset");
    glUniform2f(move, 0.0f, offsetY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PointMark::initialize()
{
    initializeOpenGLFunctions();
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

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

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), reinterpret_cast<void*>(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    int move = m_program.uniformLocation("offset");
    glUniform2f(move, posX, posY);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PointMark::render()
{
    m_program.bind();
    {
        QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
        int move = m_program.uniformLocation("offset");
        glUniform2f(move, posX, posY);
        glDrawArrays(GL_POINTS, 0, 1);
    }
    m_program.release();
}

void PointMark::updatePosition(float x, float y)
{
    posX = x;
    posY = y;
}

void PointMark::setColor(float r, float g, float b)
{
    vertices[3] = r;
    vertices[4] = g;
    vertices[5] = b;
}

void CrossMark::initialize()
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

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CrossMark::render()
{
    m_program.bind();
    {
        QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
        glDrawArrays(GL_LINES, 0, 16);
        glDrawArrays(GL_POINTS, 16, 1);
    }
    m_program.release();
}

void RectMark::initialize()
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
    glGenBuffers(1, &veo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    int color = m_program.uniformLocation("color");
    glUniform3f(color, colorVec[0], colorVec[1], colorVec[2]);

//    updateRectBound(0.2f, -0.2f, -0.2f, 0.2f);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void RectMark::render(const View& view)
{
    m_program.bind();
    {        
        int borderIndex = m_program.uniformLocation("border");
        glUniform2f(borderIndex, view.GLBorder.x, view.GLBorder.y);
        int offsetIndex = m_program.uniformLocation("offset");
        glUniform2f(offsetIndex, view.OffsetGL.x, view.OffsetGL.y);
        int zoomRateIndex= m_program.uniformLocation("zoomRate");
        glUniform1f(zoomRateIndex, view.ZoomRate);
        int color = m_program.uniformLocation("color");
        glUniform3f(color, colorVec[0], colorVec[1], colorVec[2]);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        QOpenGLVertexArrayObject::Binder vaoBinder(&vao);
        glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, nullptr);
    }
    m_program.release();
}

void RectMark::updateRectBound(float top, float bottom, float left, float right)
{
    m_top = top;
    m_bottom = bottom;
    m_left = left;
    m_right = right;

    vertices[0] = m_left;
    vertices[1] = m_top;
    vertices[3] = m_right;
    vertices[4] = m_top;
    vertices[6] = m_right;
    vertices[7] = m_bottom;
    vertices[9] = m_left;
    vertices[10] = m_bottom;
}

void RectMark::setColor(float r, float g, float b)
{
    colorVec[0] = r;
    colorVec[1] = g;
    colorVec[2] = b;
}

}   // namespace render
}   // namespace vtcore


