// signup.cpp

#include "signup.h"
#include "ui_signup.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QDateTime>


Signup::Signup(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Signup)
{
    ui->setupUi(this);
    QPixmap originalPixmap("/login-bg.png"); // Replace with your image path

    // Set the original pixmap to the QLabel
    ui->imageLabel->setPixmap(originalPixmap);

    QString path_to_database = QCoreApplication::applicationDirPath() + QDir::separator() + "database";
    QDir databaseDir(path_to_database);
    if (!databaseDir.exists()) {
        qDebug() << "Creating database directory...";
        if (databaseDir.mkpath(path_to_database)) {
            qDebug() << "Database directory created successfully.";
        } else {
            qWarning() << "Failed to create database directory.";
        }
    }

    QString pathToDatabaseFile = path_to_database + QDir::separator() + "database.db";
    if (!QFile::exists(pathToDatabaseFile)) {
        qDebug() << "Creating database file...";
        QFile databaseFile(pathToDatabaseFile);
        if (databaseFile.open(QIODevice::ReadWrite)) {
            qDebug() << "Database file created successfully.";
            databaseFile.close();
        } else {
            qWarning() << "Failed to create database file.";
        }
    }

    // Check if the random directory exists
    QString encDirPath = QCoreApplication::applicationDirPath() + QDir::separator() + "random";
    QDir encDir(encDirPath);
    if (!encDir.exists()) {
        qDebug() << "Creating random directory...";
        if (encDir.mkpath(encDirPath)) {
            qDebug() << "Random directory created successfully.";
        } else {
            qWarning() << "Failed to create random directory.";
        }
    }
    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName(path_to_database);

    if(DB.open()) {
        qDebug() << "Database connected.";
    }
    else {
        qDebug() << "Database not connected.";
        qDebug() << "Error: " << DB.lastError();
    }
}

Signup::~Signup()
{
    delete ui;
}



void Signup::on_pushButton_cancel_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Kavach", "Are you sure want to cancel?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QApplication::quit();
    }
}




void Signup::on_pushButton_create_clicked()
{
    QString username = ui->lineEdit_username_2->text();
    QString password = ui->lineEdit_password_2->text();
    QString cpassword = ui->lineEdit_confirm_password->text();

    QByteArray hashedUsername = QCryptographicHash::hash(username.toUtf8(), QCryptographicHash::Sha256).toHex();
    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

    QString encDirPath = QCoreApplication::applicationDirPath() + QDir::separator() + "random";
    QString hashedPasswordPath = encDirPath + QDir::separator() + "hsdp.txt";

    QFile hashedPasswordFile(hashedPasswordPath);

    if (!hashedPasswordFile.exists()) {
        if (hashedPasswordFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&hashedPasswordFile);
            stream << hashedUsername << "\n" << hashedPassword;
            hashedPasswordFile.close();

            qDebug() << "Hashed username and password saved to:" << hashedPasswordPath;
            QMessageBox::information(this, "Account Created", "Account Created Successfully! ");
            MainWindow *mainWindow = new MainWindow();  // Create an instance of the login page
            mainWindow->show();
            this->close();
        } else {
            qDebug() << "Failed to open hashed password file for writing.";
        }
    } else {
        qDebug() << "Hashed password file already exists. Please proceed with login.";
        QMessageBox::warning(this, "Error", "Account already exists!");
        MainWindow *mainWindow = new MainWindow();  // Create an instance of the login page
        mainWindow->show();
        this->close();
    }
}


