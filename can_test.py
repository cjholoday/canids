import subprocess
import time
import os
import sys
import click

@click.command()
@click.option('-s','--script_file', type=str)
@click.option('-t1', '--time_per_test', type=int, default=5) 
@click.option('-t2', '--time_betw_cmds', type=int, default=1)
@click.option('-t3', '--time_betw_tests', type=int, default=1) 
@click.option('-d', '--max_devices', type=int, default=1)
@click.option('-b', '--bitrate', type=str, default='500000') 

#script_file = sys.argv[1]
#time_per_test = int(sys.argv[2])
#time_betw_cmds = int(sys.argv[3])
#time_betw_tests = int(sys.argv[4])

#max_devices = int('3')
#bitrate = sys.argv[5]

def run_script(script_file, time_per_test, time_betw_cmds, time_betw_tests, max_devices, bitrate):

    """\nThis script allow for multiple can commands to be run simotaneously. The process will set up devices, start each command with a specified delay, run for a specified amount of time, take all devices down, and wait a specified amount of time before starting the next set of commands (or exit). It looks for an number at the first postion in a line in the input file, and runs that number of commands in the following lines. Placing any character before the number will cause the folling set of commands to be ignored. The input file should follow the format defined below. Further, bitrate can be specified if physical can devices are being used.    
\nInput File Format:
\n[num commands][device type][num devices]
\n[first command]
\n.
\n.
\n.
\n[last command]"""

    var = {'script_file':script_file,'time_per_test':time_per_test,'time_betw_cmds':time_betw_cmds,
        'time_betw_tests':time_betw_tests,'max_devices':max_devices,'bitrate':bitrate}

    with open(script_file, 'r') as file_in:
        cmds = [i.split(' ') for i in file_in.read().split('\n') if i != '']
    
    for num, index in zip(cmds,range(0,len(cmds))):
        if num[0].isdigit():
            do_cmds(cmds,index,int(num[0]),var)
            time.sleep(var["time_betw_tests"])  

def setup(device, num_devices, var):

    if device == 'vcan':
        subprocess.call(["sudo","modprobe","vcan"])   
        pre_linkup = ["sudo","ip","link","add","dev", "","type","vcan"]
        linkup = ["sudo","ip","link","set","up",""]
   
    elif device == 'can':
        linkup = ["sudo", "ip", "link", "set", "", "up", "type", "can", "bitrate", var["bitrate"]] 
    
    for i in range(num_devices):
        if device == 'vcan':
            pre_linkup[5] = device + str(i)
            linkup[5] = device + str(i)
            subprocess.check_call(pre_linkup)
        elif device == 'can':
            linkup[4] = device + str(i)
        subprocess.check_call(linkup)

def run(cmds, num_cmds, var):
    
    cur_subprc = [None] * num_cmds 

    for cur,i in zip(cmds,range(num_cmds)):
       cur_subprc[i] = subprocess.Popen(cur)
       time.sleep(var["time_betw_cmds"])

    time.sleep(var["time_per_test"])

    for i in range(num_cmds):
       cur_subprc[i].terminate() 

def takedown(device, num_devices, var):

    takedown = ["sudo", "ip", "link", "set", "", "down"]
    post_takedown = ["sudo", "ip", "link", "delete", ""]   
 
    for i in range(num_devices):
        takedown[4] = device + str(i)
        subprocess.check_call(takedown)
        if device == 'vcan':
            post_takedown[4] = device + str(i)
            subprocess.check_call(post_takedown)

def do_cmds(cmds,index, num_cmds, var):

    device = cmds[index][1]
    num_devices = int(cmds[index][2])

    if int(num_devices) > var["max_devices"] or device not in ['vcan','can']:
        sys.exit('invalid input')  
    
  
    setup(device, num_devices, var)
    run(cmds[index+1:index+1+num_cmds], num_cmds, var) 
    takedown(device, num_devices, var)


if __name__ == '__main__':
    run_script()
