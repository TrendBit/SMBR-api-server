import requests
import time

BASE_URL = "http://192.168.0.209:8089/control/ping-with-seq/"


def send_ping_with_seq(seq_num):
    try:
        url = f"{BASE_URL}{seq_num}"
        response = requests.get(url, timeout=2) 
        if response.status_code == 200:
            print(f"Ping {seq_num}: Response: {response.json()}")
        else:
            print(f"Ping {seq_num}: Error, status code {response.status_code}")
    except requests.exceptions.Timeout:
        print(f"Ping {seq_num}: Timeout - no response received.")
    except requests.exceptions.RequestException as e:
        print(f"Ping {seq_num}: Error sending ping: {e}")

def test_ping_sequence_for_duration(duration=10):
    start_time = time.time()
    count = 0
    seq_num = 1

    while time.time() - start_time < duration:
        send_ping_with_seq(seq_num)
        count += 1
        seq_num += 1

    print(f"Number of pings sent in {duration} seconds: {count}")

if __name__ == "__main__":
    test_ping_sequence_for_duration(10)
