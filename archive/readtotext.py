import serial, time

ser = serial.Serial("/dev/ttyUSB1", 9600, timeout=1)
#with open("mq135values.txt", "a") as f:
#    while True:
#        value = ser.readline()
#        print(value)
#        f.write(f'{value}\n')
#        time.sleep(10)

while True:
    f = open("mq135values.txt", "a")
    value = str(ser.readline())[2:8]
    print(value)
    f.write(f'{value}\n')
    f.close()
    time.sleep(5)
