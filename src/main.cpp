#include "clippercore.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    ClipperCore *core = new ClipperCore();
    return app.exec();
}
