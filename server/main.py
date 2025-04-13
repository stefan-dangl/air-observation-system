from fastapi import FastAPI
from fastapi.responses import StreamingResponse
from utils import air_data_receiver
import uvicorn

app = FastAPI()

@app.get("/api/air_data")
async def air_data():
    """SSE endpoint for air quality data"""
    return StreamingResponse(
        # air_data_generator(),
        air_data_receiver(),
        media_type="text/event-stream",
        headers={
            "Access-Control-Allow-Origin": "*",
            "Cache-Control": "no-cache",
            "Connection": "keep-alive",
        }
    )

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)
