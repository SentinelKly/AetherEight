import serial
import time
import sys
import os

WRITE_COMMAND = b'35'
READ_COMMAND = b'19'
STANDBY_COMMAND = b'3'
READ_BYTE = b'51'

RESPOND_STATUS = ["UNKNOWN", "STANDBY", "WRITING", "KNOWN"]

#port = serial.tools.list_ports.comports()[0].device

def clear_screen():
    if os.name == 'nt':
        os.system('cls')
    else:
        os.system('clear')

def open_serial_port(comport):
    return serial.Serial(port=comport, baudrate=9600, timeout=10)

def data_into_buffer(file):
    with open(file, "rb") as bin_file:
        return bin_file.read()

def byte_to_str(byte):
    return f'{byte}\n'

def get_device_response(device, output=False):
    resp = int.from_bytes(device.read(1), 'big', signed=False)
    if output is True:
        if resp < 4 and resp > 0:
            print(f"Device Response: {RESPOND_STATUS[resp]}")
        
    return resp

def stream_data_to_programmer(device, buffer):
    device.write(WRITE_COMMAND)
    get_device_response(device, True)

    for i in range(0, len(buffer)):
        bin_str = byte_to_str(buffer[i])
        device.write(bin_str.encode())
        print(get_device_response(device))

def stream_data_from_programmer(device, byte_size=0):
    device.write(READ_COMMAND)
    get_device_response(device, True)

    for i in range(0, byte_size):
        device.write(READ_BYTE)
        print(get_device_response(device))

def main():  
    arduino = open_serial_port(sys.argv[1])
    bin_buffer = data_into_buffer(sys.argv[2])

    clear_screen()
    print("Holding for device initialisation...")
    time.sleep(2)

    stream_data_to_programmer(arduino, bin_buffer)
    #stream_data_from_programmer(arduino, 10)

    arduino.close()

main()