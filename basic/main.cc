#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <signal.h>
#include "../lib/util/listener.h"

using namespace QSX;

Listener *listener = nullptr;

void signal_handler(int signal) {
  if (signal == SIGINT || signal == SIGTERM || signal == SIGABRT) {
    qDebug() << "\nSIGNAL: " << signal;
    listener->stop();
    listener->deleteLater();
    qApp->quit();
  }
}

int main(int argc, char **argv) {

  signal(SIGINT, signal_handler);

  QCoreApplication a(argc, argv);

  Configuration configuration;
  Server server;
  server.server = "107.182.191.51";
  server.server_port = 26316;
//  server.server = "127.0.0.1";
//  server.server_port = 8388;
  server.method = "aes-256-cfb";
  server.passwd = "rsy0715Z-";
  QList<Server> servers;
  servers.append(server);
  configuration.setServers(servers);

  Proxy proxy;
  proxy.use = true;
  proxy.server = "10.167.196.133";
  proxy.port = 8080;
  proxy.type = Proxy::HTTP;
  configuration.setProxy(proxy);

  configuration.setLocalPort(1080);

  listener = new Listener();
  listener->start(configuration);

  QCoreApplication::exec();

  return 0;
}
