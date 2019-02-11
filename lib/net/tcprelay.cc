#include "tcprelay.h"

namespace QSX {

TcpRelay::TcpRelay(Configuration &configuration, QObject *parent) : QObject(parent) {
  m_local = new QTcpServer(this);
  m_config = configuration;
}

TcpRelay::~TcpRelay() {
  close();
  if (m_local) {
    m_local->close();
    m_local->deleteLater();
    m_local = nullptr;
  }
}

bool TcpRelay::listen() {
  QObject::connect(m_local, &QTcpServer::newConnection, this, &TcpRelay::onAccepted);

  auto localAddress = m_config.getLocalAddress();
  auto localPort = m_config.getLocalPort();

  Q_ASSERT(localPort != 0);

  qDebug() << "TcpReply: listen" << localAddress << ":" << localPort;
  bool r = m_local->listen(localAddress, localPort);
  if (!r) {
    qWarning() << "listen error!";
    close();
    return false;
  }

  return true;
}

void TcpRelay::close() {
  for (auto i = m_cache.begin(); i != m_cache.end();) {
    auto h = i++;
    (*h)->close();
  }
  if (m_local) {
    m_local->close();
  }
}

void TcpRelay::onAccepted() {
  auto client = m_local->nextPendingConnection();

  auto handler = new TcpHandler(client, m_config, this);
  QObject::connect(handler, &TcpHandler::finished, [=](int r) {
    m_cache.remove(handler);
    handler->deleteLater();
  });

  m_cache.emplace_back(handler);
  emit accept(handler);

  // close the timed out handlers
  for (auto i = m_cache.begin(); i != m_cache.end();) {
    auto h = i++;
    if ((*h)->isTimeout()) {
      (*h)->close();
    }
  }
}

}
