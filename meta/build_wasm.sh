#!/bin/bash

set -xe

mkdir -p build

if [[ -z "$1" ]]; then
    ocen main.oc -n -c build/main.c
    FILE=build/main.c
else
    FILE=$1
fi


cp assets/index.html build/

# GUIDE for flags:
#------------------
#   -O3:                        Optimize the code.
#   -DOC_NO_BACKTRACE:          Disable backtrace: Ocen specific, required, since WASM doesn't have execinfo.h
#   -s USE_SDL=2:               Use SDL2 for graphics. (`std::og` is based on SDL2)
#   -s USE_ZLIB=1:              Use zlib for compression.  (`std::image::png` uses zlib)
#   -s ALLOW_MEMORY_GROWTH=1:   Allow memory to grow dynamically.
#   --preload-file assets:      Bundle the `assets` directory into generated WASM data
#   --use-preload-plugins:      Use the plugins in the preload file.
#   -o build/index.js:          Output file name (JS + corresponding WASM file)
#   -s ASYNCIFY=1:              Enable asyncify (allows `while` loops in the code for main event loop)
#
#   Optional flags (not used here, but needed for multi-wasm pages)
#   ----------------------------------------------------------------
#   -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=0:     Without this, SDL always assumes single canvas with ID `#canvas`.
#   -s MODULARIZE=1:                                        Generate a module that can be loaded into the same HTML page.
#   -s EXPORT_NAME="'Module1'":                             Name of the module. This is used in the HTML file to load the module.
#   -s 'EXTRA_EXPORTED_RUNTIME_METHODS=["ENV"]':            Export the ENV object to the module. This is needed so we can configure
#                                                           the module to not capture keyboard events outside the canvas. Look at
#                                                           `assets/multi-canvas.html` or `assets/index.html` for more details.


emcc $FILE \
    -O3 \
    -DOC_NO_BACKTRACE \
    -s USE_SDL=2 \
    -s USE_ZLIB=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    --preload-file assets \
    --use-preload-plugins \
    -o build/index.js \
    -s ASYNCIFY=1