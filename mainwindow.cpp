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
    toolbar->addAction(QIcon(newpix), "New Shader");
    toolbar->addAction(QIcon(openpix), "Open Shader");
    toolbar->addSeparator();
    QAction *quit = toolbar->addAction(QIcon(quitpix), "Quit");

    connect(quit, &QAction::triggered, qApp, &QApplication::quit);

    // Status bar
    statusBar()->showMessage("Ready");

    // GL Widget
    glWidget = new GLWidget;
    ui -> mainContainer -> addWidget(glWidget);

    // Update text edit with shader source
    ui -> textEdit -> setPlainText(glWidget -> getFragShaderSource());

    // Recompile on button press
    connect(ui -> compileButton, SIGNAL (released()),this, SLOT (handleCompile()));

    connect(ui -> textEdit, SIGNAL (textChanged()),this, SLOT (handleCompile()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleCompile() {

    QString source = ui -> textEdit -> toPlainText();
    bool success = glWidget -> setFragmentShaderSource(source);

    if(success) {
        statusBar()->showMessage("Compiled!");
    } else {
        statusBar()->showMessage("Failed to compile!");
    }
}
