#include <cstdio>

#include "rpcprovider.h"
#include "rpcconfig.h"
#include "rpcapplication.h"
#include "zookeeperutil.h"

#include "rpcheader.pb.h"

using namespace mrpc;

MrpcProvider::MrpcProvider()  {
}

void MrpcProvider::notifyService(google::protobuf::Service *service) {
    ServiceInfo serviceInfo;

    const google::protobuf::ServiceDescriptor* pserviceDesc = service->GetDescriptor();

    std::string serviceName = pserviceDesc->name();
    // printf("serviceName:%s\n", serviceName.c_str());

    int methodCount = pserviceDesc->method_count();

    for (int i = 0; i < methodCount; ++i) {
        const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc->method(i);
        std::string methodName = pmethodDesc->name();

        // printf("methodName:%s\n", methodName.c_str());
        serviceInfo.m_methodMap[methodName] = pmethodDesc;
    }

    serviceInfo.m_service = service;
    m_serviceMap[serviceName] = serviceInfo;
}

void MrpcProvider::run() {
    MrpcConfig config = MrpcApplication::getConfig();
    std::string ip = config.load("rpcserverip");
    int port = atoi(config.load("rpcserverport").c_str());
   
    conet::net::TcpServer server(ip, port, "MrpcProvider");
    server.setConnectionCallBack(std::bind(&MrpcProvider::onConnection, this, std::placeholders::_1));
    server.setMessageCallBack(std::bind(&MrpcProvider::onMessage, this, std::placeholders::_1, std::placeholders::_2));
    server.setThreadNum(4);

    // 注册中心
    ZkClient zkClient;
    zkClient.Start();
    for (auto &service : m_serviceMap) {
        std::string serviceNode = "/" + service.first;
        zkClient.Create(serviceNode.c_str(), nullptr, 0, 0);
        for (auto &method : service.second.m_methodMap) {
            std::string methodNode = serviceNode + "/" + method.first;
            char buf[64];
            sprintf(buf, "%s:%d", ip.c_str(), port);
            zkClient.Create(methodNode.c_str(), buf, strlen(buf), ZOO_EPHEMERAL);
        }
    }

    printf("MrpcProvider start service at ip %s, port %d\n", ip.c_str(), port);

    server.start();
}

void MrpcProvider::onConnection(const conet::net::TcpConnection::sptr& conn) {
    if (!conn->connected()) {
       conn->shutdown();
    }
}

/*

协议格式: 
    header_size(4) + header_str + args_str

*/
void MrpcProvider::onMessage(const conet::net::TcpConnection::sptr& conn, conet::net::Buffer* buffer) {
    // FIXME: 不应该一次性读完，可能有半包，应该先4个字节读出来，然后根据header_size再读剩下的数据
    std::string recv_str = buffer->retrieveAllAsString();

    int header_size = 0;
    recv_str.copy((char*)&header_size, 4);

    std::string header_str = recv_str.substr(4, header_size);
    RpcHeader header;

    std::string service_name;
    std::string method_name;
    uint32_t args_size;

    if (header.ParseFromString(header_str)) {
        service_name = header.service_name();
        method_name = header.method_name();
        args_size = header.args_size();
    } else {
        printf("parse header error\n");
        return;
    }

    std::string args_str = recv_str.substr(4 + header_size, args_size);

    printf("service_name: %s\n", service_name.c_str());
    printf("method_name: %s\n", method_name.c_str());
    printf("args_str: %s\n", args_str.c_str());

    auto it = m_serviceMap.find(service_name);
    if (it == m_serviceMap.end()) {
        printf("service_name: %s not exist\n", service_name.c_str());
        return;
    }

    auto mit = it->second.m_methodMap.find(method_name);
    if (mit == it->second.m_methodMap.end()) {
        printf("method_name: %s not exist\n", method_name.c_str());
        return;
    }

    google::protobuf::Service* service = it->second.m_service;          // UserService
    const google::protobuf::MethodDescriptor* method = mit->second;     // Login

    google::protobuf::Message* request = service->GetRequestPrototype(method).New();  // LoginRequest
    if (!request->ParseFromString(args_str)) {
        printf("request parse error\n");
        return;
    }

    google::protobuf::Message* response = service->GetResponsePrototype(method).New();  // LoginResponse

    google::protobuf::Closure* done = google::protobuf::NewCallback<MrpcProvider
                                                                    , const conet::net::TcpConnection::sptr&
                                                                    , google::protobuf::Message*>
                                                                    (this, &MrpcProvider::sendRpcResponse, conn, response);

    service->CallMethod(method, nullptr, request, response, done);
}

void MrpcProvider::sendRpcResponse(const conet::net::TcpConnection::sptr& conn, google::protobuf::Message* response) {
    std::string response_str;
    if (response->SerializeToString(&response_str)) {
        conn->send(response_str);
    } else {
        printf("serialize response error\n");
    }
    
    conn->shutdown();
}