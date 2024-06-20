#!/bin/sh

USR=aluno
VDIR=/var/data/${USR}
IMAGE=hmarcondes/dev_space:latest
REPO=${VDIR}/emb22109/emb22109
export REPO

function pod_start () {
	podman run -it --rm --security-opt label=disable \
		--userns=keep-id:uid=1000,gid=1000 \
		--volume ${VDIR}:/home/${USR} \
	docker.io/${IMAGE}
}

function setImage () {
	IMAGE=$1
}

function update_rootfs () {
	if [ -e rootfs.tar ]; then
	   OPTS="-xvf rootfs.tar" 
	elif [ -e rootfs.tar.gz ]; then
	   OPTS="-zxvf rootfs.tar.gz"
	elif [ -e rootfs.tgz ]; then
	   OPTS="-zxvf rootfs.tgz"
	else 
	  echo "rootfs package (.tar, .tar.gz or .tgz) not found !"
	  return 1
	fi

	DEV=$(blkid -o device --label rootfs)
	if [ $? == 0 ]; then
		echo "Found rootfs on -> $DEV"
		LABEL=$(blkid -o value -s LABEL $DEV)
		umount $DEV
		mkdir .tmp
		fuse2fs -o fakeroot $DEV .tmp
		if [ "$1" == "clean" ]; then
		    echo "Cleaning rootfs ..."
		    rm -r .tmp/*
		fi
		echo "Updating rootfs ..."
		tar -C .tmp/ --same-owner $OPTS
		umount $DEV
		rmdir .tmp
		echo "done!"
	else
		echo "rootfs partition not found !"
	fi
}
