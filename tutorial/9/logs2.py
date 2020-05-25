import matplotlib.pyplot as plt
import matplotlib.dates
import time
import datetime


def find_substring(substring, string): #returns a list of all indices at which the substring is found in the string
    indices = []
    index = -1 
    while True:
        index = string.find(substring, index + 1)
        if index == -1:  
            break
        indices.append(index)
    return indices


def gettime(string, index): #index is index at which 'Queue 2 has size' begins
    temphr = strfile[index - 74: index - 58]
    temphr1 = temphr.split(".")
    temptime = temphr1[0].split(':')

    time = 3600*int(temptime[0]) + 60*int(temptime[1]) + float(temptime[2]) + int(temphr1[1]) * 0.000001
    # t = datetime.time(int(temptime[0]), int(temptime[1]), int(temptime[2]), int(temphr1[1]))
    return time


file = open("persist/1/var/log/emane.log","r")

start = 0
strfile = file.read()

indlist = find_substring(" queuelength: ", strfile)  # Replace the substring with whichever queue size you require

queuesizelist = []  # Y-axis labels
xlist = []   # X-axis labels

for i in range(0,len(indlist)-3,3):
    currsize = 0
    for j in range(3):
    	temp = strfile[indlist[i+j]+14: indlist[i+j]+20] # substring
    	temp1 = temp.split(',')   # list
        print strfile[indlist[i+j]-20: indlist[i+j]+20], '====', temp1[0]
        currsize += int(temp1[0])
    queuesizelist.append(currsize)
    ntime = gettime(strfile, indlist[i])
    if start == 0:
        start = ntime
    xlist.append(ntime-start)

# gettime(strfile, indlist[0])

# x_dt = [datetime.datetime.combine(datetime.date.today(), t) for t in xlist]
# dates = matplotlib.dates.date2num(x_dt)
plt.plot(xlist, queuesizelist)
plt.show()

file.close()
