import asyncio
import json
from datetime import datetime
import serial
import json

SERIAL_PORT = '/dev/ttyACM0'
STORAGE_PATH = 'stored_data/stored_data.csv'
BAUDRATE = 9600

def parse_sensor_data(data_str):
    """Parse sensor data string into a structured dictionary."""
    try:
        # Split the string by semicolons
        parts = data_str.strip().split(';')

        if len(parts) != 7:
            raise ValueError("Invalid data format. Expected 7 values separated by ';'.")

        # Convert and assign to structured fields
        sensor_data = {
            "timestamp": parts[0],
            "sensor_id": int(parts[1]),
            "CH4 [% LEL]": float(parts[2]),
            "CO [ppm]": float(parts[3]),
            "O2 [%]": float(parts[4]),
            "CO2 [ppm]": float(parts[5]),
            "NO2 [ppm]": float(parts[6])
        }
        return sensor_data
    except (ValueError, IndexError) as e:
        print(f"Error parsing data: {e}")
        return None

async def air_data_receiver():
    """Generator that yields air data as SSE events"""

    try:
        ser = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1)
        ser.reset_input_buffer()
        ser.reset_output_buffer()

        print(f"Connected to {SERIAL_PORT} at {BAUDRATE} baud.")

        while True:
            if ser.in_waiting > 0:
                # Read a line from serial
                line = ser.readline().decode('utf-8').strip()

                line = datetime.now().strftime("%Y-%m-%d %H:%M:%S") + ";" + line
                print(f"Raw data: {line}")

                with open(STORAGE_PATH, 'a+') as file:  # Use 'a+' mode to read and append
                    file.seek(0)  # Move to start of file for reading
                    if file.read().strip() == '':
                        file.write("timestamp;sensor_id;CH4 [% LEL];CO [ppm];O2 [%];CO2 [ppm];NO2 [ppm]\n")
                    file.write(line + "\n")

                # Parse the data
                sensor_data = parse_sensor_data(line)
                if sensor_data:
                    # Print as JSON for readability
                    yield f"event: air_data\ndata: {json.dumps(sensor_data)}\n\n"


                await asyncio.sleep(0.1)  # Send data every second



    except serial.SerialException as e:
        print(f"Serial error: {e}")
    except KeyboardInterrupt:
        print("\nExiting...")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
