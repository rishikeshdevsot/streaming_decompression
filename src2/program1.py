#!/usr/bin/python


import tarfile
import io
import sys


# Setup tarfile objects
input_tar = tarfile.open(fileobj=sys.stdin, mode='r|*')
output_tar = tarfile.open(fileobj=sys.stdout, mode='w|')

tarinfo = input_tar.next()
# Read unitl no more
while(tarinfo != None):
    # Skip over symlinks both soft and hard
    if(not tarinfo.issym() and not tarinfo.islnk()):
        fileobj = input_tar.extractfile(tarinfo)
        # Write output file.
        output_tar.addfile(tarinfo, fileobj)
    tarinfo = input_tar.next()

# Flush the stdout pipe before closing output_tar
sys.stdout.flush()
input_tar.close()
output_tar.close()
