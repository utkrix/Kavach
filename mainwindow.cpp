#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGuiApplication>
#include <QScreen>
#include<QRegularExpression>
#include "signup.h"


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
    connect(ui->pushButton_Login, SIGNAL(clicked()), this, SLOT(on_pushButton_Login_clicked()));
    connect(ui->pushButton_Cancel, SIGNAL(clicked()), this, SLOT(on_pushButton_Cancel_clicked()));




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Login_clicked()
{
    QString UserName = ui->line_UserName->text();
    QString Password = ui->line_Password->text();

    if(UserName=="Nishant@2080" && Password== "qt@123")
    {

        QMessageBox::information(this,"Qt App Development","Login Sucess");

    }
    else {
        QMessageBox::warning(this,"Qt App Development","Invalid username or Password");

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

 /*void MainWindow::on_pushButton_FullScreen_clicked()
{
    // Toggle between full-screen mode and normal mode
    if (isFullScreen())
    {
        showNormal();
    }
    else
    {
        showFullScreen();

        // Center the UI after going to full-screen mode
        centerUI();
    }
} */

void MainWindow::centerUI()
{
    // Center the main window on the primary screen
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    move(screenGeometry.center() - rect().center());
}



void MainWindow::on_pushButton_account_clicked()
{
    signup =new Signup( this);
    signup->show();
}


void MainWindow::on_showpassword_clicked()
{
    ui->line_Password->setEchoMode(QLineEdit::Normal);
}

