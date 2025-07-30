#!/bin/bash

set -e

sudo apt-get install clang-18
curl -1sLf 'https://dl.cloudsmith.io/public/mull-project/mull-stable/setup.deb.sh' | sudo -E bash
sudo apt-get update
sudo apt-get install mull-18
mull-runner-18 --version
export CXX=clang++-18
export CC=clang-18
