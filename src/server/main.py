from config import DASHBOARD_PATH
from fastapi import FastAPI, Request
from fastapi.responses import HTMLResponse, StreamingResponse
import uvicorn
import socket
from serial_interface import air_data_receiver


app = FastAPI()

def get_local_ip():
    """Get the local IP address of the machine"""
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(('10.255.255.255', 1))
        ip = s.getsockname()[0]
    except Exception:
        ip = '127.0.0.1'
    finally:
        s.close()
    return ip

@app.get("/", response_class=HTMLResponse)
async def get_dashboard(request: Request):
    server_ip = get_local_ip()
    server_port = request.url.port

    print(f"server ip {server_ip}")
    print(f"server_port {server_port}")

    with open(DASHBOARD_PATH, 'r') as file:
        content = file.read()

    content = content.replace(
        'http://localhost:8000/api/air_data',
        f'http://{server_ip}:{server_port}/api/air_data'
    )
    return HTMLResponse(content=content)

@app.get("/api/air_data")
async def get_air_data():
    """SSE endpoint for air quality data"""
    return StreamingResponse(
        air_data_receiver,
        media_type="text/event-stream",
        headers={
            "Access-Control-Allow-Origin": "*",
            "Cache-Control": "no-cache",
            "Connection": "keep-alive",
        }
    )

if __name__ == "__main__":
    air_data_receiver = air_data_receiver()
    uvicorn.run(app, host="0.0.0.0", port=8000)
