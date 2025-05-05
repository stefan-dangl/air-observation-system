from config import SERIAL_PORT, STORAGE_PATH, BAUD_RATE
import json
from datetime import datetime
import serial


def _parse_sensor_data(data_str):
    """Parse sensor data into json format"""

    if not data_str:
        return None

    try:
        parts = data_str.strip().split(';')

        if len(parts) != 4:
            raise ValueError("Invalid data format. Expected 4 values separated by ';'.")

        parsed_data = {
            "timestamp": parts[0],
            "sensor_id": int(parts[1]),
            "CO2 [ppm]": float(parts[2]),
            "CH4 [% LEL]": float(parts[3]),
        }
        return parsed_data
    except (ValueError, IndexError) as e:
        print(f"Error parsing data: {e}")
        return None


async def air_data_receiver():
    """Generator that reads air data from serial interface and yields it as SSE events"""

    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        ser.reset_input_buffer()
        ser.reset_output_buffer()

        print(f"Connected to {SERIAL_PORT} at {BAUD_RATE} baud.")

        while True:
            if ser.in_waiting > 0:
                sensor_data = ser.readline().decode('utf-8').strip()

                sensor_data = datetime.now().strftime("%Y-%m-%d %H:%M:%S") + ";" + sensor_data        # extend line by date and time
                print(f"Data: {sensor_data}")

                with open(STORAGE_PATH, 'a+') as file:
                    file.seek(0)
                    if file.read().strip() == '':
                        file.write("timestamp;sensor_id;CO2 [ppm];CH4 [% LEL]\n")
                    file.write(sensor_data + "\n")

                parsed_sensor_data = _parse_sensor_data(sensor_data)
                if parsed_sensor_data:
                    yield f"event: air_data\ndata: {json.dumps(parsed_sensor_data)}\n\n"

    except serial.SerialException as e:
        print(f"Serial error: {e}")
    except KeyboardInterrupt:
        print("\nExiting...")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()
