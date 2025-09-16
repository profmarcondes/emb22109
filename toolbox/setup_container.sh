#!/bin/sh

NAME=dev

podman build --squash --tag $NAME-toolbox:latest Container/
toolbox create -i $NAME-toolbox:latest $NAME
