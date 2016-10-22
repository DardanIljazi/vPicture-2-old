#ifndef SECURE_H
#define SECURE_H

#include <QObject>
#include <QCryptographicHash>

/*
 * Those 2 keys are used to secure some information like the passsword or use it to genere the token with the url data
 * The password is hashed like this: SHA1(BEGIN_CRYPT_HASH + %{password} + END_CRYPT_HASH)
 */
#define BEGIN_CRYPT_HASH "W8Nz768hLLB44Bx"
#define END_CRYPT_HASH   "CQpwF5t59rYFPFtunDkW8pD"

class Secure : public QObject
{
    Q_OBJECT
public:
    explicit Secure(QObject *parent = 0);

signals:

public slots:
    /*
     * This function return a sha1 hash for QByteArtay data with the hash: SHA1(BEGIN_CRYPT_HASH + data + END_CRYPT_HASH)
     */
    QByteArray secure(QByteArray data);

private:
    // hash contain the hash result for dataToHash
    QCryptographicHash *hash;
    // data not hashed received by QByteArray data argument in Secure::secure(QByteArray data) function
    QString dataToHash;
};

#endif // SECURE_H
