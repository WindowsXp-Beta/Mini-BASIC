#include "guibasic.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setbuf(stdout, NULL);
    GuiBasic w;
    QObject::connect(&w, SIGNAL(quit_app()), &a, SLOT(quit()));
    w.show();
    return a.exec();
}
