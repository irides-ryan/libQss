#include "tcprelay.h"

namespace QSX {

TcpRelay::TcpRelay(Configuration &configuration) {
  m_local = std::make_unique<QTcpServer>();
  m_config = configuration;
}

TcpRelay::~TcpRelay() {
  close();
}

bool TcpRelay::listen() {

  QObject::connect(m_local.get(), &QTcpServer::newConnection, this, &TcpRelay::onAccepted);

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
}

void TcpRelay::onAccepted() {
  auto client = m_local->nextPendingConnection();

  auto handler = new TcpHandler(client, m_config);
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
