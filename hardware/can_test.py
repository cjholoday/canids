import subprocess
import time
import os
import sys

max_devices = int('3')
bitrate = '10000'

def setup(device, num_devices):

    if device == 'vcan':
        subprocess.call(["sudo","modprobe","vcan"])   
        pre_linkup = ["sudo","ip","link","add","dev", "","type","vcan"]
        linkup = ["sudo","ip","link","set","up",""]
   
    elif device == 'can':
        linkup = ["sudo", "ip", "link", "set", "", "up", "type", "can", "bitrate", bitrate] 
    
    for i in range(num_devices):
        if device == 'vcan':
            pre_linkup[5] = device + str(i)
            linkup[5] = device + str(i)
           #subprocess.call(pre_linkup)
        elif device == 'can':
            linkup[4] = device + str(i)
        subprocess.check_call(linkup)

def run(cmds, num_cmds):
    
    cur_subprc = [None] * num_cmds 

    for cur,i in zip(cmds,range(num_cmds)):
       cur_subprc[i] = subprocess.Popen(cur)

    time.sleep(5)

    for i in range(num_cmds):
       cur_subprc[i].terminate() 

def takedown(device, num_devices):

    takedown = ["sudo", "ip", "link", "set", "", "down"]
    
    for i in range(num_devices):
        takedown[4] = device + str(i)
        subprocess.check_call(takedown)

def do_cmds(cmds,index,num_cmds):

    device = cmds[index][1]
    num_devices = int(cmds[index][2])

    if int(num_devices) > max_devices or device not in ['vcan','can']:
        sys.exit('invalid input')  
    
  
    setup(device, num_devices)
    run(cmds[index+1:index+1+num_cmds], num_cmds) 
    takedown(device, num_devices)


if __name__ == '__main__':

    with open('test.txt', 'r') as file_in:
        cmds = [i.split(' ') for i in file_in.read().split('\n') if i != '']
    
    for num, index in zip(cmds,range(0,len(cmds))):
        if num[0].isdigit():
            do_cmds(cmds,index,int(num[0]))
        time.sleep(2)

"""
    #'0b10101010101' - 0x555 - minimum stuffing
    ID = "1365"
    length = "0"
    data = "0"
    bitrate = "500000"

    virtual = 0
    
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
        
        if(virtual==1):
            linkup[4] = "vcan0"
            subprocess.check_call(v_linkup[2])
        else:
            linkup[4] = "can0"
            subprocess.check_call(linkup)
            linkup[4] = "can1"
            subprocess.check_call(linkup)
            linkup[4] = "can2"
            subprocess.check_call(linkup)

        if(virtual==0):
            device = "can0"
        a = subprocess.Popen(["python3", "interframe_spacing_test.py", device, bitrate, "send", ID, length, data]) 
        if(virtual==0):
            device = "can1"
        b = subprocess.Popen(["python3", "interframe_spacing_test.py", device, bitrate, "send", ID, length, data]) 
        if(virtual==0):
            device = "can2"
        c = subprocess.Popen(["python3", "interframe_spacing_test.py", device, bitrate, "receive", ID, length, data]) 

        time.sleep(2)
    
        a.terminate()
        b.terminate()
        c.terminate()

        if(virtual==1):
            takedown[4] = "vcan0"
            subprocess.check_call(takedown)
        else:
            takedown[4] = "can0"
            subprocess.check_call(takedown)
            takedown[4] = "can1"
            subprocess.check_call(takedown)
            takedown[4] = "can2"
            subprocess.check_call(takedown)
        
        time.sleep(3)

    print("Finish")

"""
