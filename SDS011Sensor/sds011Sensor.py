import os
from sds011 import SDS011
import time

port = os.system("ls /dev/ttyUSB*")
port = "/dev/ttyUSB" + str(port)

sds = SDS011(port=port)
print(sds)

sds.set_working_period(rate=5)

meas = sds.read_measurement()

print(meas)


