#include <iostream>
#include <curl/curl.h>
#include <chrono>

const std::string BASE_URL = "http://192.168.0.69:8089/control/ping/";

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    return size * nmemb;
}

bool send_ping(CURL *curl) {
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, BASE_URL.c_str());
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 1000L); 

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

    res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        return http_code == 200;
    }
    return false;
}

void test_ping_sequence_for_duration(int duration = 10) {
    CURL *curl;
    curl = curl_easy_init();

    if (!curl) {
        std::cerr << "Error initializing CURL." << std::endl;
        return;
    }

    int count = 0;
    int success_count = 0;
    double total_time = 0.0;

    auto start_time = std::chrono::steady_clock::now();

    while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count() < duration) {
        auto request_start = std::chrono::steady_clock::now();

        if (send_ping(curl)) {
            success_count++;
        }

        auto request_end = std::chrono::steady_clock::now();
        total_time += std::chrono::duration<double, std::milli>(request_end - request_start).count();
        count++;
    }

    curl_easy_cleanup(curl);

    double avg_time_per_request = count > 0 ? total_time / count : 0.0;

    std::cout << "Total number of ping attempts in " << duration << " seconds: " << count << std::endl;
    std::cout << "Successful pings: " << success_count << std::endl;
    std::cout << "Failed pings: " << count - success_count << std::endl;
    std::cout << "Average time per request: " << avg_time_per_request << " ms" << std::endl;
}

int main() {
    test_ping_sequence_for_duration(10);
    return 0;
}

