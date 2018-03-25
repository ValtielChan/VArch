
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "src/planetgenerator.h"
#include "src/treegenerator.h"
#include "src/cameraconfig.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(QString("VArch"));

    // Instanciate modules
    m_planetGenerator = new PlanetGenerator(ui);
    m_treeGenerator = new TreeGenerator(ui);

    m_cameraConfig = new CameraConfig(ui);

    // Connect signals
    connect(ui->pb_generatePlanet, SIGNAL(clicked()), m_planetGenerator, SLOT(generatePlanet()));
    connect(ui->pb_generateTree, SIGNAL(clicked()), m_treeGenerator, SLOT(generateTree()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
