import serial, time

ser = serial.Serial("/dev/tty.usbserial-14330", 9600, timeout=1)
with open("mq135values.txt", "r") as f:
    while True:
        f.writelines(ser.readline())
        time.sleep(1)
