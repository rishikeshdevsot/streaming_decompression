#!/usr/bin/python

import tarfile
import io
import sys

# Read the input uncompressed data stream and extract all
input_tar = tarfile.open(fileobj=sys.stdin, mode='r|*')
input_tar.extractall()
input_tar.close()
