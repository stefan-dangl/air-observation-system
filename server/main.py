from fastapi import FastAPI
from fastapi.responses import StreamingResponse
from fastapi.staticfiles import StaticFiles
from utils import air_data_receiver
import uvicorn

app = FastAPI()
app.mount("/", StaticFiles(directory="../dashboard", html=True), name="static")

@app.get("/api/air_data")
async def air_data():
    """SSE endpoint for air quality data"""
    return StreamingResponse(
        serial_receiver,
        media_type="text/event-stream",
        headers={
            "Access-Control-Allow-Origin": "*",
            "Cache-Control": "no-cache",
            "Connection": "keep-alive",
        }
    )

if __name__ == "__main__":
    serial_receiver = air_data_receiver()
    uvicorn.run(app, host="0.0.0.0", port=8000)
