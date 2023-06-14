#!/bin/bash

if [ $# -eq 0 ]
  then
    echo "Usage: send_command.sh <clear | quit>"
    exit 1
fi

if [ $1 != "clear" ] && [ $1 != "quit" ]
  then
    echo "Usage: send_command.sh <clear | quit>"
    exit 1
fi

echo $1 | nc -N 127.0.0.1 4242