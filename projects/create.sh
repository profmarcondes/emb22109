#!/bin/bash

if [ ! $1 ]
then
   echo "Usage: ./create.sh project_name [ defconfig ]"
   echo "-- [defconfig] is optional. Don't specify the "_defconfig" part of the name"
   echo "-- Example: ./create.sh myProject raspberrypi3"
   exit -1
fi

PROJECT_DIR="$(pwd)/$1"
CONFIG=clean

if [ $2 ]
then	
   CONFIG=$2
fi

if [ -d $(pwd)/$1 ]; then
    echo "ERROR: Project named $1 already exists !"
else
    echo "Creating project $1 ..."
    make -C ../buildroot/ BR2_EXTERNAL=../external O=../projects/$1 ${CONFIG}_defconfig
    if [ $? -eq 0 ]; then
	    echo "Project sucessfully created!"
    else
	    rm -rf $(pwd)/$1
    fi
fi
