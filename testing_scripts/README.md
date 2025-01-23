# API Throughput Test
This script is designed to test the throughput of an API ping endpoint by sending requests at an increasing rate and measuring success rates, actual request rates, and elapsed times.

## Prerequisites
- aiohttp library

```sh
    pip install aiohttp
```

## Running the Script

```sh
    python throughput_test.py
```

## Results
- __Success rate:__ The percentage of successful responses from the server out of the total requests sent. 
- __Time taken:__ The time (in seconds) taken to send all the requests from the first to the last request. Ideally, this should be close to 1 second.
- __Real rate:__ The actual number of requests sent per second, calculated as `total requests / time taken`. This value indicates how closely the achieved rate matches the target rate.