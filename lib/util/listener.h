#pragma once

#include <memory>
#include <QtNetwork/QUdpSocket>
#include <QtNetwork/QTcpServer>
#include "types/configuration.h"

namespace QSS {

class Listener {
private:
  // tcp_reply
  std::unique_ptr<QTcpServer> m_tcpReply = nullptr;
  // udp_reply
  std::unique_ptr<QUdpSocket> m_udpReply = nullptr;

  Configuration config;

public:
  Listener();
  ~Listener();

  bool start(Configuration &config);

  void stop();

};

}
