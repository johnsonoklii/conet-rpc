#ifndef ZOOKEEPERUTIL_H
#define ZOOKEEPERUTIL_H

#define THREADED

#include <zookeeper/zookeeper.h>

#include <string>

namespace mrpc {

class ZkClient {
public:
    ZkClient();
    ~ZkClient();
    void Start();
    void Create(const char* path, const char* data, int datalen, int state = 0);
    std::string GetData(const char* path);
private:
    zhandle_t* m_zhandle;
};

} // namespace mrpc

#endif