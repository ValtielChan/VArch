#include <DefaultRenderer.h>
#include <Mesh.h>
#include <Scene.h>
#include <PhongMaterial.h>
#include <DirectionalLight.h>

#include "openglwidget.h"

#include <QOpenGLWidget>
#include <QTimer>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QMouseEvent>

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    // Qt stuff
    setFocusPolicy(Qt::StrongFocus);
    installEventFilter(this);
    makeCurrent();

    // FPS Timer
    int seconde = 1000;
    int timerInterval = seconde / 120;

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(frameTimeOut()));
    m_timer->start( timerInterval );

    for (int i = 0; i < 1024; i++) {
        m_keys[i] = false;
    }
}

void OpenGLWidget::initializeGL()
{
    glewExperimental = GL_TRUE;

    GLenum err;
    if ((err = glewInit()) != GLEW_OK)
        std::cout << glewGetErrorString(err) << std::endl;

    m_renderer = DefaultRenderer::getInstance();

    Camera* camera = m_renderer->getCamera();
    glViewport(0, 0, camera->screenWidth(), camera->screenHeight());

    glEnable(GL_DEPTH_TEST);

    createTestScene();
}

void OpenGLWidget::resizeGL(int width, int height)
{
    Camera* camera = m_renderer->getCamera();

    glViewport(0, 0, width, height);

    camera->setScreenSize((GLfloat)this->width(), (GLfloat)this->height());
}

void OpenGLWidget::paintGL()
{
    makeCurrent();

    m_renderer->render();
}

void OpenGLWidget::createTestScene()
{
    makeCurrent();

    Scene* scene = Scene::getInstance();

    // Create a cube
    PhongMaterial* cubeMaterial = new PhongMaterial();

    Mesh* cubeMesh = new Mesh(cubeMaterial);
    cubeMesh->generateCube();

    Object* cube = new Object();
    cube->addComponent(cubeMesh);

    // Light
    DirectionalLight* dirLight = new DirectionalLight();
    dirLight->direction = glm::vec3(-1, -1, -1);
    dirLight->diffuse = glm::vec3(1);

    // Add to scene
    scene->addLight(dirLight);
    scene->addObject(cube);
}

// Events

void OpenGLWidget::moveCamera()
{
    GLfloat deltaTime = (m_timer->interval() / 1000.0f) * 2;

    Camera* camera = m_renderer->getCamera();


    if (m_keys[Qt::Key_Z]) {
        camera->processMove(CameraMovement::FORWARD, deltaTime);
    }
    if (m_keys[Qt::Key_S]) {
        camera->processMove(CameraMovement::BACKWARD, deltaTime);
    }
    if (m_keys[Qt::Key_Q]) {
        camera->processMove(CameraMovement::LEFTWARD, deltaTime);
    }
    if (m_keys[Qt::Key_D]) {
        camera->processMove(CameraMovement::RIGHTWARD, deltaTime);
    }
    //emit cameraPosChanged(camera.position.x, camera.position.y, camera.position.z);
}

void OpenGLWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() < 1024)
        m_keys[keyEvent->key()] = true;

    switch (keyEvent->key()) {

        case Qt::Key_W:
            makeCurrent();
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case Qt::Key_F:
            makeCurrent();
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
    }
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->key() < 1024)
        m_keys[keyEvent->key()] = false;
}

void OpenGLWidget::mousePressEvent(QMouseEvent *mouseEvent)
{

}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    Camera* camera = m_renderer->getCamera();

    //Camera orientation
    if (mouseEvent->buttons() & Qt::LeftButton) {

        setCursor(QCursor(Qt::BlankCursor));

        int xPos = mouseEvent->pos().x();
        int yPos = mouseEvent->pos().y();

        if (m_firstMouse) {
            m_lastX = xPos;
            m_lastY = yPos;
            m_firstMouse = false;
        }

        GLfloat xoffset = xPos - m_lastX;
        GLfloat yoffset = m_lastY - yPos;  // Reversed since y-coordinates go from bottom to left

        m_lastX = xPos;
        m_lastY = yPos;

        camera->processLook(xoffset, yoffset);
    }

    //Camera move forward/backward
    if (mouseEvent->buttons() & Qt::LeftButton) {
        //std::cout << "Right click and move!" << std::endl;
    }
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    m_firstMouse = true;
    setCursor(QCursor(Qt::CrossCursor));
}

void OpenGLWidget::frameTimeOut()
{
    update();
    moveCamera();
}
