import os
import mgen
import math
import datetime
import socket
import time

import sys


starttime = sys.argv[1]
nodeid = int(sys.argv[2])
numnodes = int(sys.argv[3])

PKT_SIZE = 512
INIT_FLOW = 5

# i = 1
A_MAX = 100.0
EPSILON = 0.50
K_PARA = 200.0

curr_rates = {}
for i in range(1, numnodes+1):
    if i != nodeid:
        curr_rates[i] = INIT_FLOW

sender = mgen.Controller()
sender.send_command("output persist/{}/var/log/mgen.out".format(nodeid))
sender.send_command("start " + starttime)
sender.send_command("txlog")
print "start mgen"

# "Manually"  start a flow from this sender
# sender.send_event("on 1 udp dst 127.0.0.1/5000 per [1 1024]")

for ind, rate in curr_rates.iteritems():
    onevent="1.0 ON {} UDP SRC 5001 DST 10.100.0.{}/5001 PERIODIC [{} {}] INTERFACE bmf0".format(ind, ind, rate, PKT_SIZE)
    print onevent
    sender.send_event(onevent)

print (datetime.datetime.now())

start = int(round(time.time() * 1000))



fifo_path = "/tmp/emane-mgen_fifo_node{}".format(nodeid)
if os.path.exists(fifo_path):
    os.remove(fifo_path)
os.mkfifo(fifo_path)

# a forever loop until we interrupt it or
# an error occurs
while True:
    # Establish connection with client.
    with open(fifo_path, 'r') as fifo:
        data = fifo.readline()[:-1]
        print"Reader: ", data
    
    now = int(round(time.time() * 1000))
    if now - start < 50 * 1000:
        continue
    # i = i+1
    # if 50 < i:
    for weight in data.split(','):
        wid = int(weight.split(':')[0])
        w = float(weight.split(':')[1])
        # print "wid", wid
        # print "w", w
        if w == 0:
            rate = int(A_MAX)
        else: 
            rate = int(K_PARA/w - EPSILON)
        if rate < 0: 
            rate = 0
        elif rate > A_MAX:
            rate = int(A_MAX)

        mod_event =  "MOD {} UDP SRC 5001 DST 10.100.0.{}/5001 PERIODIC [{} {}] INTERFACE bmf0".format(wid, wid, rate, PKT_SIZE)
        print datetime.datetime.now(), mod_event
        print datetime.datetime.now(), data

        if rate != curr_rates[wid]:
            curr_rates[wid] = rate
            sender.send_event(mod_event)
