import subprocess
import time
import os
import sys

"""
python3 can_test.py [time_per_test][time_betw_cmds][time_betw_tests]

[num commands][device type][num devices]
[command 0]
.
.
.
[command n-1]
"""

script_file = sys.argv[1]
time_per_test = int(sys.argv[2])
time_betw_cmds = int(sys.argv[3])
time_betw_tests = int(sys.argv[4])

max_devices = int('3')
bitrate = sys.argv[5]

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
            subprocess.check_call(pre_linkup)
        elif device == 'can':
            linkup[4] = device + str(i)
        subprocess.check_call(linkup)

def run(cmds, num_cmds):
    
    cur_subprc = [None] * num_cmds 

    for cur,i in zip(cmds,range(num_cmds)):
       cur_subprc[i] = subprocess.Popen(cur)
       time.sleep(time_betw_cmds)

    time.sleep(time_per_test)

    for i in range(num_cmds):
       cur_subprc[i].terminate() 

def takedown(device, num_devices):

    takedown = ["sudo", "ip", "link", "set", "", "down"]
    post_takedown = ["sudo", "ip", "link", "delete", ""]   
 
    for i in range(num_devices):
        takedown[4] = device + str(i)
        subprocess.check_call(takedown)
        if device == 'vcan':
            post_takedown[4] = device + str(i)
            subprocess.check_call(post_takedown)

def do_cmds(cmds,index,num_cmds):

    device = cmds[index][1]
    num_devices = int(cmds[index][2])

    if int(num_devices) > max_devices or device not in ['vcan','can']:
        sys.exit('invalid input')  
    
  
    setup(device, num_devices)
    run(cmds[index+1:index+1+num_cmds], num_cmds) 
    takedown(device, num_devices)


if __name__ == '__main__':

    with open(script_file, 'r') as file_in:
        cmds = [i.split(' ') for i in file_in.read().split('\n') if i != '']
    
    for num, index in zip(cmds,range(0,len(cmds))):
        if num[0].isdigit():
            do_cmds(cmds,index,int(num[0]))
            time.sleep(time_betw_tests)
