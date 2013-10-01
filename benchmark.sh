#!/usr/bin/env bash

set -u
set -e

test -x ./bot_bot || exit 1

game=othello
log=log_${game}.txt

rm -f $log
for p1time in $(LANG="C" seq .1 .2 .9); do
for p2time in $(LANG="C" seq .1 .2 .9); do
	echo "p1time=$p1time p2time=$p2time" | tee -a $log
	for n in $(seq 1 50); do
		./bot_bot $game $p1time $p2time >> $log
		echo -n "-"
	done
	echo
done
done

