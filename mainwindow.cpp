#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGuiApplication>
#include <QScreen>
#include<QRegularExpression>
#include "signup.h"
#include "kavach.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Adjust the main window size based on the screen size
    QScreen *screen = QGuiApplication::primaryScreen();
    QSize screenSize = screen->geometry().size();
    resize(screenSize.width() * 0.8, screenSize.height() * 0.8);

    // Other customizations based on screen size...

    // Connect signals and slots
    connect(ui->pushButton_Cancel, SIGNAL(clicked()), this, SLOT(on_pushButton_Cancel_clicked()));




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Login_clicked()
{
    QString username = ui->line_UserName->text();
    QString password = ui->line_Password->text();

    QByteArray hashedUsername = QCryptographicHash::hash(username.toUtf8(), QCryptographicHash::Sha256).toHex();
    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();

    QString encDirPath = QCoreApplication::applicationDirPath() + QDir::separator() + "random";
    QString hashedPasswordPath = encDirPath + QDir::separator() + "hsdp.txt";

    QFile hashedPasswordFile(hashedPasswordPath);
    if (hashedPasswordFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&hashedPasswordFile);
        QByteArray storedUsername = stream.readLine().trimmed().toUtf8(); // Convert QString to QByteArray
        QByteArray storedHashedPassword = stream.readLine().trimmed().toUtf8(); // Convert QString to QByteArray
        hashedPasswordFile.close();

        qDebug() << "Input Hashed Username: " << hashedUsername;
        qDebug() << "Input Hashed Password: " << hashedPassword;
        qDebug() << "Stored Hashed Username: " << storedUsername;
        qDebug() << "Stored Hashed Password: " << storedHashedPassword;

        if (storedUsername == hashedUsername && storedHashedPassword == hashedPassword) {
            qDebug() << "Login successful!";
            this->close();
            Kavach *kavach = new Kavach();
            kavach->show();

        } else {
            qDebug() << "Login failed. Incorrect username or password.";
            // Handle incorrect login
        }
    } else {
        qDebug() << "Failed to open hashed password file for reading.";
        // Handle file read error
    }
}





void MainWindow::on_pushButton_Cancel_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Qt App Development","Are You sure want to exit?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes)
    {
        QApplication::quit();
    }
}



void MainWindow::centerUI()
{
    // Center the main window on the primary screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    move(screenGeometry.center() - rect().center());
}



void MainWindow::on_pushButton_account_clicked()
{
    this->close();

    signup =new Signup();
    signup->show();

}


