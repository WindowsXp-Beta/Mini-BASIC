#include "guibasic.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    setbuf(stdout, NULL);
    GuiBasic w;
    w.show();
    return a.exec();
}
