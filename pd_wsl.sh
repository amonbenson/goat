#!/bin/sh

# setup xserver
export DISPLAY=$(awk '/nameserver / {print $2; exit}' /etc/resolv.conf 2>/dev/null):0
export LIBGL_ALWAYS_INDIRECT=1

# start puredata
/usr/bin/pd -path $(pwd) -noaudio
