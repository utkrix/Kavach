// signup.cpp

#include "signup.h"
#include "qevent.h"
#include "ui_signup.h"
#include <QMessageBox>

Signup::Signup(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Signup)
{
    ui->setupUi(this);
    QPixmap originalPixmap("/home/nishant/Downloads/Password_Manager/login-bg.png"); // Replace with your image path

    // Set the original pixmap to the QLabel
    ui->imageLabel->setPixmap(originalPixmap);
}

Signup::~Signup()
{
    delete ui;
}

void Signup::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event); // Call the base class's resizeEvent

    // Get the new size of the window
    QSize newSize = event->size();

    // Scale the original pixmap to the new size
    QPixmap scaledPixmap = ui->imageLabel->pixmap().scaled(newSize, Qt::KeepAspectRatio);

    // Set the scaled pixmap to the QLabel
    ui->imageLabel->setPixmap(scaledPixmap);
}

void Signup::on_pushButton_cancel_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Password Manager", "Are you sure want to cancel?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QApplication::quit();
    }
}

void Signup::on_pushButton_create_clicked()
{
    QString UserName = ui->lineEdit->text();
    QString Password = ui->lineEdit_2->text();

    if (UserName == "Nishant@2080" && Password == "qt@123")
    {
        QMessageBox::information(this, "Password Manager", "Account Succesfully Created");
    }
    else
    {
        QMessageBox::warning(this, "Qt App Development", "Invalid username or Password");
    }
}

void Signup::on_show_password_clicked()
{
    ui->lineEdit_2->setEchoMode(QLineEdit::Normal);
}
