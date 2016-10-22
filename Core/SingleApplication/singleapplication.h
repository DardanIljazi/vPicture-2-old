#ifndef APPLICATION_H
#define APPLICATION_H

#include "localserver.h"

#include <QApplication>
#include <QGuiApplication>
#include <QLocalSocket>

/**
 * @brief The Application class handles trivial application initialization procedures
 */
class SingleApplication : public QObject
{
  Q_OBJECT
public:
  explicit SingleApplication();
  ~SingleApplication();
  bool shouldContinue();

signals:
  void showUp();

private slots:
  void slotShowUp();
  
private:
  QLocalSocket* socket;
  LocalServer* server;
  bool _shouldContinue;

};

#endif // APPLICATION_H
