#include "listener.h"

namespace QSX {

Listener::Listener(QObject *parent) : QObject(parent) {

}

Listener::~Listener() {
  stop();
}

bool Listener::start(Configuration &config) {
  bool ret;
  m_tcpReply = new QSX::TcpRelay(config, this);
  ret = m_tcpReply->listen();
  if (!ret) {
    // show error message
    m_tcpReply->close();
    return false;
  }
  QObject::connect(m_tcpReply, &TcpRelay::accept, this, &Listener::onAccepted);

  m_udpReply = new QUdpSocket(this);
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
    m_tcpReply->deleteLater();
    m_tcpReply = nullptr;
  }
  if (m_udpReply) {
    m_udpReply->close();
    m_udpReply->deleteLater();
    m_udpReply = nullptr;
  }
}

void Listener::onAccepted(TcpHandler *handler) {
  emit accept(handler);
}

}
