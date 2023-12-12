#include "kavach.h"
#include "./ui_kavach.h"
#include <iostream>

using namespace std;


Kavach::Kavach(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Kavach)
{
    ui->setupUi(this);
}

Kavach::~Kavach()
{
    delete ui;
}


void Kavach::on_newPw_clicked()
{
    newPass = new newpass(this);
    hide();
    newPass->show();
}

void Kavach::on_oldPw_clicked()
{
    //saved passwords view

}

