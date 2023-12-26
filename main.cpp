#include "kavach.h"

#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Kavach w;
    w.show();
    return a.exec();
}
