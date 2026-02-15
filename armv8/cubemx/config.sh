#!/usr/bin/env bash

rm -r ./build
rm -r ./NonSecure/build/
rm -r ./Secure/build/
cmake . -B ./build/ -DCMAKE_TOOLCHAIN_FILE=./gcc-arm-none-eabi.cmake