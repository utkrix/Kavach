#include"mainwindow.h"
#include"kavach.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Kavach w;
    w.show();
    return a.exec();
}
