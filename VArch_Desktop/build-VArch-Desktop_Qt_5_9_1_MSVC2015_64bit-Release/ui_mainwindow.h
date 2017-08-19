/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include "openglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionVersion;
    QWidget *centralWidget;
    OpenGLWidget *openGLWidget;
    QTabWidget *tabWidget;
    QWidget *planetTab;
    QGroupBox *groupBox_2;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QDoubleSpinBox *frequency;
    QLabel *label_2;
    QLabel *label_3;
    QDoubleSpinBox *scale;
    QSpinBox *octaves;
    QGroupBox *groupBox_3;
    QWidget *formLayoutWidget_2;
    QFormLayout *formLayout_2;
    QLabel *label_4;
    QSpinBox *nbChunk;
    QLabel *label_5;
    QSpinBox *depth;
    QLabel *label_6;
    QSpinBox *meshScale;
    QPushButton *pb_generatePlanet;
    QWidget *terrainTab;
    QWidget *treeTab;
    QGroupBox *groupBox;
    QWidget *formLayoutWidget_3;
    QFormLayout *formLayout_3;
    QLabel *label_7;
    QLabel *label_8;
    QSlider *horizontalSlider;
    QSlider *horizontalSlider_2;
    QMenuBar *menuBar;
    QMenu *menuaBOUT;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1095, 693);
        actionVersion = new QAction(MainWindow);
        actionVersion->setObjectName(QStringLiteral("actionVersion"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        openGLWidget = new OpenGLWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setEnabled(true);
        openGLWidget->setGeometry(QRect(10, 10, 821, 491));
        openGLWidget->setCursor(QCursor(Qt::CrossCursor));
        openGLWidget->setFocusPolicy(Qt::StrongFocus);
        openGLWidget->setAcceptDrops(true);
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setGeometry(QRect(840, 10, 251, 641));
        tabWidget->setTabShape(QTabWidget::Rounded);
        planetTab = new QWidget();
        planetTab->setObjectName(QStringLiteral("planetTab"));
        groupBox_2 = new QGroupBox(planetTab);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 10, 231, 121));
        formLayoutWidget = new QWidget(groupBox_2);
        formLayoutWidget->setObjectName(QStringLiteral("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 30, 211, 91));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(formLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        frequency = new QDoubleSpinBox(formLayoutWidget);
        frequency->setObjectName(QStringLiteral("frequency"));
        frequency->setSingleStep(0.1);
        frequency->setValue(1);

        formLayout->setWidget(0, QFormLayout::FieldRole, frequency);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        label_3 = new QLabel(formLayoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        scale = new QDoubleSpinBox(formLayoutWidget);
        scale->setObjectName(QStringLiteral("scale"));
        scale->setSingleStep(0.1);
        scale->setValue(0.5);

        formLayout->setWidget(1, QFormLayout::FieldRole, scale);

        octaves = new QSpinBox(formLayoutWidget);
        octaves->setObjectName(QStringLiteral("octaves"));
        octaves->setMaximum(20);
        octaves->setValue(4);

        formLayout->setWidget(2, QFormLayout::FieldRole, octaves);

        groupBox_3 = new QGroupBox(planetTab);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 150, 231, 311));
        formLayoutWidget_2 = new QWidget(groupBox_3);
        formLayoutWidget_2->setObjectName(QStringLiteral("formLayoutWidget_2"));
        formLayoutWidget_2->setGeometry(QRect(10, 30, 211, 109));
        formLayout_2 = new QFormLayout(formLayoutWidget_2);
        formLayout_2->setSpacing(6);
        formLayout_2->setContentsMargins(11, 11, 11, 11);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        formLayout_2->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(formLayoutWidget_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_4);

        nbChunk = new QSpinBox(formLayoutWidget_2);
        nbChunk->setObjectName(QStringLiteral("nbChunk"));
        nbChunk->setMinimum(1);

        formLayout_2->setWidget(0, QFormLayout::FieldRole, nbChunk);

        label_5 = new QLabel(formLayoutWidget_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_5);

        depth = new QSpinBox(formLayoutWidget_2);
        depth->setObjectName(QStringLiteral("depth"));
        depth->setMinimum(1);
        depth->setMaximum(7);
        depth->setValue(4);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, depth);

        label_6 = new QLabel(formLayoutWidget_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_6);

        meshScale = new QSpinBox(formLayoutWidget_2);
        meshScale->setObjectName(QStringLiteral("meshScale"));
        meshScale->setMinimum(1);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, meshScale);

        pb_generatePlanet = new QPushButton(groupBox_3);
        pb_generatePlanet->setObjectName(QStringLiteral("pb_generatePlanet"));
        pb_generatePlanet->setGeometry(QRect(10, 270, 211, 31));
        tabWidget->addTab(planetTab, QString());
        terrainTab = new QWidget();
        terrainTab->setObjectName(QStringLiteral("terrainTab"));
        tabWidget->addTab(terrainTab, QString());
        treeTab = new QWidget();
        treeTab->setObjectName(QStringLiteral("treeTab"));
        tabWidget->addTab(treeTab, QString());
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setEnabled(false);
        groupBox->setGeometry(QRect(10, 510, 411, 141));
        formLayoutWidget_3 = new QWidget(groupBox);
        formLayoutWidget_3->setObjectName(QStringLiteral("formLayoutWidget_3"));
        formLayoutWidget_3->setGeometry(QRect(10, 30, 391, 101));
        formLayout_3 = new QFormLayout(formLayoutWidget_3);
        formLayout_3->setSpacing(6);
        formLayout_3->setContentsMargins(11, 11, 11, 11);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        formLayout_3->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(formLayoutWidget_3);
        label_7->setObjectName(QStringLiteral("label_7"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, label_7);

        label_8 = new QLabel(formLayoutWidget_3);
        label_8->setObjectName(QStringLiteral("label_8"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_8);

        horizontalSlider = new QSlider(formLayoutWidget_3);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, horizontalSlider);

        horizontalSlider_2 = new QSlider(formLayoutWidget_3);
        horizontalSlider_2->setObjectName(QStringLiteral("horizontalSlider_2"));
        horizontalSlider_2->setOrientation(Qt::Horizontal);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, horizontalSlider_2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1095, 20));
        menuaBOUT = new QMenu(menuBar);
        menuaBOUT->setObjectName(QStringLiteral("menuaBOUT"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuaBOUT->menuAction());
        menuaBOUT->addAction(actionVersion);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionVersion->setText(QApplication::translate("MainWindow", "Version", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Perlin Noise", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Frequency", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Scale", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "Octaves", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Properties", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "Side Chunks \302\262", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "Octree Depth", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "Mesh Scale", Q_NULLPTR));
        pb_generatePlanet->setText(QApplication::translate("MainWindow", "Generate", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(planetTab), QApplication::translate("MainWindow", "Planet", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(terrainTab), QApplication::translate("MainWindow", "Terrain", Q_NULLPTR));
        tabWidget->setTabText(tabWidget->indexOf(treeTab), QApplication::translate("MainWindow", "Tree", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "Camera", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "TextLabel", Q_NULLPTR));
        menuaBOUT->setTitle(QApplication::translate("MainWindow", "About", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
