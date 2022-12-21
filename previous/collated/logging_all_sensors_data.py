import os
import simple_sds011
import bmpsensor
import board
import adafruit_bh1750
from time import gmtime, strftime
import time
import serial

   
   
#-------------------------------------------   
def query_sds011():
	port = os.system("ls /dev/ttyUSB*")
	port = "/dev/ttyUSB" + str(port)
	#print("port:", port)    
	pm = simple_sds011.SDS011(port)

	pm.mode = simple_sds011.MODE_PASSIVE
	#print(pm.period)
	sds011_results = pm.query()['value']
	print(sds011_results)
	return sds011_results
    
def query_bmp180():
	temp, pressure, altitude = bmpsensor.readBmp180()
	bmp180_results = {"temp": temp, "pressure": pressure, "altitude": altitude}
	print(bmp180_results)
	return bmp180_results

def query_bh1750():
	i2c = board.I2C()
	bh1750 = adafruit_bh1750.BH1750(i2c)
	bh1750_results = {"lux":bh1750.lux}
	print(bh1750_results)
	return bh1750_results

def query_mq7_mq135():
    ser = serial.Serial("/dev/ttyUSB2", 9600, timeout=1)
    value = str(ser.readline())[2:8]
    print(value)
    return value
    
    



while True:
    timestamp = strftime("%Y-%m-%d, %H:%M:%S", gmtime())
    sds011_results = query_sds011()
    #bmp180_results = query_bmp180()
    #bh1750_results = query_bh1750()
    #mq7_mq135_results = query_mq7_mq135()
    f = open("stored_data.txt", 'a')
    f.write(f"{timestamp},  {sds011_results['pm2.5']}, {sds011_results['pm10.0']}, {bmp180_results['temp']},{bmp180_results['pressure']}, {bmp180_results['altitude']}, {bh1750_results['lux']}, {mq7_mq135_results}\n")
    f.close()
    time.sleep(5)
