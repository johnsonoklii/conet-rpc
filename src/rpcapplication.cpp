#include <cstdio>
#include <unistd.h>

#include <string>


#include "rpcapplication.h"

using namespace mrpc;

void ShowArgUsage() {
    printf("format: command -i config_file\n");
}

MrpcConfig MrpcApplication::m_config;

MrpcApplication::MrpcApplication(){ }

MrpcApplication& MrpcApplication::getInstance() {
    static MrpcApplication app;
    return app;
}

void MrpcApplication::init(int argc, char **argv) {
    if(argc < 2) {
        ShowArgUsage();
        exit(EXIT_FAILURE);
    }

    int c = 0;
    const char* config_file = "";
    while ( (c = getopt(argc, argv, "i:")) != -1 ) {
        switch (c) {
            case 'i':
                config_file = optarg;
                break;
            case '?':
                ShowArgUsage();
                exit(EXIT_FAILURE);
            case ':':
                ShowArgUsage();
                exit(EXIT_FAILURE);
            default:
                ShowArgUsage();
                exit(EXIT_FAILURE);
        }
    }

    // 加载配置
    m_config.loadConfigFile(config_file);
}

MrpcConfig& MrpcApplication::getConfig() {
    return m_config;
}