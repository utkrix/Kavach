// mainwindow.h

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QRegularExpression>
#include "signup.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Login_clicked();
    void on_pushButton_Cancel_clicked();
    void on_pushButton_account_clicked();
    void on_show_password_linkActivated(const QString &link);
    void on_showpassword_clicked();  // Add this line

private:
    Ui::MainWindow *ui;
    void centerUI();
    Signup *signup;
};

#endif // MAINWINDOW_H
