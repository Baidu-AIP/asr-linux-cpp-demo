#!/bin/sh
valgrind --gen-suppressions=all --leak-check=full --show-reachable=yes --suppressions=../scripts/srt.supp ../run/asrDemo_srt

