#Resources
#https://docs.python.org/3/library/shutil.html#module-shutil
# encoding = ISO-8859-1

import gzip
import sys
import io
import shutil

import tarfile
import sys

tar = tarfile.open(fileobj=sys.stdin, mode='r|*')
tar.extractall()
tar.close()



#with gzip.open(sys.stdin, 'rb') as f:
#    file_content = gzip.decompress(f.read())
