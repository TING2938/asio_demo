import socket
import struct
import matplotlib.pyplot as plt

sock = socket.socket()

sock.connect(('127.0.0.1', 1300))

plt.ion()

time, value = [], []

while True:
    data = sock.recv(16)
    data = struct.unpack("=qd", data)
    print(data)
    value.append(data[1])
    time.append(data[0])
    plt.plot(time, value, "--or")
    plt.pause(0.1)
