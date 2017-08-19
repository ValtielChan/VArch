#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>

class DefaultRenderer;

class QKeyEvent;
class QMouseEvent;
class QOpenGLWidget;
class QTimer;
class QElapsedTimer;

class OpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT

private:

    DefaultRenderer *m_renderer;
    QTimer *m_timer;

    // Events
    bool m_keys[1024];
    bool m_firstMouse;
    int m_lastX, m_lastY;

public slots:
    void frameTimeOut();
    void createTestScene();

public:
    OpenGLWidget(QWidget *parent = 0);

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();



    //Events
    void moveCamera();
    void keyPressEvent(QKeyEvent *keyEvent);
    void keyReleaseEvent(QKeyEvent *keyEvent);
    void mousePressEvent(QMouseEvent *mouseEvent);
    void mouseMoveEvent(QMouseEvent *mouseEvent);
    void mouseReleaseEvent(QMouseEvent *mouseEvent);
};

#endif // OPENGLWIDGET_H
