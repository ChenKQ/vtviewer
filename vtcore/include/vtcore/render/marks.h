#ifndef MARKS_H
#define MARKS_H

#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

namespace vtcore
{
namespace render
{

class View;

/**
  * @brief HorizontalScaler 水平尺的一些基本参数，水平尺用于衡量激光器伺服转台水平向偏转角，常量含义如下：
  * 水平尺底部竖直向位置（采用OpenGL的坐标）
  * 水平尺左侧水平向位置（采用OpenGL的坐标）
  * 水平尺右侧水平向位置（采用OpenGL的坐标）
  * 水平尺在控件中的长度（采用OpenGL的坐标）
  * 水平尺主刻度上沿位置（采用OpenGL的坐标）
  * 水平尺次刻度上沿位置（采用OpenGL的坐标）
  *
  * 水平向角度负最大角度（度）
  * 水平向角度正最大角度（度）
  * 主刻度之间角度距离（度）
  * 次刻度之间角度距离（度）
  * 主刻度step （采用OpenGL的坐标）
  * 次刻度step （采用OpenGL的坐标）
  *
  * 水平尺在伺服有效转角范围内的颜色
  * 水平尺在伺服有效转角范围外的颜色
  */
typedef struct HorizontalScaler
{
    static constexpr float bottomMargin = -0.95f;
    static constexpr float leftMargin = -0.81f;
    static constexpr float rightMargin = 0.81f;
    static constexpr float length = rightMargin-leftMargin;
    static constexpr float majorHeight = bottomMargin + 0.03f;
    static constexpr float minorHeight = bottomMargin + 0.02f;

    static constexpr int negativeLimit = -180;
    static constexpr int postiveLimit = 180;
    static constexpr int majorUnit = 90;
    static constexpr int minorUnit = 10;
    static constexpr float majorStep = length*majorUnit/(postiveLimit-negativeLimit);
    static constexpr float minorStep = length*minorUnit/(postiveLimit-negativeLimit);

    static constexpr float validColor[3] {0.0f, 0.0f, 1.0f};
    static constexpr float invalidColor[3] {0.5f, 0.5f, 0.5f};

    /**
     * @brief value2Position : 将角度转换为控件中的水平位置（采用OpenGL的坐标）
     * @param value : 角度值
     * @return 控件中的水平位置（采用OpenGL的坐标）
     */
    static float value2Position(float value);
} HorizontalScaler;

/**
  * @brief VerticalScaler 竖直尺的一些基本参数，竖直尺用于衡量激光器伺服转台俯仰向偏转角，常量含义如下：
  * 竖直尺右侧水平向位置（采用OpenGL的坐标）
  * 竖直尺顶部竖直向位置（采用OpenGL的坐标）
  * 竖直尺底部竖直向位置（采用OpenGL的坐标）
  * 竖直尺在控件中的长度（采用OpenGL的坐标）
  * 竖直尺主刻度左沿位置（采用OpenGL的坐标）
  * 竖直尺次刻度左沿位置（采用OpenGL的坐标）
  *
  * 竖直向角度负最大角度（度）
  * 竖直向角度正最大角度（度）
  * 主刻度之间角度距离（度）
  * 次刻度之间角度距离（度）
  * 主刻度step （采用OpenGL的坐标）
  * 次刻度step （采用OpenGL的坐标）
  *
  * 竖直尺在伺服有效转角范围内的颜色
  * 竖直尺在伺服有效转角范围外的颜色
  */
typedef struct VerticalScaler
{
    static constexpr float rightMargin = 0.96f;
    static constexpr float topMargin = 0.81f;
    static constexpr float bottomMargin = -0.81f;
    static constexpr float length = topMargin - bottomMargin;
    static constexpr float majorWidth = rightMargin - 0.02f;
    static constexpr float minorWidth = rightMargin - 0.01f;

    static constexpr int negativeLimit = -30;
    static constexpr int postiveLimit = 30;
    static constexpr int majorUnit = 10;
    static constexpr int minorUnit = 1;
    static constexpr float majorStep = length*majorUnit/(postiveLimit-negativeLimit);
    static constexpr float minorStep = length*minorUnit/(postiveLimit-negativeLimit);

    static constexpr float validColor[3] {0.0f, 0.0f, 1.0f};
    static constexpr float invalidColor[3] {0.5f, 0.5f, 0.5f};

    static float value2Position(float value);
} VerticalScaler;

/**
 * @brief The ScaleMarks class : 绘制水平尺和竖直尺
 * ```
 *  ScaleMarks mark;
 *  mark.initialize();
 *
 *  在paintGL()方法中调用
 *  mark.render();
 * ```
 */
class ScaleMarks : public QOpenGLFunctions
{
public:
    ScaleMarks();

    void initialize();
    void render();

    int getHorizontalNegativeLimit() const;
    void setHorizontalNegativeLimit(int value);

    int getHorizontalPostiveLimit() const;
    void setHorizontalPostiveLimit(int value);

    int getHorizontalPointNumber() const;
    void setHorizontalPointNumber(int value);

    int getVerticalNegativeLimit() const;
    void setVerticalNegativeLimit(int value);

    int getVerticalPostiveLimit() const;
    void setVerticalPostiveLimit(int value);

    int getVerticalPointNumber() const;
    void setVerticalPointNumber(int value);

private:
    void initialHorizontalScaler();
    void initialVerticalScaler();

