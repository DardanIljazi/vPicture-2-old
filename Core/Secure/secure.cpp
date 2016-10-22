#include "secure.h"

Secure::Secure(QObject *parent) :
    QObject(parent)
{
    hash = new QCryptographicHash(QCryptographicHash::Sha1);
}

QByteArray Secure::secure(QByteArray data){
    dataToHash.clear();
    hash->reset();

    dataToHash = data;

    // Insert in the begining of data BEGIN_CRYPT_HASH
    data.insert(0, BEGIN_CRYPT_HASH);
    // Insert in the end of data END_CRYPT_HASH
    data.insert(data.size(), END_CRYPT_HASH);

    // Add the content to hash into hash
    hash->addData(data);

    // Return the data hashed
    return hash->result().toHex();
}
