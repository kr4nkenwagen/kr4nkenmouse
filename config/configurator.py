import serial
import sys
import time
import argparse

def main():
    parser = argparse.ArgumentParser(description='Send commands to ESP32-C3 over Serial')
    parser.add_argument('commands', nargs='+', help='Commands to send (e.g. set_sens=100)')
    parser.add_argument('--port', default='/dev/ttyACM0', help='Serial port (e.g. COM3 or /dev/ttyACM0)')
    parser.add_argument('--baud', type=int, default=115200, help='Baud rate (default: 115200)')
    args = parser.parse_args()
    try:
        ser = serial.Serial(args.port, args.baud, timeout=1)
        ser.dtr = False
        ser.rts = False
        time.sleep(0.1)
        for cmd in args.commands:
            print(f"Sending: {cmd}")
            ser.write(f"{cmd}\n".encode('utf-8'))
            time.sleep(0.05)
        ser.close()
        print("Done.")
    except serial.SerialException as e:
        print(f"Error: Could not open serial port {args.port}: {e}")
if __name__ == "__main__":
    main()
