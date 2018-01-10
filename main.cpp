#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setStyle("gtk");
    QApplication a(argc, argv);
    MainWindow window;

    window.resize(800, 500);
    window.setWindowTitle("piCanvas");

    window.setToolTip("QWidget");
    window.setWindowIcon(QIcon("web.png"));

    window.show();

    return a.exec();

}
