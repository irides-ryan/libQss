#include "proxy.h"

namespace QSX {

Proxy::Proxy()
        : use(false),
          type(HTTP),
          server("192.168.1.1"),
          port(8080),
          timeout(PROXY_TIMEOUT_DEFAULT) {}

}
