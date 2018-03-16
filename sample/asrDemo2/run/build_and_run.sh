#!/bin/bash
rm asrDemo core core.*
make clean && make && echo "build success, wait 3s to run" && sleep 3 && ./asrDemo

