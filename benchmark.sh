#!/usr/bin/env bash

set -u

test -x ./bot_bot || exit 1

log=log_blocks.txt

rm -f $log
for p1time in $(LANG="C" seq .1 .2 .9); do
for p2time in $(LANG="C" seq .1 .2 .9); do
	echo "p1time=$p1time p2time=$p2time" | tee -a $log
	for n in $(seq 1 10); do
		./bot_bot blocks $p1time $p2time $k >> $log
		echo -n "-"
	done
	echo
done
done

