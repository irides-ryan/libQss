#include "configuration.h"

namespace QSX {

Configuration::Configuration()
  : m_shareOverLan(false), m_localPort(1080), m_index(0) {}

Configuration::~Configuration() {
  unregisterChooser();
}

Server Configuration::getServer(Address &destination) {
  if (m_chooser) {
    return m_chooser->getServer(destination, m_index);
  }

  if (m_servers.empty()) {
    return Server();
  }

  int index = m_index;
  if (!(0 <= index && index < m_servers.size())) {
    index = 0;
  }
  return m_servers[index];
}

}
