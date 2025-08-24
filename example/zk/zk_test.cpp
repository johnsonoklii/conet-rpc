#include "mrpcapplication.h"
#include "zookeeperutil.h"
using namespace mrpc;

int main(int argc, char **argv) {
    MrpcApplication::init(argc, argv);
    ZkClient zkClient;
    zkClient.Start();
    
    std::string node = "/UserService";
    std::string data = "127.0.0.1:9999";
    zkClient.Create(node.c_str(), data.c_str(), data.size(), ZOO_EPHEMERAL);

    getchar();
    return 0 ;
}