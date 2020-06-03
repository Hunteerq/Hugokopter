import struct

import cv2
import socket
import time
import io
from PIL import Image
import numpy as np

serverAddressPort = ('', 8081)
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.bind(serverAddressPort)
time_start = time.time()
client_socket.listen(1)
print("Listen for connection...")
connection, address = client_socket.accept()
print("Connection accepted")
img = Image.open(r"zdj.jpeg")
b = io.BytesIO()
sendData = io.BytesIO()
img.save(b, "jpeg")
sendData.write(struct.pack(">i", b.getbuffer().nbytes))
sendData.write(b.getvalue())
print(b.getbuffer().nbytes)
print(sendData.getbuffer().nbytes)
connection.sendall(sendData.getvalue())
#connection.send(b"----frame")
time.sleep(20)
connection.close()
client_socket.close()