#ifndef PLANETGENERATOR_H
#define PLANETGENERATOR_H

#include <QObject>

class Planet;

namespace Ui {
class MainWindow;
}

class PlanetGenerator : public QObject
{
    Q_OBJECT

private:

    Ui::MainWindow* ref_ui;
    Planet* m_planet;

public:
    PlanetGenerator(Ui::MainWindow* ui);
    ~PlanetGenerator();

public slots:

    void generatePlanet();
};

#endif // PLANETGENERATOR_H
