#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGuiApplication>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Adjust the main window size based on the screen size
    QScreen *screen = QGuiApplication::primaryScreen();
    QSize screenSize = screen->geometry().size();
    resize(screenSize.width() * 0.8, screenSize.height() * 0.8);

    // Connect signals and slots
    connect(ui->pushButton_Login, SIGNAL(clicked()), this, SLOT(on_pushButton_Login_clicked()));
    connect(ui->pushButton_Cancel, SIGNAL(clicked()), this, SLOT(on_pushButton_Cancel_clicked()));
    connect(ui->pushButton_account, SIGNAL(clicked()), this, SLOT(on_pushButton_account_clicked()));
    connect(ui->showpassword, SIGNAL(clicked()), this, SLOT(on_showpassword_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Login_clicked()
{
    QString UserName = ui->line_UserName->text();
    QString Password = ui->line_Password->text();

    // login logic - check from database
}

void MainWindow::on_pushButton_Cancel_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Qt App Development", "Are You sure want to exit?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
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
    qDebug() << "Button clicked - Opening Signup";

    // Check if the signup window is already open
    if (!signup)
    {
        signup = new Signup(this);  // Pass the correct parent (this) to the constructor
        connect(signup, &Signup::destroyed, [=]() { signup = nullptr; });  // Handle window destruction
        signup->show();
        signup->activateWindow();
    }
    else
    {
        qDebug() << "Signup window is already open.";
    }
}
void MainWindow::on_show_password_linkActivated(const QString &link)
{
    ui->line_Password->setEchoMode(QLineEdit::Normal);
}

void MainWindow::on_showpassword_clicked()
{
    ui->line_Password->setEchoMode(QLineEdit::Normal);
}
