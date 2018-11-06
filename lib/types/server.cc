#include "server.h"

namespace QSS {

Server::Server()
        : server(""),
          local("localhost"),
          passwd(""),
          method(""),
          remarks(""),
          server_port(8388),
          local_port(1080),
          timeout(SERVER_TIMEOUT_DEFAULT) {}

}
