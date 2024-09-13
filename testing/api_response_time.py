import requests
import time

BASE_URL = "http://192.168.0.209:8089/ping-direct"

def measure_http_latency_direct(repeats=10):
    total_response_time = 0
    successful_requests = 0
    failed_requests = 0

    for i in range(repeats):
        try:
            start_time = time.time()
            response = requests.get(BASE_URL, timeout=1)
            end_time = time.time()

            if response.status_code == 200:
                response_time = (end_time - start_time) * 1000  
                total_response_time += response_time
                successful_requests += 1
            else:
                failed_requests += 1
        except requests.exceptions.Timeout:
            failed_requests += 1
        except requests.exceptions.RequestException:
            failed_requests += 1

    if successful_requests > 0:
        average_response_time = total_response_time / successful_requests
        print(f"\nTotal attempts: {repeats}")
        print(f"Successful pings: {successful_requests}")
        print(f"Failed pings: {failed_requests}")
        print(f"Average response time: {average_response_time:.2f} ms")
    else:
        print("\nNo successful responses were received.")

if __name__ == "__main__":
    measure_http_latency_direct(100)  

