#!/usr/bin/env python

from pylab import *
import scipy as ss
from sys import argv
import re

configre = re.compile(r'''^p1time=(\d+\.\d+) p2time=(\d+\.\d+)$''')
winningre = re.compile(r'''^winner: bot (\d) .+$''')

fh = open(argv[1],"r")
results = {}
current_key = None

for line in fh:
    match = configre.match(line)
    if match:
        key = tuple(map(lambda x: int(10.*float(x)),match.groups()))
        assert(key not in results)
        results[key] = []
        current_key = key

    match = winningre.match(line)
    if match:
        assert(current_key is not None)
        results[current_key].append(int(match.groups()[0])-1)

fh.close()
for key,value in results.items():
    print key, float(sum(value))/len(value), len(value)

aa = ss.arange(1,10,dtype=ss.integer)

def proba(p1time=None,p2time=None):
    bb = ss.zeros(aa.shape)
    cc = ss.zeros(aa.shape)
    for k,time in enumerate(aa):
        if p1time is not None:
            key = (p1time,time)
        if p2time is not None:
            key = (time,p2time)
        try:
            res = results[key]
            bb[k] = float(sum(res))/len(res)
            cc[k] = len(res)
        except KeyError:
            print "can't find key %s" % repr(key)
    return bb,cc

figure(1)
title("p2 win probability")
figure(2)
title("realisation count")

for time in aa:
    bb,cc = proba(p1time=time)
    figure(1)
    plot(aa,bb,label="p1time=%d" % time)
    figure(2)
    plot(aa,cc,label="p1time=%d" % time)

figure(1)
legend()
figure(2)
legend()

show()
