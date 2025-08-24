#include <semaphore.h>
#include <time.h>

#include "zookeeperutil.h"
#include "mrpcapplication.h"

using namespace mrpc;

void global_watcher(zhandle_t *zh, int type,
                    int state, const char *path,void *watcherCtx) {
    if (type == ZOO_SESSION_EVENT) {
        if (state == ZOO_CONNECTED_STATE) {
            sem_t *sem = (sem_t*)zoo_get_context(zh);
            sem_post(sem);
        }
    }
}

void zktest_string_completion(int rc, const char *name, const void *data) {
}

void zktest_stat_completion(int rc, const struct Stat *stat, const void *data) {

}

void data_completion(int rc, const char *value, int value_len,
                     const struct Stat *stat, const void *data) {
    if (rc == ZOK) {
        printf("Data: %.*s\n", value_len, value);
    } else {
        printf("Error: %s\n", zerror(rc));
    }
}

ZkClient::ZkClient(): m_zhandle(nullptr) {}

ZkClient::~ZkClient() {
    printf("zkclient::~zkclient start...\n");
    if (m_zhandle) {
        zookeeper_close(m_zhandle);
        printf("zkclient::~zkclient zookeeper_close...\n");
    }
    printf("zkclient::~zkclient end...\n");
}

void ZkClient::Start() {
    std::string ip = MrpcApplication::getConfig().load("zookeeperip");
    std::string port = MrpcApplication::getConfig().load("zookeeperport");
    std::string host = ip + ":" + port;

    m_zhandle = zookeeper_init(host.c_str(), global_watcher, 30000, 0, nullptr, 0);
    if (m_zhandle == nullptr) {
        printf("zookeeper_init error\n");
        exit(EXIT_FAILURE);
    }
    
    // zookeeper_init是异步进行连接，通过global_watcher回调函数判断是否连接成功
    // 信号量需要等待global_watcher给出信号
    sem_t sem;
    sem_init(&sem, 0, 0);
    zoo_set_context(m_zhandle, &sem);

    sem_wait(&sem);

    printf("zookeeper_init success\n");
}

void ZkClient::Create(const char* path, const char* data, int datalen, int state) {
    // int flag = zoo_aexists(m_zhandle, path, 0, zktest_stat_completion, "aexists");
    int flag = zoo_exists(m_zhandle, path, 0, nullptr);
    if (flag == ZNONODE) {
        char buffer[512];
        flag = zoo_create(m_zhandle, path, data, datalen,
                            &ZOO_OPEN_ACL_UNSAFE, state, buffer, sizeof(buffer) - 1);
        if (flag) {
            printf("zoo_create error, flag = %d\n", flag);
            zookeeper_close(m_zhandle);
            exit(EXIT_FAILURE);
        }
    }
}

std::string ZkClient::GetData(const char* path) {
    char buf[128];
    int len = sizeof(buf);

    int flag = zoo_get(m_zhandle, path, 0, buf, &len, nullptr);
    if (flag != ZOK) {
        return "";
    }
    
    return buf;
}