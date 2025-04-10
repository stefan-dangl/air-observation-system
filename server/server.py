from fastapi import FastAPI, Response
from fastapi.responses import StreamingResponse
import asyncio
import json
from datetime import datetime
import random
import time

app = FastAPI()

def generate_air_data(sensor_id: int = 1):
    """Generate simulated air quality data"""
    return {
        "timestamp": datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
        "sensor_id": sensor_id,
        "CH4 [% LEL]": round(random.uniform(0, 20), 1),
        "CO [ppm]": random.randint(0, 50),
        "O2 [%]": round(random.uniform(18, 22), 1),
        "CO2 [ppm]": random.randint(400, 1000),
        "NO2 [ppm]": round(random.uniform(0, 0.5), 1)
    }

async def air_data_generator():
    """Generator that yields air data as SSE events"""
    while True:
        data = generate_air_data()
        # Format as SSE
        yield f"event: air_data\ndata: {json.dumps(data)}\n\n"
        await asyncio.sleep(1)  # Send data every second

@app.get("/api/air_data")
async def air_data():
    """SSE endpoint for air quality data"""
    return StreamingResponse(
        air_data_generator(),
        media_type="text/event-stream",
        headers={
            "Access-Control-Allow-Origin": "*",
            "Cache-Control": "no-cache",
            "Connection": "keep-alive",
        }
    )

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)
