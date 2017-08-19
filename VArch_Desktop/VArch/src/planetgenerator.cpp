#include <Planet.h>
#include <Scene.h>
#include <Mesh.h>
#include <DirectionalLight.h>
#include <OrbitalManipulator.h>
#include <Noise.h>

#include "planetgenerator.h"
#include "ui_mainwindow.h"
#include <QMainWindow>

PlanetGenerator::PlanetGenerator(Ui::MainWindow* ui) : ref_ui(ui)
{

}

PlanetGenerator::~PlanetGenerator()
{

}

void PlanetGenerator::generatePlanet()
{
    ref_ui->openGLWidget->makeCurrent();

    // Get UI form datas ==

    // Perlin noise
    float frequency = ref_ui->frequency->value();
    float scale = ref_ui->scale->value();
    int octaves = ref_ui->octaves->value();

    // Properties
    int nbChunk = ref_ui->nbChunk->value();
    int depth = ref_ui->depth->value();
    float meshScale = ref_ui->meshScale->value();

    // Planet generation ==

    NoiseProperties np = NoiseProperties(frequency * nbChunk, scale, octaves);
    CubeMap cubeMap(pow(2, depth) * nbChunk * 2);
    cubeMap.generateSimplex(np);

    ColorTable colorTable = ColorTable::Nature(128);
    m_planet = new Planet (cubeMap, colorTable, depth, nbChunk);

    // Build scene ==

    Scene* scene = Scene::getInstance();
    scene->clearScene();

    Camera* camera = scene->getCamera();

    if (!dynamic_cast<OrbitalManipulator*>(camera->cameraManipulator())) {
        OrbitalManipulator* orbitalManipulator = new OrbitalManipulator(camera);
        camera->setManipulator(orbitalManipulator);
        camera->setSensitivity(.2f);
        camera->setSpeed(30);
    }

    DirectionalLight* light = new DirectionalLight;
    light->direction = glm::vec3(-1, -1, -1);
    light->diffuse = glm::vec3(.5f);

    Object* object = new Object();
    m_planet->addMeshesToObject(object);

    scene->addObject(object);
    scene->addLight(light);
}
