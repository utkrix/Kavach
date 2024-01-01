#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>  // Include the necessary header
#include<QMainWindow>
namespace Ui {
class Signup;
}

class Signup : public QMainWindow  // Inherit from QDialog
{
    Q_OBJECT

public:
    explicit Signup(QWidget *parent = nullptr);
    ~Signup();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_pushButton_create_clicked();
    void on_pushButton_cancel_clicked();
    void on_show_password_clicked();

private:
    Ui::Signup *ui;
};

#endif // SIGNUP_H
