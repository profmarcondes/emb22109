#!/bin/sh

HOSTSPAWN="flatpak-spawn --directory=/tmp --host "

if [ -e rootfs.tar ]; then
	OPTS="-xvf rootfs.tar" 
elif [ -e rootfs.tar.gz ]; then
	OPTS="-zxvf rootfs.tar.gz"
elif [ -e rootfs.tgz ]; then
	OPTS="-zxvf rootfs.tgz"
else 
	echo "rootfs package (.tar, .tar.gz or .tgz) not found !"
	exit 1
fi

#LABEL=$(blkid -o value -s LABEL $DEV)
DEV=$(blkid -o device --label rootfs)
if [ $? == 0 ]; then
	echo "Found rootfs on -> $DEV"

	findmnt --source $DEV >> /dev/null
	if [ "$?" == "0" ]; then
		$HOSTSPAWN umount $DEV
		if [ "$?" == "1" ]; then
			echo "Device busy! Close all open windows within path"
			exit 1
		fi
	fi
	
	if [ ! -d .tmp ]; then
		mkdir .tmp 
	else
		echo "Something nasty happened, please remove .tmp directory and try again!"
		exit 1
	fi
	
	fuse2fs -o fakeroot $DEV .tmp
	if [ "$?" == "1" ]; then
		echo "Error trying to mount $DEV, gracefully exiting..."
		rmdir .tmp
		exit 1
	fi

	if [ "$1" == "clean" ]; then
		echo "Cleaning rootfs ..."
		rm -r .tmp/*
	fi

	echo "Updating rootfs ..."
	tar -C .tmp/ --same-owner $OPTS
	umount $DEV
	rmdir .tmp
	echo "done!"

	exit 0
else
	echo "rootfs partition not found !"
	exit 1
fi
