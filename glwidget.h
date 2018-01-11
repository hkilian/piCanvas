
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QTime>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram);
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void rotateBy(int xAngle, int yAngle, int zAngle);
    void setClearColor(const QColor &color);

    QString getFragShaderSource();
    QString getVertexShaderSource();

    void setVertexShaderSource(QString source);
    bool setFragmentShaderSource(QString source);

signals:
    void clicked();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void handleUpdate();
    void makeObject();

    QColor clearColor;
    QPoint lastPos;
    int xRot;
    int yRot;
    int zRot;
    QOpenGLTexture *textures[6];
    QOpenGLShaderProgram *program;
    QOpenGLBuffer vbo;

    QString m_fragshader;
    QOpenGLShader *fshader;

    QString m_vertshader;
    QOpenGLShader *vshader;

    QTimer m_updatetimer;
    QTime m_time;

    float m_zoom = 0.2;

};

#endif
