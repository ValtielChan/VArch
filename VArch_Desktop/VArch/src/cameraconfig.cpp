#include <Scene.h>
#include <Camera.h>

#include "cameraconfig.h"

CameraConfig::CameraConfig(Ui::MainWindow *ui) : ref_ui(ui)
{
    connect(ui->slider_cameraFOV, SIGNAL(valueChanged(int)), this, SLOT(changeCameraFOV(int)));
    connect(ui->slider_cameraSensitivity, SIGNAL(valueChanged(int)), this, SLOT(changeCameraSensitivity(int)));
    connect(ui->slider_cameraSpeed, SIGNAL(valueChanged(int)), this, SLOT(changeCameraSpeed(int)));

    ref_camera = Scene::getInstance()->getCamera();

    ui->slider_cameraFOV->setValue(ref_camera->zoom() * 100);
    ui->slider_cameraSensitivity->setValue(ref_camera->sensitivity() * 100);
    ui->slider_cameraSpeed->setValue(ref_camera->movementSpeed());

    ref_ui->lb_cameraFOV->setText(QString::number(ref_camera->zoom()));
    ref_ui->lb_cameraSensitivity->setText(QString::number(ref_camera->sensitivity()));
    ref_ui->lb_cameraSpeed->setText(QString::number(ref_camera->movementSpeed()));
}

void CameraConfig::changeCameraSensitivity(int value)
{
    float fValue = (float)value / 100.f;

    ref_camera->sensitivity() = fValue;
    ref_ui->lb_cameraSensitivity->setText(QString::number(fValue));
}

void CameraConfig::changeCameraFOV(int value)
{
    float fValue = (float)value / 100.f;

    ref_camera->zoom() = fValue;
    ref_ui->lb_cameraFOV->setText(QString::number(fValue));

    ref_camera->updateProjectionMatrix();
}

void CameraConfig::changeCameraSpeed(int value)
{
    ref_camera->movementSpeed() = value;
    ref_ui->lb_cameraSpeed->setText(QString::number(value));
}
