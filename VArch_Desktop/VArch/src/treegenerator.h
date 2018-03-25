#ifndef TREEGENERATOR_H
#define TREEGENERATOR_H

#include <QObject>

class Tree;

namespace Ui {
class MainWindow;
}

class TreeGenerator : public QObject
{
    Q_OBJECT

private:

    Ui::MainWindow* ref_ui;
    Tree* m_tree;

public:
    TreeGenerator(Ui::MainWindow* ui);
    ~TreeGenerator();

public slots:

    void generateTree();
};

#endif // TREEGENERATOR_H
