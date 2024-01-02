#include "savedpw.h"
#include "qboxlayout.h"
#include "ui_savedpw.h"
#include <QFile>
#include <QSqlQuery>
#include <QByteArray>
#include <QIODevice>
#include <QDebug>
#include <QCryptographicHash>
#include <QSqlError>
#include <QDir>
#include <QCoreApplication>
#include <QClipboard>
#include <QPushButton>
#include "kavach.h"
#include<QHBoxLayout>

#include<QVBoxLayout>


void savedpw::on_backButton_clicked()
{
    // Hide the current window (savedpw) and show the Kavach window
    this->hide();
    Kavach *kavachWindow = new Kavach(this);  // Assuming the 'this' pointer is a valid parent
    kavachWindow->show();
}


savedpw::savedpw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::savedpw)
{

    // Directory paths
    QString directoryPath = QCoreApplication::applicationDirPath() + QDir::separator() + "database";
    QString databasePath = directoryPath + QDir::separator() + "database.db";
    QString encDirPath = QCoreApplication::applicationDirPath() + QDir::separator() + "random";

    // Full paths
    QString encPath = encDirPath + QDir::separator() + "enc.txt";

    ui->setupUi(this);

    qDebug() << "SAVED PW";

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Labels layout
    QHBoxLayout *labelsLayout = new QHBoxLayout();

    SiteLabel = new QLabel(this);
    SiteLabel->setText("Site Name:");
    SiteLabel->setFont(QFont("Times New Roman", 18));
    SiteLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    UsernameLabel = new QLabel(this);
    UsernameLabel->setText("Username:");
    UsernameLabel->setFont(QFont("Times New Roman", 18));
    UsernameLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    PasswordLabel = new QLabel(this);
    PasswordLabel->setText("Password:");
    PasswordLabel->setFont(QFont("Times New Roman", 18));
    PasswordLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    labelsLayout->addWidget(SiteLabel);
    labelsLayout->addWidget(UsernameLabel);
    labelsLayout->addWidget(PasswordLabel);

    mainLayout->addLayout(labelsLayout);

    // Passwords layout
    QVBoxLayout *passwordsLayout = new QVBoxLayout();

    // Assuming you have a QStringList with passwords
    QStringList passwords; // Replace this with your actual data

    for (const QString &password : passwords) {
        QLabel *passwordLabel = new QLabel(password, this);
        passwordLabel->setFont(QFont("Times New Roman", 18));
        passwordLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        passwordsLayout->addWidget(passwordLabel);
    }

    mainLayout->addLayout(passwordsLayout);

    // Back button
    QPushButton* backButton = new QPushButton("Back", this);
    backButton->setFont(QFont("Times New Roman", 18));
    connect(backButton, &QPushButton::clicked, this, &savedpw::on_backButton_clicked);

    mainLayout->addWidget(backButton, 0, Qt::AlignBottom);


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

    qDebug() << "Encrypted Password Length:" << encryptedPassword.length();
    qDebug() << "Derived Key Length:" << derivedKey.length();

    // Convert the hex-encoded encrypted password back to QByteArray
    QByteArray encryptedData = QByteArray::fromHex(encryptedPassword);

    if (derivedKey.size() != 32) {
        qWarning() << "Derived key has unexpected size:" << derivedKey.size();
        return QByteArray();  // Return an empty QByteArray to indicate an error
    }

    // Use XOR operation to decrypt the password
    QByteArray decryptedPassword;
    for (int i = 0; i < encryptedData.size(); ++i) {
        decryptedPassword.append(encryptedData.at(i) ^ derivedKey.at(i % derivedKey.size()));
    }

    QString decryptedString = QString::fromUtf8(decryptedPassword);

    qDebug() << "Decrypted Password Length:" << decryptedPassword.length();
    qDebug() << "Decrypted Password:" << decryptedString;

    return decryptedString.toUtf8();;
}

void savedpw::retrieveAndDisplayPasswords() {

    // Retrieve passwords from the database


    QString siteNames, userNames, dePassword;
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

        siteNames += site + "\n";
        userNames += username + "\n";



        // Decrypt the password
        QByteArray encryptedPasswordBytes = encryptedPassword.toUtf8();
        QByteArray decryptedPassword = decryptPassword(encryptedPasswordBytes, encryptionKey);
        qDebug() << "Decrypted Password:" << decryptedPassword;

        dePassword += decryptedPassword + "\n";


    }

    SiteLabel->setText("Site:\n \n" + siteNames.trimmed());
    UsernameLabel->setText("Username:\n \n" + userNames.trimmed());
    PasswordLabel->setText("Password:\n \n" + dePassword.trimmed());
}
