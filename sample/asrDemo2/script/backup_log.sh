#!/bin/bash
# asrdemo.log超过大小后，自动重命名为asrdemo.log.bak。本脚本自动重命名asrdemo.log.bak。
# crontab运行的间隔根据实际测试结果调整。注意不要两次运行的间隔过长，导致asrdemo.log.bak获取不到。
# 按照open_log_file第二个参数，即
LOG_NAME="asrdemo.log.bak"
base_path=$(cd `dirname $0`; pwd)
run_path="${base_path}/../run"
now=$(date "+%Y%m%d_%H%M%S")
file_name="${run_path}/${LOG_NAME}"
if [ -f "${file_name}" ]; then
  set -x
  mv "${file_name}"  "${file_name}.$now" 
else
  echo skip;
fi
