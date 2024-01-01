// signup.cpp

#include "signup.h"
#include "ui_signup.h"
#include "mainwindow.h"
#include <QMessageBox>

Signup::Signup(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Signup)
{
    ui->setupUi(this);
    QPixmap originalPixmap("/login-bg.png"); // Replace with your image path

    // Set the original pixmap to the QLabel
    ui->imageLabel->setPixmap(originalPixmap);

    QString path_to_database = QCoreApplication::applicationDirPath() + QDir::separator() + "database" + QDir::separator() + "database.db";
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


