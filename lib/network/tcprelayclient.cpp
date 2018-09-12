/*
 * tcprelayclient.cpp - the source file of TcpRelayClient class
 *
 * Copyright (C) 2018 Symeon Huang <hzwhuang@gmail.com>
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
#include "util/common.h"
#include <QDebug>
#include <utility>

namespace QSS {

TcpRelayClient::TcpRelayClient(QTcpSocket *localSocket,
                               int timeout,
                               Address server_addr,
                               const std::string& method,
                               const std::string& password)
    : TcpRelay(localSocket, timeout, server_addr, method, password)
{
}

void TcpRelayClient::handleStageAddr(std::string &data)
{
    auto cmd = static_cast<int>(data.at(1));
    if (cmd == 3) {//CMD_UDP_ASSOCIATE
        qDebug("UDP associate");
        static const char header_data [] = { 5, 0, 0 };
        QHostAddress addr = local->localAddress();
        uint16_t port = local->localPort();
        std::string toWrite = std::string(header_data, 3) + Common::packAddress(addr, port);
        local->write(toWrite.data(), toWrite.length());
        stage = UDP_ASSOC;
        return;
    } if (cmd == 1) {//CMD_CONNECT
        data = data.substr(3);
    } else {
        qCritical("Unknown command %d", cmd);
        close();
        return;
    }

    int header_length = 0;
    Common::parseHeader(data, remoteAddress, header_length);
    if (header_length == 0) {
        qCritical("Can't parse header. Wrong encryption method or password?");
        close();
        return;
    }

    QDebug(QtMsgType::QtInfoMsg).noquote().nospace()
            << "Connecting " << remoteAddress << " from "
            << local->peerAddress().toString() << ":" << local->peerPort();

    stage = DNS;
    static const char res [] = { 5, 0, 0, 1, 0, 0, 0, 0, 16, 16 };
    static const QByteArray response(res, 10);
    local->write(response);
    std::string toWrite = encryptor->encrypt(data);
    dataToWrite += toWrite;

    if (proxy.type() == QNetworkProxy::HttpProxy || proxy.type() == QNetworkProxy::Socks5Proxy) {
        // if proxy is set, then the proxy will lookup for dns.
        stage = CONNECTING;
        startTime = QTime::currentTime();
        remote->connectToHost(serverAddress.getAddress().c_str(), serverAddress.getPort());
    } else {
        serverAddress.lookUp([this](bool success) {
            if (success) {
                stage = CONNECTING;
                startTime = QTime::currentTime();
                remote->connectToHost(serverAddress.getFirstIP(), serverAddress.getPort());
            } else {
                QDebug(QtMsgType::QtDebugMsg).noquote() << "Failed to lookup server address. Closing TCP connection.";
                close();
            }
        });
    }
}

void TcpRelayClient::handleLocalTcpData(std::string &data)
{
    if (stage == STREAM) {
        data = encryptor->encrypt(data);
        writeToRemote(data.data(), data.size());
    } else if (stage == INIT) {
        static const char reject_data [] = { 0, 91 };
        static const char accept_data [] = { 5, 0 };
        static const QByteArray reject(reject_data, 2);
        static const QByteArray accept(accept_data, 2);
        if (data[0] != char(5)) {
            qCritical("An invalid socket connection was rejected. "
                      "Please make sure the connection type is SOCKS5.");
            local->write(reject);
        } else {
            local->write(accept);
        }
        stage = ADDR;
    } else if (stage == CONNECTING || stage == DNS) {
        // take DNS into account, otherwise some data will get lost
        dataToWrite += encryptor->encrypt(data);
    } else if (stage == ADDR) {
        handleStageAddr(data);
    } else {
        qCritical("Local unknown stage.");
    }
}

void TcpRelayClient::handleRemoteTcpData(std::string &data)
{
    data = encryptor->decrypt(data);
}

}  // namespace QSS