    void drawHorizontalLine(float* vertices, int& index, float bottomMargin, float leftMargin, float rightMargin,
                            const float* color);
    void drawVerticalLine(float* vertices, int& index, float rightMargin, float topMargin, float bottomMargin,
                          const float* color);
private:
    QOpenGLShaderProgram m_program;
    QString vertexShaderFile {":/shaders/shaders/marks.vert"};
    QString fragmentShaderFile{":/shaders/shaders/marks.frag"};

    // horizontal scaler
    int HorizontalNegativeLimit = -170;
    int HorizontalPostiveLimit = 120;
    int HorizontalPointNumber = 0;

    QOpenGLVertexArrayObject vaoHorizontalScaler;
    unsigned int vboHorizontalScaler;
    float verticesHorizontalScaler[1000] {0.0f};

    // vertical scaler
    int VerticalNegativeLimit = -14;
    int VerticalPostiveLimit = 28;
    int VerticalPointNumber = 0;

    QOpenGLVertexArrayObject vaoVerticalScaler;
    unsigned int vboVerticalScaler;
    float verticesVerticalScaler[1000] {0.0f};
};

/**
 * @brief The IndicatorMark class : 角度指示绘制
 */
class IndicatorMark: public QOpenGLFunctions
{
public:
    IndicatorMark() = default;

    void initialize();
    void render();

    // 更新角度
    void updateAngle(float angleX, float angleY);

private:
    void initializeHorizontalIndicator();
    void initializeVerticalIndicator();

private:
    QOpenGLShaderProgram m_program;
    QString vertexShaderFile {":/shaders/shaders/indicator.vert"};
    QString fragmentShaderFile{":/shaders/shaders/indicator.frag"};

    // indicator
    float offsetX = 0.0f;
    float offsetY = 0.0f;

    // horizontal indicator
    QOpenGLVertexArrayObject vaoIndicatorH;
    unsigned int vboIndicatorH;
    float verticesIndicatorH[9]
    {
        0.0f, HorizontalScaler::bottomMargin, 1.0f,
        (-1.0f-HorizontalScaler::bottomMargin)*0.4f, -1.0f, 1.0f,
        (1.0f+HorizontalScaler::bottomMargin)*0.4f, -1.0f, 1.0f,
    };

    // vertical indicator
    QOpenGLVertexArrayObject vaoIndicatorV;
    unsigned int vboIndicatorV;
    float verticesIndicatorV[9]
    {
        VerticalScaler::rightMargin, 0.0f, 1.0f,
        1.0f, (VerticalScaler::rightMargin-1.0f), 1.0f,
        1.0f, (1.0f-VerticalScaler::rightMargin), 1.0f
    };
};

/**
 * @brief The PointMark class : 点绘制
 */
class PointMark: public QOpenGLFunctions
{
public:
    PointMark() = default;

    void initialize();
    void render();
    void updatePosition(float x, float y);
    void setColor(float r, float g, float b);
private:
    QOpenGLShaderProgram m_program;
    QString vertexShaderFile {":/shaders/shaders/point.vert"};
    QString fragmentShaderFile{":/shaders/shaders/point.frag"};

    QOpenGLVertexArrayObject vao;
    unsigned int vbo;
    float vertices[6]
    {
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f
    };

    float posX = 0;
    float posY = 0;
};

/**
 * @brief The CrossMark class : 绘制cross
 */
class CrossMark: public QOpenGLFunctions
{
public:
    CrossMark() = default;

    void initialize();
    void render();
private:
    QOpenGLShaderProgram m_program;
    QString vertexShaderFile {":/shaders/shaders/cross.vert"};
    QString fragmentShaderFile{":/shaders/shaders/cross.frag"};

    QOpenGLVertexArrayObject vao;
    unsigned int vbo;
    float vertices[17*3]
    {
        -0.34f, 0.0f, 1.0f,
        -0.18f, 0.0f, 1.0f,
        -0.16f, 0.0f, 1.0f,
        -0.01f, 0.0f, 1.0f,
        0.01f, 0.0f, 1.0f,
        0.16f, 0.0f, 1.0f,
        0.18f, 0.0f, 1.0f,
        0.34f, 0.0f, 1.0f,

        0.0f, -0.34f, 1.0f,
        0.0f, -0.18f, 1.0f,
        0.0f, -0.16f, 1.0f,
        0.0f, -0.01f, 1.0f,
        0.0f, 0.01f, 1.0f,
        0.0f, 0.16f, 1.0f,
        0.0f, 0.18f, 1.0f,
        0.0f, 0.34f, 1.0f,

        0.0f, 0.0f, 1.0f
    };
};

/**
 * @brief The RectMark class : 绘制矩形
 */
class RectMark: public QOpenGLFunctions
{
public:
    RectMark() = default;

    void initialize();
    void render(const View& view);
    void updateRectBound(float top, float bottom, float left, float right);
    void setColor(float r, float g, float b);
private:
    float m_top = 0.4f;
    float m_bottom = -0.2f;
    float m_left = -0.2f;
    float m_right = 0.2f;
private:
    QOpenGLShaderProgram m_program;
    QString vertexShaderFile {":/shaders/rect.vert"};
    QString fragmentShaderFile{":/shaders/rect.frag"};

    QOpenGLVertexArrayObject vao;
    unsigned int vbo, veo;
    float vertices[4*3] {0.0f};

    unsigned int indices[8]
    {
        0,1,
        1,2,
        2,3,
        3,0
    };

    float colorVec[3] {0.9f, 0.5f, 0.0f};
};

}   // namespace render
}   // namespace vtcore

#endif // MARKS_H
