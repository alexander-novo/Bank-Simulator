#!/bin/sh

file=1
while [ $file -lt 6 ]; do
        ./banksim events_$((file)).txt 1 1
        read -r -p "Press enter to continue..." key
        ./banksim events_$((file)).txt 3 3
        read -r -p "Press enter to continue..." key
        ./banksim events_$((file)).txt 3 1
        read -r -p "Press enter to continue..." key

        file=$((file + 1))
done
