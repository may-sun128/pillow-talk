#!/usr/bin/bash

# This script launches qjoypad and the script to handle qjoypads output

# remove the `--notray` flag for program to run in background 
#qjoypad --notray &

# Kill previous instance of qjoypad is it's already running
pkill -f qjoypad
sleep 3
echo launched
qjoypad & 
./pillowtalk.py
