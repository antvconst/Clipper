#include "clipper.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Clipper *w = new Clipper();
    return a.exec();
}
