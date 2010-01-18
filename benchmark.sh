#!/usr/bin/env bash

log=log_blocks.txt

rm -f $log
for p1time in $(seq .1 .1 1); do
for p2time in $(seq .1 .1 1); do
	echo "p1time=$p1time p2time=$p2time" | tee -a $log
	for n in $(seq 1 50); do
		./bot_bot blocks $p1time $p2time $k >> $log
		echo -n "-"
	done
	echo
done
done

