#!/bin/bash
builddir=$(dirname "$(readlink -f $0)")/build
mkdir -p "$builddir"
cd "$builddir"
cmake -G "Eclipse CDT4 - Unix Makefiles" ../src/
