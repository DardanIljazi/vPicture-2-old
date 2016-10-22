#include "../../network.h"

void Network::connectUser(QString mail, QString password){
    QVariantMap arguments;
    arguments["email"] = mail;

    if(!isPasswordCrypted(password))
        arguments["password"] = QString(secure->secure(password.toLatin1())); // The password is going to be secured with hash like said in secure.h (Secure Class)

    else // If the password is soon secured when received to Network::connectUser, we use it as it.
        arguments["password"] = password;

    localUrl->url("connect", arguments);

    get(localUrl->getUrl());

    // We set the users settings in Settings Class
    localSettings->addValue("user/email", mail);
    localSettings->addValue("user/p", arguments["password"]);
}

bool Network::isPasswordCrypted(QString password){
    // If the argument: QString password contains only a-f or A-F characters and its size is 40 (SHA1), that means it is a crypted password
    if(password.contains(QRegExp("^[a-fA-F0-9]{40}$")))
        return true;

    return false;
}
