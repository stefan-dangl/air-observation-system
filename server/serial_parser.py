import serial
import json

# UART Configuration
SERIAL_PORT = '/dev/ttyACM0'
BAUDRATE = 9600

def parse_sensor_data(data_str):
    """Parse sensor data string into a structured dictionary."""
    try:
        # Split the string by semicolons
        parts = data_str.strip().split(';')

        if len(parts) != 6:
            raise ValueError("Invalid data format. Expected 6 values separated by ';'.")

        # Convert and assign to structured fields
        sensor_data = {
            "sensor_id": int(parts[0]),
            "CH4 [% LEL]": float(parts[1]),
            "CO [ppm]": float(parts[2]),
            "O2 [%]": float(parts[3]),
            "CO2 [ppm]": float(parts[4]),
            "NO2 [ppm]": float(parts[5])
        }
        return sensor_data
    except (ValueError, IndexError) as e:
        print(f"Error parsing data: {e}")
        return None

def main():
    # Initialize serial connection
    try:
        ser = serial.Serial(SERIAL_PORT, BAUDRATE, timeout=1)
        print(f"Connected to {SERIAL_PORT} at {BAUDRATE} baud.")

        while True:
            if ser.in_waiting > 0:
                # Read a line from serial
                line = ser.readline().decode('utf-8').strip()
                print(f"Raw data: {line}")

                # Parse the data
                sensor_data = parse_sensor_data(line)
                if sensor_data:
                    # Print as JSON for readability
                    print(json.dumps(sensor_data, indent=4))

    except serial.SerialException as e:
        print(f"Serial error: {e}")
    except KeyboardInterrupt:
        print("\nExiting...")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()

if __name__ == "__main__":
    main()
