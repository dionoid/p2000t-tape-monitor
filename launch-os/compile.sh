#!/bin/bash

winpty docker run -v `pwd | sed 's/\//\/\//g'`://src/ -it z88dk/z88dk make

# calculate checksums
python checksums.py
