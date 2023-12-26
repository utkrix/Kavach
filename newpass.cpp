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
#include <ctype.h>
#include <cctype>
#include <QChar>
#include "sqlite3.h"

using namespace std;

newpass::newpass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newpass)
{
    ui->setupUi(this);

    QSqlDatabase mydb = QSqlDatabase::addDatabase("QSQLITE");
    mydb.setDatabaseName("C:/Users/USER/Documents/Kavach/db/cred.db");
    mydb.open();


    if (!mydb.open())
        ui-> lable -> setText("Not Connected");
    else
        ui->lable->setText("Connected to db!");
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



void newpass::on_savepw_clicked()
{
    QString userName = ui->username->text();
    QString pass = ui->pass->text();
    QString passConf = ui->passConf->text();
    QString site = ui->site->text();
    string strPass;

    if ((ui->username->text().isEmpty()) or  (ui->pass->text().isEmpty()) or (ui->site->text().isEmpty()))
    {
        QMessageBox::information(this,  "Error", "Some fields are missing!");
    }
    if(pass == passConf)
    {

        // strength checker:
        strPass = pass.toStdString();
        strengthCheck(pass);

        // db connection


    }
    else{
        QMessageBox::information(this,  "Error", "Password do not match!");
    }



}

void  newpass::strengthCheck(QString str){
    int l_case=0, u_case=0, digit=0, special=0;

    int l=str.length(),i;

    for (i = 0; i < l; i++) {
        QChar currentChar = str[i];

        if (currentChar.isLower())
            l_case = 1;
        if (currentChar.isUpper())
            u_case = 1;
        if (currentChar.isDigit())
            digit = 1;
        if (!currentChar.isLetterOrNumber())
            special = 1;
    }
    // strength checking

    if(l_case && u_case && digit && special && l>=10)
        ui->strengthDisplay->setText("<font color='green'>Strong password!</font>");

    else if((l_case+u_case+digit+special>=3) && l>=8)
        ui->strengthDisplay->setText("<font color='blue'>Moderate strength; consider a strong one!</font>");
    else
        ui->strengthDisplay->setText("<font color='red'>Weak password; please use a strong password!</font>");


}

void mongoConnect(){
    string atlas, url, env;

}

void newpass::on_savepw_2_clicked()
{
    // go to landing

}

