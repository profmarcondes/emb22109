#!/bin/sh

function die {
        echo "$*" >&2
        echo "See $0 -h"
        exit -1
}

function needs_arg {
        if [ -z "$OPTARG" ]; then
                die "No value for --$OPT (use --$OPT=value)"
        fi
}

function usage {
	echo "Usage: $0 <options>"
        echo "   Option                 : Description (type: default_value)"
        echo "--------------------------------------------------------------------------------"
        echo "   -d | --delay	: Delay between blinks in seconds (float: 0.1) "
	echo "   -p | --pin     : GPIO Pin do control (for leds, use 21-24) (int: 21)"
	echo "   -t | --times   : Number of blinks to execute (int: 10)"
        exit 0
}

DELAY=.1
PIN=21
TIMES=10

while getopts :hd:p:t:-: OPT; do

  # support long options: https://stackoverflow.com/a/28466267/519360
  if [ "$OPT" = "-" ]; then   #: long option: reformulate OPT and OPTARG
    OPT="${OPTARG%%=*}"       # extract long option name
    OPTARG="${OPTARG#$OPT}"   # extract long option argument (may be empty)
    OPTARG="${OPTARG#=}"      # if long option argument, remove assigning `=`
  fi

  case "$OPT" in
    d | delay)       
		needs_arg; 
		DELAY=$OPTARG
		;;
    
    p | pin )
		needs_arg; 
		PIN=$OPTARG 
		;;
		
    t | times )               
		needs_arg; 
		TIMES=$OPTARG 
		;;
		
    h | help)                               
		usage 
		;;
		
    ??* )
		die "Illegal option --$OPT" 
		;; # bad long option
    \? )                                        
		die "Illegal option" 
		;; # bad short option
  esac

done
shift $((OPTIND-1)) # remove parsed options and args from $@ list

if [ ! -e /sys/class/gpio/gpio${PIN}/ ]; then
	echo $PIN > /sys/class/gpio/export
	echo "out" > /sys/class/gpio/gpio$PIN/direction
fi

COUNTER=0
while [ $COUNTER -lt $TIMES ]; do
	echo 0 > /sys/class/gpio/gpio$PIN/value
	sleep $DELAY
	echo 1 > /sys/class/gpio/gpio$PIN/value
	let COUNTER=COUNTER+1
	sleep $DELAY
done

echo 0 > /sys/class/gpio/gpio${PIN}/value

