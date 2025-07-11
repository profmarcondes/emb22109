#!/bin/sh

podman build --squash --tag emb22109-toolbox:latest Container/
toolbox create -i emb22109-toolbox:latest emb22109
