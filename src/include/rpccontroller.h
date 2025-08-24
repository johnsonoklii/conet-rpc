#ifndef MRPC_CONTROLLER_H
#define MRPC_CONTROLLER_H

#include <string>

#include <google/protobuf/service.h>

namespace mrpc {

class MrpcController : public google::protobuf::RpcController {
public:
    MrpcController();

    void Reset() override;
    bool Failed() const override;
    std::string ErrorText() const override;
    void SetFailed(const std::string& reason) override;

    void StartCancel() override;
    bool IsCanceled() const override;
    void NotifyOnCancel(google::protobuf::Closure* callback) override;

private:
    bool m_failed;
    std::string m_error_info;
};

} // namespace mrpc


#endif