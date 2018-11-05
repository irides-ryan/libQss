#pragma once

#include <QtCore/QString>

namespace QSS {

class Proxy {

  const uint16_t PROXY_TIMEOUT_DEFAULT = 3;

public:
  const uint16_t PROXY_TIMEOUT_MAX = 10;
  enum {
    HTTP, SOCKS5
  };

  bool use;
  int type;
  QString server;
  uint16_t port;
  uint16_t timeout;

  Proxy();

  Proxy &operator=(Proxy const &proxy) {
    use = proxy.use;
    type = proxy.type;
    server = proxy.server;
    port = proxy.port;
    timeout = proxy.timeout;
    return *this;
  }
};

}
