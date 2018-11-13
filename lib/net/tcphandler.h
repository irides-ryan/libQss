#pragma once

#include <memory>
#include <QtNetwork/QTcpSocket>
#include <crypto/encryptor.h>
#include <types/configuration.h>

namespace QSX {

class TcpHandler : public QObject {

  Q_OBJECT

  typedef std::function<void()> Finisher;

  enum STATE { INIT, ADDR, UDP_ASSOC, DNS, CONNECTING, STREAM, DESTROYED };

  static const char HANDLE_ACCEPT[], HANDLE_REJECT[], HANDLE_RESPONSE[];

  std::unique_ptr<QTcpSocket> m_local, m_remote;
  QSS::Configuration* m_config;
  QSS::Encryptor* m_encryptor;

public:
  TcpHandler(QTcpSocket *socket, QSS::Configuration &configuration);
  ~TcpHandler() override;

  void close();

  uint64_t getCountRead() {
    return m_countRead;
  }

  uint64_t getCountWrite() {
    return m_countWrite;
  }

private:
  STATE m_state = INIT;
  uint64_t m_countRead = 0, m_countWrite = 0;

  void handle(QByteArray &data);
  void createRemote(QSS::Address &destination);
  void loadProxyRemote(QSS::Proxy &proxy);
  void sendToRemote(QByteArray &data);

protected slots:
  void onRecvLocal();
  void onRecvRemote();
  void onConnectedRemote();
  void onErrorRemote();

signals:
  void bytesRead(uint64_t s);
  void bytesWrite(uint64_t s);
};

}
