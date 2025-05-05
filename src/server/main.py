from config import DASHBOARD_PATH
from fastapi import FastAPI
from fastapi.responses import StreamingResponse, FileResponse
import uvicorn
from serial_interface import air_data_receiver


app = FastAPI()

@app.get("/")
async def get_dashboard():
    return FileResponse(DASHBOARD_PATH)

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
