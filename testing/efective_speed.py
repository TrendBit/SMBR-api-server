import requests
import time

BASE_URL = "http://192.168.0.209:8089/control/ping/"

session = requests.Session()

def send_ping(session):
    try:
        response = session.get(BASE_URL, timeout=1)  
        if response.status_code == 200:
            return True  
        else:
            return False  
    except requests.exceptions.Timeout:
        return False  
    except requests.exceptions.RequestException:
        return False  

def test_ping_sequence_for_duration(duration=10):
    start_time = time.time()
    count = 0
    success_count = 0
    total_time = 0

    while time.time() - start_time < duration:
        request_start = time.time()
        if send_ping(session):  
            success_count += 1
        request_end = time.time()

        total_time += (request_end - request_start)
        count += 1

    if count > 0:
        avg_time_per_request = (total_time / count) * 1000  
    else:
        avg_time_per_request = 0

    print(f"Number of ping attempts in {duration} seconds: {count}")
    print(f"Successful pings: {success_count}")
    print(f"Failed pings: {count - success_count}")
    print(f"Average processing time per request: {avg_time_per_request:.2f} ms")

if __name__ == "__main__":
    test_ping_sequence_for_duration(5)

