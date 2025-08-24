#ifndef MRPCAPPLICATION_H
#define MRPCAPPLICATION_H

#include "rpcconfig.h" 

namespace mrpc {

class MrpcApplication
{
public:
    
    static MrpcApplication& getInstance();
    
    static void init(int argc, char **argv);
    static MrpcConfig& getConfig();

private:
    MrpcApplication();
    MrpcApplication(const MrpcApplication&) = delete;
    MrpcApplication(const MrpcApplication&&) = delete;

    static MrpcConfig m_config;
};

} // namespace mrpc



#endif