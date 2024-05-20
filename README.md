# Ocen WASM Template

This is a template using the `std::og` library in [ocen](https://github.com/ocen-lang/ocen)
to draw some graphics to the screen. `std::og` is a set of wrappers around `SDL` to make it
a friendlier API for fast-protoyping (loosely inspired by `p5.js` / `raylib`).

This repository demonstrates how to compile such an application to WebAssembly using `emscripten`,
and has a sample github workflow to automatically build and deploy the application to github pages.

## Customization

- The `assets` folder is automatically bundled into the generated `wasm` (can be changed in `build_wasm.sh`).
  Any files you want to be available to the application should be placed in this folder.

- `assets/index.html` is the default HTML template for the application. It only works with the default emscriptem
    build, and assumes the files generated are `index.js` and `index.wasm`.

- `assets/multi-canvas.html` is a demonstration of how to load in different compiled WASM modules into the same
    page. This requires building the WASM modules with different names (`-s MODULARIZE=1 -s EXPORT_NAME="FOO"` flags).
    This is demonstrated in `./meta/multi_build_wasm.sh`.

## Building

### Native SDL build

Need to have `SDL2` installed on your system, and should be able to compile C code with `-lSDL2 -lz`

```shell
$ ocen main.oc -o main
$ ./main
```

### Emscripten build, Local toolchain

Need to have `emscripten` installed locally, and activated so that `emcc` is in your path.

```shell
$ ./meta/build_wasm.sh
$ python3 -m http.server build/
```

### Emscripten build, Docker

Must have docker installed on your system, and running.

```shell
$ ./meta/build_wasm_docker.sh
$ python3 -m http.server build/
```