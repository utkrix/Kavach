#include <QtGlobal>
#include "ui_savedpw.h"
#include "savedpw.h"
#include <QFile>
#include <QSqlQuery>
#include <QByteArray>
#include <QIODevice>
#include <QDebug>
#include <QCryptographicHash>
#include <QSqlError>
#include <QDir>
#include <QTableWidgetItem>
#include <QApplication>


savedpw::savedpw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::savedpw)
{
    ui->setupUi(this);
    // Directory paths
    QString directoryPath = QCoreApplication::applicationDirPath() + QDir::separator() + "database";
    QString databasePath = directoryPath + QDir::separator() + "database.db";
    QString encDirPath = QCoreApplication::applicationDirPath() + QDir::separator() + "random";

    // Full paths
    QString encPath = encDirPath + QDir::separator() + "enc.txt";

    ui->setupUi(this);
    qDebug() << "SAVED PW";

    // Open the database
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databasePath);  // Set the database name
    if (!db.open()) {
        qDebug() << "Failed to open the database:" << db.lastError().text();
    } else {
        qDebug() << "Database opened successfully.";
        retrieveAndDisplayPasswords();
    }
}

savedpw::~savedpw()
{
    delete ui;
}

QByteArray savedpw::decryptPassword(const QByteArray &encryptedPassword, const QByteArray &encryptionKey) {
    // Use cryptographic hash to derive a key from the encryption key
    QByteArray derivedKey = QCryptographicHash::hash(encryptionKey, QCryptographicHash::Sha256);

    // Convert the hex-encoded encrypted password back to QByteArray
    QByteArray encryptedData = QByteArray::fromHex(encryptedPassword);

    // Use XOR operation to decrypt the password
    QByteArray decryptedPassword;
    for (int i = 0; i < encryptedData.size(); ++i) {
        decryptedPassword.append(encryptedData.at(i) ^ derivedKey.at(i % derivedKey.size()));
    }

    return decryptedPassword;
}

void savedpw::retrieveAndDisplayPasswords() {
    // Retrieve passwords from the database
    QSqlQuery query("SELECT username, site, password FROM credentials");
    while (query.next()) {
        QString username = query.value(0).toString();
        QString site = query.value(1).toString();
        QString encryptedPassword = query.value(2).toString();

        // Retrieve the encryption key from the enc.txt file
        QString encPath = QCoreApplication::applicationDirPath() + QDir::separator() + "random" + QDir::separator() + "enc.txt";
        QFile encFile(encPath);
        QByteArray encryptionKey;

        if (encFile.exists() && encFile.open(QIODevice::ReadOnly)) {
            encryptionKey = encFile.readAll();
            encFile.close();
        } else {
            qWarning() << "Failed to read the encryption key from enc.txt. Error:" << encFile.errorString();
            return;
        }

        qDebug() << "Username:" << username;
        qDebug() << "Site:" << site;
        qDebug() << "Encrypted Password:" << encryptedPassword;
        qDebug() << "Encryption Key:" << encryptionKey;

        // Decrypt the password
        QByteArray encryptedPasswordBytes = encryptedPassword.toUtf8();
        QByteArray decryptedPassword = decryptPassword(encryptedPasswordBytes, encryptionKey);
        qDebug() << "Decrypted Password:" << decryptedPassword;

        // Display the decrypted password in your UI (modify as needed)
        QTableWidgetItem *itemSite = new QTableWidgetItem(site);
        QTableWidgetItem *itemUsername = new QTableWidgetItem(username);
        QTableWidgetItem *itemPassword = new QTableWidgetItem(decryptedPassword);

        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, itemSite);
        ui->tableWidget->setItem(row, 1, itemUsername);
        ui->tableWidget->setItem(row, 2, itemPassword);
    }
}
