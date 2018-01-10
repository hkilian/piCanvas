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

}

MainWindow::~MainWindow()
{
    delete ui;
}
