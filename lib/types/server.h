#pragma once

#include <QtCore/QString>

namespace QSX {

class Server {

  const static uint16_t SERVER_TIMEOUT_DEFAULT = 5;

public:
  const static uint16_t SERVER_TIMEOUT_MAX = 20;

  QString server;
  QString local;
  QString passwd;
  QString method;
  QString remarks;
  uint16_t server_port;
  uint16_t local_port;
  uint16_t timeout;

  Server();

  Server &operator=(Server const &s) = default;

  bool operator==(Server const &r) {
    return server == r.server
           && local == r.local
           && passwd == r.passwd
           && method == r.method
           && remarks == r.remarks
           && server_port == r.server_port
           && local_port == r.local_port
           && timeout == r.timeout;
  }

  static Server getDefault() {
    return Server();
  }
};

}
