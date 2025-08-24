#include "mrpccontroller.h"

using namespace mrpc;

MrpcController::MrpcController() {
    m_failed = false;
    m_error_info = "";
}

void MrpcController::Reset() {
    m_failed = false;
    m_error_info = "";
}

bool MrpcController::Failed() const {
    return m_failed;
}

std::string MrpcController::ErrorText() const {
    return m_error_info;
}

void MrpcController::SetFailed(const std::string& reason) {
    m_failed = true;
    m_error_info = reason;
}

void MrpcController::StartCancel() {

}

bool MrpcController::IsCanceled() const {
    return false;
}

void MrpcController::NotifyOnCancel(google::protobuf::Closure* callback) {

}