#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setSamples(16); // fix noise on texture
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow window;
    window.show();
    return app.exec();
}
