#!/bin/bash
set -x
cmake -D CMAKE_INSTALL_PREFIX=/usr/local/caffeDebugPython2 -D python_version=2 -D CMAKE_BUILD_TYPE=Debug ..
