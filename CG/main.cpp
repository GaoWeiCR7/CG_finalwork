#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    if(argc == 3)
    {
        QString path = argv[1];
        QString contentpath = argv[2];
        QFile file(path);
        w.getcontentpath(contentpath);
        w.filedeal(file);
    }
    w.show();

    return a.exec();
}
