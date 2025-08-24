#ifndef MRPCONFIG_H
#define MRPCONFIG_H

#include <unordered_map>
#include <string>

namespace mrpc {

class MrpcConfig {
public:
    void loadConfigFile(const char *config_file);
    std::string load(const std::string &key) const;

private:
    void trim(std::string &str);

private:
    std::unordered_map<std::string, std::string> m_configMap;
};

} // namespace mrpc

#endif