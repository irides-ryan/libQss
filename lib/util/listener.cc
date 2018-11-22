#include "listener.h"

namespace QSX {

Listener::Listener() {

}

Listener::~Listener() {
  m_tcpReply.reset();
  m_udpReply.reset();
}

bool Listener::start(Configuration &config) {
  bool ret;
  m_tcpReply = std::make_unique<QSX::TcpRelay>(config);
  ret = m_tcpReply->listen();
  if (!ret) {
    // show error message
    m_tcpReply->close();
    return false;
  }
  QObject::connect(m_tcpReply.get(), &TcpRelay::accept, this, &Listener::onAccepted);

  m_udpReply = std::make_unique<QUdpSocket>();
  ret = m_udpReply->bind();
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

void Listener::onAccepted(TcpHandler *handler) {
  emit accept(handler);
}

}
