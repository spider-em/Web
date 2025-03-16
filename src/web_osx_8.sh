#!/bin/sh -f

# See if X -depth 8   is currently running and use its display
DPY8=`ps -eo command | grep 'bin/X' | grep '\-depth 8' | grep -v grep | grep -v xinit | cut -f 2 -d " "`
#echo DPY8: $DPY8

if [ $DPY8 ] ; then
  echo Attaching to current 8 bit DISPLAY: $DPY8 
  export DISPLAY=$DPY8
else
  export DISPLAY=:99
  echo Starting : quartz-wm -- on New: X $DISPLAY -depth 8 
  xinit  /usr/bin/quartz-wm  -- /usr/X11/bin/X $DISPLAY -depth 8 2>/dev/null & 
  sleep 2
fi

# Start web
echo "Set your DISPLAY Env. Variable to: $DISPLAY"
echo "Calling: web_osx_8"
web_osx_8 $1 & 
exit
