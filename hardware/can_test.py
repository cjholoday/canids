import subprocess
import time
import os

#sudo modprobe can && sudo modprobe vcan && sudo ip link add dev vcan0 type 

if __name__ == '__main__':
    #'0b10101010101' - 0x555 - minimum stuffing
    ID = "1365"
    length = "0"
    data = "0"
    bitrate = "500000"

    virtual = 1
    
    device = "vcan0"

    v_linkup =[["sudo","modprobe","vcan"],["sudo","ip","link","add","dev",device,"type","vcan"],["sudo","ip","link","set","up","vcan0"]] 
    linkup = ["sudo", "ip", "link", "set", device, "up", "type", "can", "bitrate", bitrate] 
    takedown = ["sudo", "ip", "link", "set", device, "down"]
    
    #linkup[4] = "can1"
    #takedown[4] = "can1"
    subprocess.call(v_linkup[0] + [' && '] + v_linkup[1])
    for i in range(9):
        
        ID = str(i)
        length = str(i)
        data = str(i)
        bitrate = str(10000 * (i+1))    
        
        if(virtual):
            device = "vcan0"
            subprocess.check_call(v_linkup[2])
        else:
            device = "can0"
            subprocess.check_call(linkup)
            device = "can1"
            subprocess.check_call(linkup)

        a = subprocess.Popen(["python3", "interframe_spacing_test.py", device, bitrate, "send", ID, length, data]) 
        b = subprocess.Popen(["python3", "interframe_spacing_test.py", device, bitrate, "receive", ID, length, data]) 

        time.sleep(2)
    
        a.terminate()
        b.terminate()

        if(virtual):
            device = "vcan0"
            subprocess.check_call(takedown)
        else:
            device = "can0"
            subprocess.check_call(linkup)
            device = "can1"
            subprocess.check_call(linkup)
        
        time.sleep(3)

    print("Finish")