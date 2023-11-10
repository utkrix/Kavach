#ifndef NEWPASS_H
#define NEWPASS_H

#include <QDialog>

namespace Ui {
class newpass;
}

class newpass : public QDialog
{
    Q_OBJECT

public:
    explicit newpass(QWidget *parent = nullptr);
    ~newpass();

private slots:

    void on_generatePW_clicked();


private:
    Ui::newpass *ui;
};

#endif // NEWPASS_H
