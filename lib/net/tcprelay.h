#pragma once

#include <memory>
#include <QtNetwork/QTcpServer>
#include "types/configuration.h"
#include "tcphandler.h"

namespace QSX {

class TcpRelay : public QObject {

Q_OBJECT

  Configuration m_config;
  std::unique_ptr<QTcpServer> m_local;
  std::list<TcpHandler *> m_cache;

public:
  explicit TcpRelay(Configuration &configuration);
  ~TcpRelay() override;

  bool listen();
  void close();

protected:

protected slots:
  void onAccepted();

signals:
  void accept(TcpHandler *handler);

};

}
