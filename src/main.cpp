#include "clippercore.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    ClipperCore *core = new ClipperCore();
    Q_UNUSED(core);

    return app.exec();
}
