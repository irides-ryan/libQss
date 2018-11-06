#include "configuration.h"

namespace QSS {

Configuration::Configuration()
        : m_shareOverLan(false) {}

Configuration::~Configuration() {
  unregisterChooser();
}

Server Configuration::getServer(int index) {
  if (index < 0 && m_chooser) {
    return m_chooser->getServer();
  }
  if (m_servers.empty()) {
    return Server();
  } else {
    if (!(0 < index && index < m_servers.size())) {
      index = 0;
    }
    return m_servers[index];
  }
}

}
