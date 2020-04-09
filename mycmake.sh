#!/bin/bash
set -x
cmake -D CMAKE_INSTALL_PREFIX=/usr/local/caffeDebug/ -D python_version=3 -D CMAKE_BUILD_TYPE=Debug ..
