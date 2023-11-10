#ifndef SAVEDPW_H
#define SAVEDPW_H

#include <QDialog>

namespace Ui {
class savedpw;
}

class savedpw : public QDialog
{
    Q_OBJECT

public:
    explicit savedpw(QWidget *parent = nullptr);
    ~savedpw();

private:
    Ui::savedpw *ui;
};

#endif // SAVEDPW_H
