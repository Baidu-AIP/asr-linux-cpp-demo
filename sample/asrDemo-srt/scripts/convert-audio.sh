#!/bin/sh
EXT=$1

#ffmpeg -y  -i 16k.wav  -acodec pcm_s16le -f s16le -ac 1 -ar 16000 16k.pcm
cd .. && \
cd data/video && \
for file in $(ls *.$EXT);
do
   name=`echo $file|cut -d . -f 1`
   ffmpeg -y  -i $file  -acodec pcm_s16le -f s16le -ac 1 -ar 16000 $name.pcm
done
