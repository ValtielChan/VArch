#define __QML__

#ifdef __QML__

#include <QtQuick/QQuickView>
#include <QGuiApplication>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    //qmlRegisterType<PieChart>("Charts", 1, 0, "PieChart");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("VArch_Desktop/VArch/qml/VArch.qml"));
    view.show();
    return app.exec();
}

#else

#include <QApplication>
#include <QFile>

#include "mainwindow.h"
#include "openglwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":darkstyle/style.qss");
    file.open(QFile::ReadOnly);
    QString StyleSheet = QLatin1String(file.readAll());

    //qApp->setStyle("plastique");
    qApp->setStyleSheet(StyleSheet);

    MainWindow w;
    w.show();

    return a.exec();
}

#endif
