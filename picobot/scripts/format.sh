#!/bin/sh
meson format -i meson.build
clang-format -i src/main.cpp
