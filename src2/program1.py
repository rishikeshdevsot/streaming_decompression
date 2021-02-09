import tarfile
import io
import sys


# Read input data.
input_tar = tarfile.open(fileobj=sys.stdin, mode='r|*')
output_tar = tarfile.open(fileobj=sys.stdout, mode='w|')
tarinfo = input_tar.next()
while(tarinfo != None):
    fileobj = input_tar.extractfile(tarinfo)

    # Write output file.
    output_tar.addfile(tarinfo, fileobj)
    tarinfo = input_tar.next()

input_tar.close()
output_tar.close()
