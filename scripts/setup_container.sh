#!/bin/sh

podman build --squash --tag locahost/emb22109-toolbox:latest Container/
toolbox create -i locahost/emb22109-toolbox:latest emb22109
