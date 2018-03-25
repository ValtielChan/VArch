#include <Tree.h>
#include <Scene.h>
#include <Mesh.h>
#include <DirectionalLight.h>
#include <OrbitalManipulator.h>

#include "treegenerator.h"
#include "ui_mainwindow.h"
#include <QMainWindow>

TreeGenerator::TreeGenerator(Ui::MainWindow *ui) : ref_ui(ui)
{

}

TreeGenerator::~TreeGenerator()
{

}

void TreeGenerator::generateTree()
{
    ref_ui->openGLWidget->makeCurrent();

    // Get UI form datas ==

    TreeParameters treeParams(
                ref_ui->trunkWidth->value(),
                ref_ui->trunkHeight->value(),
                ref_ui->nbBranches->value()
            );

    VoxelOctreeLOD lod(7, 4, 20, false);

    m_tree = new Tree(treeParams, lod);

    // (Re)Build scene ==

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

    Mesh* treeMesh = m_tree->mesh();
    Object* object = new Object();
    object->addComponent(treeMesh);

    scene->addObject(object);
    scene->addLight(light);
}
