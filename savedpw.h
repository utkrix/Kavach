#ifndef SAVEDPW_H
#define SAVEDPW_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class savedpw;
}

class savedpw : public QDialog
{
    Q_OBJECT

public:
    explicit savedpw(QWidget *parent = nullptr);
    ~savedpw();

    QByteArray decryptPassword(const QByteArray &encryptedPassword, const QByteArray &encryptionKey);

private:
    Ui::savedpw *ui;
    QSqlDatabase db;

public slots:
    void retrieveAndDisplayPasswords();
};

#endif // SAVEDPW_H
