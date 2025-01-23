import aiohttp
import asyncio
import time
import signal

url = "http://192.168.0.69:8089/sensor/ping/" # Change to your IP address
initial_rate = 50  # Initial number of requests per second
rate_increment = 10  # Increment in requests per second
pause_duration = 2  # Pause between tests (in seconds)
stop_event = asyncio.Event()


async def send_request(session):
    try:
        async with session.get(url, timeout=1) as response:
            if response.status == 200:
                return True
    except asyncio.TimeoutError:
        pass
    except Exception:
        pass
    return False


async def send_requests(rate):
    success = 0
    total = 0
    interval = 1 / rate  

    async with aiohttp.ClientSession() as session:
        tasks = []
        start_time = time.time() 

        for _ in range(rate):
            tasks.append(asyncio.create_task(send_request(session)))
            await asyncio.sleep(interval)  

        end_time = time.time()  
        responses = await asyncio.gather(*tasks)

        success = sum(1 for r in responses if r)
        total = len(responses)

        elapsed = end_time - start_time  
        real_rate = total / elapsed 
        return success, total, elapsed, real_rate


async def main():
    rate = initial_rate

    try:
        while not stop_event.is_set():
            print(f"Testing {rate} requests per second...")
            success, total, elapsed, real_rate = await send_requests(rate)

            success_rate = (success / total) * 100 if total > 0 else 0
            print(f"{rate} req/s - {success_rate:.2f}% ({success}/{total}), "
                  f"time taken: {elapsed:.2f}s, real rate: {real_rate:.2f} req/s")

            await asyncio.sleep(pause_duration) 
            rate += rate_increment
    except asyncio.CancelledError:
        print("Shutting down...")
    finally:
        print("All tasks have been cleaned up.")


def handle_exit():
    stop_event.set()


if __name__ == "__main__":
    for sig in (signal.SIGINT, signal.SIGTERM):
        signal.signal(sig, lambda s, f: handle_exit())
    asyncio.run(main())
