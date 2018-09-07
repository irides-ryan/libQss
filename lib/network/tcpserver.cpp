#include <utility>

/*
 * tcpserver.cpp
 *
 * Copyright (C) 2015-2017 Symeon Huang <hzwhuang@gmail.com>
 *
 * This file is part of the libQtShadowsocks.
 *
 * libQtShadowsocks is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * libQtShadowsocks is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libQtShadowsocks; see the file LICENSE. If not, see
 * <http://www.gnu.org/licenses/>.
 */


#include "tcprelayclient.h"
#include "tcprelayserver.h"
#include "tcpserver.h"
#include "util/common.h"
#include <QDebug>
#include <utility>

namespace QSS {

TcpServer::TcpServer(std::string method,
                     std::string password,
                     int timeout,
                     bool is_local,
                     bool auto_ban,
                     Address serverAddress)
    : method(std::move(method))
    , password(std::move(password))
    , isLocal(is_local)
    , autoBan(auto_ban)
    , serverAddress(std::move(serverAddress))
    , timeout(timeout)
{
}

TcpServer::~TcpServer()
{
    if (isListening()) {
        close();
    }
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    auto localSocket = std::make_unique<QTcpSocket>();
    localSocket->setSocketDescriptor(socketDescriptor);

    if (!isLocal && autoBan && Common::isAddressBanned(localSocket->peerAddress())) {
        QDebug(QtMsgType::QtInfoMsg).noquote() << "A banned IP" << localSocket->peerAddress()
                                               << "attempted to access this server";
        return;
    }

    //timeout * 1000: convert sec to msec
    std::shared_ptr<TcpRelay> con;
    if (isLocal) {
        con = std::make_shared<TcpRelayClient>(localSocket.release(),
                                               timeout * 1000,
                                               serverAddress,
                                               method,
                                               password);
        con->setProxy(m_proxyType, m_proxyServerAddress, m_proxyPort);
    } else {
        con = std::make_shared<TcpRelayServer>(localSocket.release(),
                                               timeout * 1000,
                                               serverAddress,
                                               method,
                                               password,
                                               autoBan);
    }
    conList.push_back(con);
    connect(con.get(), &TcpRelay::bytesRead, this, &TcpServer::bytesRead);
    connect(con.get(), &TcpRelay::bytesSend, this, &TcpServer::bytesSend);
    connect(con.get(), &TcpRelay::latencyAvailable,
            this, &TcpServer::latencyAvailable);
    connect(con.get(), &TcpRelay::finished, this, [con, this]() {
        conList.remove(con);
    });
}

void TcpServer::setProxy(int proxyType, const std::string& proxyServerAddress, const uint16_t& proxyPort) {
    m_proxyType = proxyType;
    m_proxyServerAddress = proxyServerAddress;
    m_proxyPort = proxyPort;
}

}  // namespace QSS
