#include "listener.h"

namespace QSS {

Listener::Listener() {

}

Listener::~Listener() {
  m_tcpReply.reset();
  m_udpReply.reset();
}

bool Listener::start(QSS::Configuration &config) {
  bool ret;
  auto localAddress = config.getLocalAddress();
  auto localPort = config.getLocalPort();

  Q_ASSERT(localPort != 0);

  m_tcpReply = std::make_unique<QTcpServer>();
  ret = m_tcpReply->listen(localAddress, localPort);
  if (!ret) {
    // show error message
    m_tcpReply->close();
    return false;
  }

  m_udpReply = std::make_unique<QUdpSocket>();
  ret = m_udpReply->bind(localAddress, localPort);
  if (!ret) {
    // show error message
    m_udpReply->close();
    return false;
  }

  return true;
}

void Listener::stop() {
  if (m_tcpReply) {
    m_tcpReply->close();
  }
  if (m_udpReply) {
    m_udpReply->close();
  }
}

}
