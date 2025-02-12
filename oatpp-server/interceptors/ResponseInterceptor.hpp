#pragma once
#include "oatpp/web/server/interceptor/ResponseInterceptor.hpp"

/**
 * @class ResponseInterceptor
 * @brief A custom response interceptor that adds a CORS header to outgoing responses.
 */
class ResponseInterceptor : public oatpp::web::server::interceptor::ResponseInterceptor {
public:

    /**
     * @brief Intercepts the outgoing response to add CORS headers.
     * 
     * @param request The incoming request that triggered this response.
     * @param response The outgoing response to be intercepted and modified.
     * @return The modified outgoing response.
     */
    std::shared_ptr<OutgoingResponse> intercept(const std::shared_ptr<IncomingRequest>& request,
                                                const std::shared_ptr<OutgoingResponse>& response) override {
        response->putHeader("Access-Control-Allow-Origin", "*");
        return response;
    }
};


