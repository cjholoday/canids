import log_utils
import matplotlib.pyplot as plt
import math
import sys

NUMIDS = 2048
BYTE = 8
COLORS = ['b','0.5','r','c','m','y','k','g']
MARKER = ['.',',','o','v','8','p','^','>']

#TODO: update to take on multiple message data lengths, save figures to a folder
#every time this is run, more command line options w/guide, especially with
#input log folder, and output specific IDS to new log files

if __name__ == '__main__':

    logfile = sys.argv[1]

    times = [[] for i in range(NUMIDS)]
    ids = [[] for i in range(NUMIDS)]
    databytes = [[[] for i in range(BYTE)] for i in range(NUMIDS)]
    validIDs = []

    messages = log_utils.return_message_list(logfile)

    for temp in messages[:]:
        tempID = temp.can_id
        if not(tempID in validIDs):
            validIDs.append(tempID)
        times[tempID].append(temp.time)
        ids[tempID].append(temp.can_id)
        for i in range(BYTE):
            databytes[tempID][i].append(temp.data[i])
            
    print(validIDs)

    #set plt font size
    plt.rcParams['font.size'] = 6

    for i in range(len(validIDs)):
        #temporary to only show ID: 0x0c8
        if validIDs[i] != 200:
            continue
        fig, axes = plt.subplots(ncols=4,nrows=3)
        fig.suptitle('ID: ' + str(validIDs[i]))
        for j in range(BYTE):
            axes[int(j/4),int(j%4)].set_title('BYTE: ' + str(j))
            axes[int(j/4),int(j%4)].scatter(times[validIDs[i]], databytes[validIDs[i]][j], marker='.', color=COLORS[j])
        for j in range(0,8,2):
            axes[2,int(j/2)].set_title('BYTES: ' + str(j) + '-' + str(j+1))
            temp1 = databytes[validIDs[i]][j]
            temp2 = databytes[validIDs[i]][j+1]
            axes[2,int(j/2)].scatter(times[validIDs[i]], [(temp1*256)+temp2 for temp1,temp2 in zip(temp1,temp2)], marker='.', color=COLORS[j])
        mng = plt.get_current_fig_manager()
        mng.resize(*mng.window.maxsize())
        plt.show()
