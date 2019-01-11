#pragma once

#include <memory>
#include "net/tcprelay.h"
#include <QtNetwork/QUdpSocket>
#include "types/configuration.h"

namespace QSX {

class Listener : public QObject {

Q_OBJECT

private:
  // tcp_reply
  TcpRelay *m_tcpReply = nullptr;
  // udp_reply
  QUdpSocket *m_udpReply = nullptr;

  Configuration config;

public:
  explicit Listener(QObject *parent);
  ~Listener() override;

  bool start(Configuration &config);
  void stop();

protected slots:
  void onAccepted(TcpHandler *handler);

signals:
  void accept(TcpHandler *handler);
};

}
