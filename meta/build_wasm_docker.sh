#!/bin/bash

set -xe

ocen main.oc -n -c build/main.c
docker run --rm -v $(PWD):/mnt/ -w /mnt/ emscripten/emsdk ./meta/build_wasm.sh build/main.c
