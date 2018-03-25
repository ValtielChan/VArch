#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class PlanetGenerator;
class TreeGenerator;
class CameraConfig;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    PlanetGenerator* m_planetGenerator;
    TreeGenerator* m_treeGenerator;

    CameraConfig* m_cameraConfig;
};

#endif // MAINWINDOW_H
