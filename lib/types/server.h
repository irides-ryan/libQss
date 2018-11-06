#pragma once

#include <QtCore/QString>

namespace QSS {

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

  static Server getDefault() {
    return Server();
  }
};

}
