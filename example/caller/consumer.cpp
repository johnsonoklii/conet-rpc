#include "user.pb.h"

#include "mrpcapplication.h"
#include "mrpcchannel.h"
#include "mrpccontroller.h"

using namespace mrpc;
int main(int argc, char** argv) {
    MrpcApplication::init(argc, argv);

    user::UserServiceRpc_Stub stub(new MrpcChannel);
    user::LoginRequest request;
    request.set_username("zhangsan");
    request.set_password("123456");

    user::LoginResponse response;
    MrpcController controller;

    stub.Login(&controller, &request, &response, nullptr);

    if (controller.Failed()) {
        printf("stub.Login fail! error message is %s\n",controller.ErrorText().c_str());
        return -1;
    }

    if (response.rscode().code() == 0) {
        printf("rpc login success! tolen is %s\n", response.token().c_str());
    } else {
        printf("rpc login fail! error code is %d, error message is %s\n", response.rscode().code(), response.rscode().message().c_str());
    }

    return 0;
}