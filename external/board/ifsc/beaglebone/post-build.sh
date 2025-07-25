#!/bin/sh

echo "Buildroot - $(git describe) - $(date)" > ${TARGET_DIR}/etc/build-id
