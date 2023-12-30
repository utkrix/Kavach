#include "newpass.h"
#include<iostream>
#include "ui_newpass.h"
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
#include <QCryptographicHash>

using namespace std;




newpass::newpass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newpass)
{
    ui->setupUi(this);

    QString directoryPath = QCoreApplication::applicationDirPath() + QDir::separator() + "database";
    QString databasePath = directoryPath + QDir::separator() + "database.db";

    // Check if the directory exists
    QDir directory(directoryPath);
    if (!directory.exists()) {
        // Create the directory
        if (directory.mkpath(".")) {
            qDebug() << "Directory created successfully.";
        } else {
            qWarning() << "Failed to create the directory.";
        }
    } else {
        qDebug() << "Directory already exists.";
    }

    // Check if the database file exists
    QFile databaseFile(databasePath);
    if (!databaseFile.exists()) {
        // Create the database file
        if (databaseFile.open(QIODevice::ReadWrite)) {
            qDebug() << "Database file created successfully.";
            databaseFile.close();
        } else {
            qWarning() << "Failed to create the database file.";
        }
    } else {
        qDebug() << "Database file already exists.";
    }

    // Set up the SQLite database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databasePath);

    if (!db.open()) {
        qWarning() << "Failed to open database:" << db.lastError().text();
    } else {
        qDebug() << "Database opened successfully.";
    }

    // Create tables if they do not exist

    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS credentials (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT NOT NULL UNIQUE, password TEXT NOT NULL);");

    if (query.exec()) {
        qDebug() << "Table credentials created successfully.";
    } else {
        qWarning() << "Failed to create table credentials" << ":" << query.lastError().text();
    }
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
    }
    else{
        QMessageBox::information(this,  "Error", "Password do not match!");
    }



}

void  newpass::strengthCheck(QString str){

    int pass = 0;
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
    {
       pass = 1;
       ui->strengthDisplay->setText("<font color='green'>Strong password!</font>");
    }
    else if((l_case+u_case+digit+special>=3) && l>=8)
        ui->strengthDisplay->setText("<font color='blue'>Moderate strength; consider a strong one!</font>");
    else
        ui->strengthDisplay->setText("<font color='red'>Weak password; please use a strong password!</font>");


    if (pass == 1)
    {
        QByteArray hashedPassword = QCryptographicHash::hash(str.toUtf8(), QCryptographicHash::Sha256).toHex();
        qDebug() << hashedPassword;
    }
}


void newpass::on_savepw_2_clicked()
{
    // go to landing

}

