import requests
import time
from concurrent.futures import ThreadPoolExecutor, as_completed

BASE_URL = "http://192.168.0.209:8089/control/ping"

num_pings = 80

success_count = 0
timeout_count = 0
error_504_count = 0

def send_ping():
    global success_count, timeout_count, error_504_count
    try:
        response = requests.get(BASE_URL, timeout=7) 
        if response.status_code == 200:
            success_count += 1
            return f"Ping: Response: {response.json()}"
        elif response.status_code == 504:
            error_504_count += 1
            return f"Ping: Error, status code 504"
        else:
            return f"Ping: Error, status code {response.status_code}"
    except requests.exceptions.Timeout:
        timeout_count += 1
        return f"Ping: Timeout - no response."
    except requests.exceptions.RequestException as e:
        return f"Ping: Error sending ping: {e}"

def test_ping_sequence():
    global success_count, timeout_count, error_504_count
    
    with ThreadPoolExecutor(max_workers=num_pings) as executor:
        futures = []
        for i in range(num_pings):
            futures.append(executor.submit(send_ping))
            time.sleep(0.001)  
        
        for future in as_completed(futures):
            print(future.result())
    
    print("\n--- Statistics ---")
    print(f"Total pings: {num_pings}")
    print(f"Successful pings: {success_count}")
    print(f"Pings with 504 error: {error_504_count}")
    print(f"Timeouts from script: {timeout_count}")

if __name__ == "__main__":
    test_ping_sequence()

