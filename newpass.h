#ifndef NEWPASS_H
#define NEWPASS_H

#include <QDialog>
#include <QtSql>
#include<QtCore>
#include<QFileInfo>
#include<QDebug>




using namespace std;
namespace Ui {
class newpass;
}

class newpass : public QDialog
{
    Q_OBJECT

public:
    explicit newpass(QWidget *parent = nullptr);
    ~newpass();
    void mongoConnect();


private slots:

    void  strengthCheck(QString str);


    void on_generatePW_clicked();


    void on_savepw_clicked();




    void on_savepw_2_clicked();

private:
    Ui::newpass *ui;

    QSqlDatabase mydb;
};

#endif // NEWPASS_H
