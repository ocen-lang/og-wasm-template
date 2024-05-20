#!/bin/bash

# NOTE: This is a demo on how to build separate files into modules which can be loaded
#       into the **same** HTML page. Note the usage of `-s MODULARIZE` and `-s EXPORT_NAME`
#       flags.
#       Look at `assets/multi-canvas.html` for how to load multiple modules into the same
#       HTML page.

set -xe

mkdir -p build

cp assets/multi-canvas.html build/


ocen main.oc -n -c build/main.c

# NOTE: This is just to make the second file different from the first one.
cat main.oc | sed 's/BLACK/RED/g' > build/main2.oc
ocen build/main2.oc -n -c build/main2.c
rm build/main2.oc

#   Optional flags (needed for multi-wasm pages)
#   ----------------------------------------------------------------
#   -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=0:     Without this, SDL always assumes single canvas with ID `#canvas`.
#   -s MODULARIZE=1:                                        Generate a module that can be loaded into the same HTML page.
#   -s EXPORT_NAME="'Module1'":                             Name of the module. This is used in the HTML file to load the module.
#   -s 'EXTRA_EXPORTED_RUNTIME_METHODS=["ENV"]':            Export the ENV object to the module. This is needed so we can configure
#                                                           the module to not capture keyboard events outside the canvas. Look at
#                                                           `assets/multi-canvas.html` or `assets/index.html` for more details.
#                                                           NOTE: ENV is exported by default when NOT using MODULARIZE.

emcc build/main.c \
    -DOC_NO_BACKTRACE \
    -s USE_SDL=2 \
    -s USE_ZLIB=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    --preload-file assets \
    --use-preload-plugins \
    -s ASYNCIFY=1 \
    -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=0 \
    -s MODULARIZE=1 \
    -s EXPORT_NAME=Module1 \
    -o build/mod1.js \
    -s 'EXTRA_EXPORTED_RUNTIME_METHODS=["ENV"]'

emcc build/main2.c \
    -DOC_NO_BACKTRACE \
    -s USE_SDL=2 \
    -s USE_ZLIB=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    --preload-file assets \
    --use-preload-plugins \
    -s ASYNCIFY=1 \
    -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=0 \
    -s MODULARIZE=1 \
    -s EXPORT_NAME=Module2 \
    -o build/mod2.js \
    -s 'EXTRA_EXPORTED_RUNTIME_METHODS=["ENV"]'