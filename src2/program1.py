#!/usr/bin/python

# tarfile doesn't support symlink extraction https://duplicity.readthedocs.io/en/latest/_modules/tarfile.html ("cannot extract (sym)link as file object")
# https://bugs.python.org/issue35483
# https://stackoverflow.com/questions/10060069/safely-extract-zip-or-tar-using-python
# (cannot use support symlinks)

import tarfile
import io
import sys


# Read input data.
input_tar = tarfile.open(fileobj=sys.stdin, mode='r|*')
output_tar = tarfile.open(fileobj=sys.stdout, mode='w|')

tarinfo = input_tar.next()
while(tarinfo != None):
    if(not tarinfo.issym()):
        fileobj = input_tar.extractfile(tarinfo)
        # Write output file.
        output_tar.addfile(tarinfo, fileobj)
    tarinfo = input_tar.next()

sys.stdout.flush()
input_tar.close()
output_tar.close()
