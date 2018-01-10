#include "glwidget.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      clearColor(Qt::black),
      xRot(0),
      yRot(0),
      zRot(0),
      program(0)
{
    memset(textures, 0, sizeof(textures));

}

GLWidget::~GLWidget()
{
    makeCurrent();
    vbo.destroy();
    delete program;
    doneCurrent();
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(200, 200);
}

void GLWidget::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;
    update();
}

void GLWidget::setClearColor(const QColor &color)
{
    clearColor = color;
    update();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    makeObject();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

    vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    m_vertshader =
        "attribute highp vec4 vertex;\n"
        "attribute mediump vec4 texCoord;\n"
        "varying mediump vec4 texc;\n"
        "uniform mediump mat4 matrix;\n"
        "void main(void)\n"
        "{\n"
        "    gl_Position = matrix * vertex;\n"
        "    texc = texCoord;\n"
        "}\n";
    vshader->compileSourceCode(m_vertshader);

    fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    m_fragshader =
        "uniform vec2 u_resolution;\n\n"
        "void main(void) {\n\n"
        "    vec2 st = gl_FragCoord.xy/u_resolution;\n"
        "    gl_FragColor = vec4(st.x, st.y, 0.0, 1.0);\n"
        "\n}\n";
    fshader->compileSourceCode(m_fragshader);

    program = new QOpenGLShaderProgram;
    program->addShader(vshader);
    program->addShader(fshader);
    program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    program->link();
    program->bind();
    program->setUniformValue("u_resolution", width(), height());
}

void GLWidget::paintGL()
{
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 m;
    float t = 0.2;
    m.ortho(-t, +t, +t, -t, 4.0f, 25.0f);
    m.translate(0.0f, 0.0f, -10.0f);
    m.rotate(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    m.rotate(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
    m.rotate(zRot / 16.0f, 0.0f, 0.0f, 1.0f);

    program->setUniformValue("matrix", m);
    program->setUniformValue("u_resolution", width(), height());
    program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));

    for (int i = 0; i < 6; ++i) {
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }

}
void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        rotateBy(8 * dy, 8 * dx, 0);
    } else if (event->buttons() & Qt::RightButton) {
        rotateBy(8 * dy, 0, 8 * dx);
    }
    lastPos = event->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
    emit clicked();
}

void GLWidget::makeObject()
{
    static const int coords[6][4][3] = {
        { { +1, -1, -1 }, { -1, -1, -1 }, { -1, +1, -1 }, { +1, +1, -1 } },
        { { +1, +1, -1 }, { -1, +1, -1 }, { -1, +1, +1 }, { +1, +1, +1 } },
        { { +1, -1, +1 }, { +1, -1, -1 }, { +1, +1, -1 }, { +1, +1, +1 } },
        { { -1, -1, -1 }, { -1, -1, +1 }, { -1, +1, +1 }, { -1, +1, -1 } },
        { { +1, -1, +1 }, { -1, -1, +1 }, { -1, -1, -1 }, { +1, -1, -1 } },
        { { -1, -1, +1 }, { +1, -1, +1 }, { +1, +1, +1 }, { -1, +1, +1 } }
    };

    //for (int j = 0; j < 6; ++j)
        //textures[j] = new QOpenGLTexture(QImage(QString(":/images/side%1.png").arg(j + 1)).mirrored());

    QVector<GLfloat> vertData;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            // vertex position
            vertData.append(0.2 * coords[i][j][0]);
            vertData.append(0.2 * coords[i][j][1]);
            vertData.append(0.2 * coords[i][j][2]);
            // texture coordinate
            vertData.append(j == 0 || j == 3);
            vertData.append(j == 0 || j == 1);
        }
    }

    vbo.create();
    vbo.bind();
    vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
}

QString GLWidget::getVertexShaderSource() {
    return m_vertshader;
}

QString GLWidget::getFragShaderSource() {
    return m_fragshader;
}

void GLWidget::setVertexShaderSource(QString source) {
    m_vertshader = source;
}

bool GLWidget::setFragmentShaderSource(QString source) {

    m_fragshader = source;

    //QMutexLocker locker(&m);

    if (program && program->isLinked()) {
        program->removeShader(fshader);
    }

    //program = new QOpenGLShaderProgram();
    fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);

    if (!fshader->compileSourceCode(m_fragshader)) {
        qDebug() << "Error compiling fragment shader:";
        qDebug() << fshader->log();
        return false;
    }

    if (!program->addShader(fshader)) {
        qDebug() << "Failed to add fragment shader:\n" << program->log();
        return false;
    }

    // Compile fragment shader
    if (!program->addShader(vshader)) {
        qDebug() << "Failed to add vertex shader:\n" << program->log();
        return false;
    }

    if (!program->link()) {
        qDebug() << "Failed to link the program\n:" << program->log();
        return false;
    }

    // Update opengl
    update();

    return true;

}
