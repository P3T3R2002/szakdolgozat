from serial.tools import list_ports
import (
    csv,
    time,
    serial
)

ports = list_ports.comports()
for port in ports:
    print(port)