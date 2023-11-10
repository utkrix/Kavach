#include "newpass.h"
#include "ui_newpass.h"
#include<iostream>
#include<cstdlib>
#include<ctime>
#include <QDebug>
#include <QString>
#include <QRandomGenerator>
#include <QClipboard>
#include <QApplication>
#include <QMessageBox>


using namespace std;

newpass::newpass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newpass)
{
    ui->setupUi(this);
}

newpass::~newpass()
{
    delete ui;
}


void newpass::on_generatePW_clicked()
{
    QString password;
    const char alphanum[] = "0123456789!@#$%^&*abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int string_length = sizeof(alphanum) - 1;

    for (int i = 0; i < 12; i++) {
        char randomChar = alphanum[QRandomGenerator::global()->bounded(string_length)];
        password.append(randomChar);
    }

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(password);

    ui->pass->setText(password);

    QMessageBox::information(this, "Password Copied", "Password copied to clipboard: " + password);



}


