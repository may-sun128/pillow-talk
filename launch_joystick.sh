#!/usr/bin/bash

# This script launches qjoypad and the script to handle qjoypads output

# remove the `--notray` flag for program to run in background 
qjoypad --notray &
./keys.py