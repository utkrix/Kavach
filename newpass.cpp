#include <QtGlobal>
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
#include <QInputDialog>
#include <QFile>
#include <QIODevice>
#include <QDir>
#include"kavach.h"

using namespace std;

QByteArray generateRandomKey(int keySize) {
    QByteArray randomData;
    randomData.resize(keySize / 8); // Convert bits to bytes

    // Use Qt's random generator
    QRandomGenerator randomGenerator;
    for (int i = 0; i < randomData.size(); ++i) {
        randomData[i] = static_cast<char>(randomGenerator.bounded(256));
    }

    return randomData;
}

void createEncryptionFile(const QString& filePath, int keySize) {
    QFile encFile(filePath);
    if (!encFile.exists()) {
        if (encFile.open(QIODevice::ReadWrite)) {
            qDebug() << "Encryption file created successfully.";

            // Generate a unique value based on timestamp
            QByteArray uniqueValue = QString(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")).toUtf8();

            // Use cryptographic hash to generate a key from the unique value
            QByteArray encryptionKey = QCryptographicHash::hash(uniqueValue, QCryptographicHash::Sha256);
            qDebug() << encryptionKey;
            qint64 bytesWritten = encFile.write(encryptionKey);
            if (bytesWritten == keySize / 8) {
                qDebug() << "Encryption key written to the encryption file.";
            } else {
                qWarning() << "Failed to write the encryption key to the encryption file.";
            }

            encFile.close();
        } else {
            qWarning() << "Failed to create the encryption file.";
        }
    } else {
        qDebug() << "Encryption file already exists.";
    }
}

QByteArray encryptPassword(const QByteArray& password, const QByteArray& encryptionKey) {
    // Use cryptographic hash to derive a key from the encryption key
    QByteArray derivedKey = QCryptographicHash::hash(encryptionKey, QCryptographicHash::Sha256);

    // Use XOR operation for simple encryption (You may want to use a more secure algorithm)
    QByteArray encryptedPassword;
    for (int i = 0; i < password.size(); ++i) {
        encryptedPassword.append(password.at(i) ^ derivedKey.at(i % derivedKey.size()));
    }

    return encryptedPassword.toHex(); // Convert to hex for storage
}

newpass::newpass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newpass)
{
    ui->setupUi(this);

    QString directoryPath = QCoreApplication::applicationDirPath() + QDir::separator() + "database";
    QString databasePath = directoryPath + QDir::separator() + "database.db";
    QString encDirPath = QCoreApplication::applicationDirPath() + QDir::separator() + "random";
    QString encPath = encDirPath + QDir::separator() + "enc.txt";



    QDir directory(directoryPath);
    if (!directory.exists()) {
        if (directory.mkpath(".")) {
            qDebug() << "Directory created successfully.";
        } else {
            qWarning() << "Failed to create the directory.";
        }
    } else {
        qDebug() << "Directory already exists.";
    }

    QDir encDirec(encDirPath);
    if (!encDirec.exists()) {
        if (encDirec.mkpath(".")) {
            qDebug() << "enc Directory created successfully.";
            // Create the encryption file and write the encryption key
        } else {
            qWarning() << "enc Failed to create the directory.";
        }
    } else {
        qDebug() << "enc Directory already exists.";
    }

    createEncryptionFile(encPath, 256);

    QFile databaseFile(databasePath);
    if (!databaseFile.exists()) {
        if (databaseFile.open(QIODevice::ReadWrite)) {
            qDebug() << "Database file created successfully.";
            databaseFile.close();
        } else {
            qWarning() << "Failed to create the database file.";
        }
    } else {
        qDebug() << "Database file already exists.";
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databasePath);

    if (!db.open()) {
        qWarning() << "Failed to open database:" << db.lastError().text();
    } else {
        qDebug() << "Database opened successfully.";
    }

    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS credentials (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT NOT NULL, site TEXT NOT NULL, password TEXT NOT NULL);");

    if (query.exec()) {
        qDebug() << "Table credentials created successfully.";
    } else {
        qWarning() << "Failed to create table credentials" << ":" << query.lastError().text();
    }

    db.close();
}



newpass::~newpass()
{
    delete ui;
}


