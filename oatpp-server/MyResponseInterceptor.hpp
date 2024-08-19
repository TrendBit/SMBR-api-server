#ifndef MyResponseInterceptor_hpp
#define MyResponseInterceptor_hpp

#include "oatpp/web/server/interceptor/ResponseInterceptor.hpp"

class MyResponseInterceptor : public oatpp::web::server::interceptor::ResponseInterceptor {
public:

    std::shared_ptr<OutgoingResponse> intercept(const std::shared_ptr<IncomingRequest>& request,
                                                const std::shared_ptr<OutgoingResponse>& response) override {
        response->putHeader("Access-Control-Allow-Origin", "*");
        return response;
    }
};

#endif /* MyResponseInterceptor_hpp */
