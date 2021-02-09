import tarfile
import io
import sys


input_tar = tarfile.open(fileobj=sys.stdin, mode='r|*')
input_tar.extractall()
input_tar.close()
