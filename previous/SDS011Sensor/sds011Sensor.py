import os
import simple_sds011

    

def query_sds011():
    port = os.system("ls /dev/ttyUSB*")
    port = "/dev/ttyUSB" + str(port)
    print("port:", port)
    pm = simple_sds011.SDS011(port)
    pm.mode = simple_sds011.MODE_PASSIVE
    print(pm.period)
    
    
    sds011_results = pm.query()['value']
    print(sds011_results)
    

