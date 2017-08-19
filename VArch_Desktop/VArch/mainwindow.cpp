
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "src/planetgenerator.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Instanciate modules
    m_planetGenerator = new PlanetGenerator(ui);

    // Connect signals
    connect(ui->pb_generatePlanet, SIGNAL(clicked()), m_planetGenerator, SLOT(generatePlanet()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
