#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include <QMenuBar>
#include <QStatusBar>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    // Images
    QPixmap newpix("new.png");
    QPixmap openpix("open.png");
    QPixmap quitpix("quit.png");

    // Toolbar
    QToolBar *toolbar = addToolBar("main toolbar");

    QAction *newProject = toolbar->addAction(QIcon(newpix), "New Project");
    toolbar->addAction(QIcon(openpix), "Open Shader");
    connect(newProject, SIGNAL(triggered(bool)), this, SLOT(newProject()));

    toolbar->addSeparator();
    QAction *quit = toolbar->addAction(QIcon(quitpix), "Quit");
    connect(quit, &QAction::triggered, qApp, &QApplication::quit);

    // Status bar
    statusBar()->showMessage("Ready");

    // GL Widget
    glWidget = new GLWidget;
    ui -> mainContainer -> addWidget(glWidget);

    // Update text edit with shader source
    ui -> textEditFragment -> setPlainText(glWidget -> getFragShaderSource());
    ui -> textEditVertex -> setPlainText(glWidget -> getVertexShaderSource());

    // Recompile on button press or text change
    connect(ui -> textEditFragment, SIGNAL (textChanged()),this, SLOT (handleCompile()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleCompile() {

    QString source = ui -> textEditFragment -> toPlainText();
    bool success = glWidget -> setFragmentShaderSource(source);

    if(success) {
        statusBar()->showMessage("Compiled!");
    } else {
        statusBar()->showMessage("Failed to compile!");
    }
}

void MainWindow::newProject() {

    qDebug() << "NEW PROJECT!";

}