void newpass::on_generatePW_clicked()
{
    QString password;
    const char alphanum[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char special[] = "!@#$%^&*";

    int string_length = sizeof(alphanum) - 1;

    // generate a random position to insert the special character
    int specialPosition = QRandomGenerator::global()->bounded(14);

    for (int i = 0; i < 14; ++i) {
        // if the current position is the one designated for the special character, append it
        if (i == specialPosition) {
            char randomSpecialChar = special[QRandomGenerator::global()->bounded(strlen(special))];
            password.append(randomSpecialChar);
        } else {
            // appending a random alphanumeric character
            char randomChar = alphanum[QRandomGenerator::global()->bounded(string_length)];
            password.append(randomChar);
        }
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
        strengthCheck(userName, pass, site);
    }
    else{
        QMessageBox::information(this,  "Error", "Password do not match!");
    }



}

void  newpass::strengthCheck( QString username,QString password, QString site)
{
    QString path_to_database = QCoreApplication::applicationDirPath() + QDir::separator() + "database" + QDir::separator() + "database.db";
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path_to_database);

    if(db.open()) {
        qDebug() << "Database connected.";
    }
    else {
        qDebug() << "Database not connected.";
        qDebug() << "Error: " << db.lastError();
    }

    int l_case=0, u_case=0, digit=0, special=0, pass = 0;

    int l=password.length(),i;

    for (i = 0; i < l; i++) {
        QChar currentChar = password[i];

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
    {
       pass = 2;
       ui->strengthDisplay->setText("<font color='blue'>Moderate strength; consider a strong one!</font>");

    }
       else
    {
        ui->strengthDisplay->setText("<font color='red'>Weak password; Please use a strong password!</font>");
    }

    if(!db.open()) {
        qDebug() << "Failed to open the database.";
        return;
    }


    if (pass == 1)
    {
        QByteArray encryptionKey;
        QString encFilePath = QCoreApplication::applicationDirPath() + QDir::separator() + QDir::separator() + "random" + QDir::separator() + "enc.txt";

        QFile encFile(encFilePath);

        // Print the absolute path of the current working directory
        qDebug() << "Current working directory:" << QDir::currentPath();

        if (encFile.exists() && encFile.open(QIODevice::ReadOnly)) {
            encryptionKey = encFile.readAll();
            encFile.close();
            qDebug() << "Encryption key read successfully:" << encryptionKey.toHex();
        } else {
            // Handle the case where the encryption key is not available
            qWarning() << "Failed to read the encryption key from" << encFilePath << ". Error:" << encFile.errorString();
            return;
        }

        // Encrypt the password before storing it in the database
        QByteArray encryptedPassword = encryptPassword(password.toUtf8(), encryptionKey);

        QSqlQuery query;

        query.prepare("INSERT INTO credentials (username, password, site) VALUES (?, ?, ?)");

        query.addBindValue(username);
        query.addBindValue(encryptedPassword);
        query.addBindValue(site);

        qDebug() << "Site:" << site;
        qDebug() << "Username:" << username;
        qDebug() << "Encrypted Password:" << encryptedPassword;

        if (query.exec())
        {
            // Password saved successfully
            QMessageBox::information(this, "Password Saved!", "Password Saved Successfully!");
            this->hide();
            Kavach *kavachWindow = new Kavach(this);  // Assuming the 'this' pointer is a valid parent
            kavachWindow->show();
        }
        else
        {
            qDebug() << "Query failed:" << query.lastError().text();
        }
    }

    else if (pass == 2)
    {
         QString text = QInputDialog::getText(this, "Warning", "The password entered in Moderate in Strength, do you want to continue or use a different one? "
                                                              "\nEnter Y to change password and N to proceed!");
        if (text == "Y")
         {
            qDebug() << "Change password!";
         }
        else{
            qDebug() << "Saving password";
         }
}
}

void newpass::on_savepw_2_clicked()
{
this->hide();
Kavach *kavachWindow = new Kavach(this);  // Assuming the 'this' pointer is a valid parent
kavachWindow->show();

}


void newpass::on_cancel_clicked()
{
ui->username->clear();
ui->pass->clear();
ui->passConf->clear();
ui->site->clear();
}

