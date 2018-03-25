#ifndef CAMERACONFIG_H
#define CAMERACONFIG_H

#include <QObject>

#include "ui_mainwindow.h"

class Camera;

namespace Ui {
class MainWindow;
}

class CameraConfig : public QObject
{
    Q_OBJECT

private:

    Ui::MainWindow* ref_ui;
    Camera* ref_camera;

public:

    CameraConfig(Ui::MainWindow* ui);

    void updateCamera();
public slots:

    void changeCameraSensitivity(int value);
    void changeCameraFOV(int value);
    void changeCameraSpeed(int value);
};

#endif // CAMERACONFIG_H
