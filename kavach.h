#ifndef KAVACH_H
#define KAVACH_H

#include <QMainWindow>
#include "newpass.h"
#include"savedpw.h"
#include"mainwindow.h"




QT_BEGIN_NAMESPACE
namespace Ui { class Kavach; }
QT_END_NAMESPACE

class Kavach : public QMainWindow
{
    Q_OBJECT

public:
    Kavach(QWidget *parent = nullptr);
    ~Kavach();

private slots:
    void on_newPw_clicked();

    void on_oldPw_clicked();

private:
    Ui::Kavach *ui;

    newpass *newPass;

    savedpw *savedPw;

    MainWindow *mainwindow;
};
#endif // KAVACH_H
