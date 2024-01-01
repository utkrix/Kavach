#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>  // Include the necessary header
#include<QMainWindow>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>

namespace Ui {
class Signup;
}

class Signup : public QMainWindow  // Inherit from QDialog
{
    Q_OBJECT

public:
    explicit Signup(QWidget *parent = nullptr);
    ~Signup();



private slots:
    void on_pushButton_create_clicked();
    void on_pushButton_cancel_clicked();


private:
    Ui::Signup *ui;

     QSqlDatabase DB;
};

#endif // SIGNUP_H
