#ifndef MRPCCHANNEL_H
#define MRPCCHANNEL_H

#include <memory>

#include <google/protobuf/service.h>

#include "zookeeperutil.h"

namespace mrpc {

class MrpcChannel: public google::protobuf::RpcChannel {
public:
    MrpcChannel();

    void CallMethod(const google::protobuf::MethodDescriptor *method,
                    google::protobuf::RpcController *controller,
                    const google::protobuf::Message *request,
                    google::protobuf::Message *response,
                    google::protobuf::Closure *done);

private:
    std::unique_ptr<ZkClient> m_zkClient;
};

}



#endif