#include "savedpw.h"
#include "ui_savedpw.h"

savedpw::savedpw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::savedpw)
{
    ui->setupUi(this);
}

savedpw::~savedpw()
{
    delete ui;
    int a;
    a = 5;
}
