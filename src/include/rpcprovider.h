#ifndef MRPC_PROVIDER_H
#define MRPC_PROVIDER_H

#include <memory>
#include <string>
#include <unordered_map>

#include <google/protobuf/descriptor.h>
#include <google/protobuf/service.h>

#include <conet/net/tcp_connection.h>
#include <conet/net/tcp_server.h>

#include "rpcconfig.h"

namespace mrpc {

class MrpcProvider {
public:
    MrpcProvider();

    void notifyService(google::protobuf::Service *service);
    void run();

private:

    struct ServiceInfo{
        google::protobuf::Service* m_service;
        std::unordered_map<std::string, const google::protobuf::MethodDescriptor*> m_methodMap;
    };

    void onConnection(const conet::net::TcpConnection::sptr&);
    void onMessage(const conet::net::TcpConnection::sptr&, conet::net::Buffer*);

    void sendRpcResponse(const conet::net::TcpConnection::sptr&, google::protobuf::Message*);

    std::unordered_map<std::string, ServiceInfo> m_serviceMap;
};

} // namespace mrpc

#endif