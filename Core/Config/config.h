#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QStringList>
#include <QAction>
#include <QDebug>

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = 0);

signals:

public slots:
    QStringList         getListMimeDataAllowed();
    QString             mimeDataAllowedScheme();
    QList<QAction *>    getLanguagesList();
private:
    // m_listMimeData contains the list of allowed extensions
    QStringList m_listMimeData;

};

#endif // CONFIG_H
