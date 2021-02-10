# sample_compression

## Versions
1. Version1: Using C++
2. Version2: Using python2


## Version 1: C++
### Dependencies
1. libarchive-dev
2. openmp
3. g++

### Compile Instructions
1. `cd src`
2. `make` \
will output binaries `program_1` and `program_2`

### Runnning the decompressor
`cat <archive-location>.tar.gz | ./program_1 | ./program_2`

### Architecture
program_1 creates a struct archive object `ina` which access the `stdin` stream using the `archive_read_open_fd()` function by passing in 0(fd for stdin is 0) as the fd. Another archive object `outa` accesses the `stdout` stream by passing in `fd = 1`. The ina reads each archive entry in 256 byte chunks using the `archive_read_data()` function and outa writes this data in uncompressed form to `stdout`. program_2 then reads this uncompressed data and writes to disk using the `archive_read_extract()` function. 

### Testing
#### Edge cases : result
1. Empty archives : extracts to an empty folder
2. Archives with archives: Keeps the inner arhives i.e. doesn't perform recursive decompression
3. Archives with symlinks: Extracts the symlinks which still contain the same data(for both hard and soft links)
4. Archives with named pipes/fifos: Extracts the fifo without problems
5. No input/invalid input to program_1: program_1 would detect this, give error `Couldn't open archive fd to stdin` and kill itself, program_2 follows suit
6. Archives with different formats(zip, tar, etc): libarchive supports many compression formats: [found here](https://www.freebsd.org/cgi/man.cgi?query=archive_read_support_format_all&apropos=0&sektion=3&manpath=FreeBSD+12.2-RELEASE+and+Ports&arch=default&format=html )
#### Verification
Verification was perfomed using the `diff -ur <dir1> <dir2>` bash command which recursively compares whether two directories and their corresponding files are the same.

### Optimization
#### Benchmarks used
1. Sparse directory archive: Used the gnome [Papirus](https://www.gnome-look.org/s/Gnome/p/1166289) full-icon theme which contains the icons for each type of gnome application for many different sizes and different themes. As such it is very sparse and has several files with deep directory entries.
2. Large file arhive: Used `/dev/random` to generate a 1GB file with random data and archived it. 

#### Benchmark results
The main parameter that allowed for optimization is the size of the character buffer to copy data from `ina` to `outa`. If it is kept too small then it uses less memory but takes a long time to copy each entry, if it is too large it copies quickly but uses a large block of memory. After testing with several values 256 byte buffer seemed to give the best results for both benchmarks. Interestingly when using a buffer size which is too large(around 8192 bytes) was used, the copying speed degrades again, a possible cause could be the time taken to write the buffer to the stdout stream. 

### Previous Implementations
#### Blocking Implementation
The initial implementation was performed by reading from the `stdin` using `std::cin` with `getlines()`. Storing this input into a character buffer and passing it to `archive_read_open_memory()` function. This method was functional but time consuming. The input stream was blocked until entirely read and only then did decompression start. Upon realizing that `stdin` and `stdout` can be represented with fd 0 and fd 1. I switched the implementation to use `archive_read_open_fd()` which allowed streaming reads and writes giving better performance. The code structure was also neater when using this function

#### Multi-threaded implementation
This is a failed attempt at multi-threading "the decompression + writing to `stdout` process" using OpenMP. In this implementation, threads start performing reads and writes to different archive entries in parallel. The pathnames of the entries which were already processed were added to a vector `addedEntries` which is checked before a new entry is processed to avoid conflicts. The implementation was working for an archive with few entries but upon testing it with the sparse directory benchmark, several races/conflicts were discovered and adding the synchornization primitives was time consuming and made the code very complex. Also for sparse entries as the program processed more entries the vector `addedEntries` kept getting larger. So before processing an entry an `O(n)` string comparison operation takes place. When combined with the overarching loop which iterates over all the archives, the algorithm becomes `O(n^2)` thereby overshadowing the benefits of multithreading. Due to these reasons, this implementation was given up on.
libarchive also doesn't seem to support random access so starting threading at different locations is not an option.

## Version 2: python2
### Dependencies
1. python2 located at `/usr/bin/python`
2. python modules: io, tarfile, sys

### Instructions to run
1. `cd src2`
2. `cat <archive-location>.tar.gz | ./program1.py | ./program2.py`

### Architecture
The architecture is similar to the C++ implementation. In this case the `tarfile.open()` function is used to open a stream to `stdin` (called input_tar) and `stdout` (called output_tar) in program1. Program1 iterates over all the members of the archive, extracts each file and sends it to the output stream. The symlinks(both hard and soft) are skipped, the details are provied in the testing section. 

### Testing
#### Edge cases : result
1. Empty archives : extracts to an empty folder
2. Archives with archives: Keeps the inner arhives i.e. doesn't perform recursive decompression
3. Archives with symlinks: tarfile cannot provide a fileobject to a symlink in an input stream based on looking at the source code.( [Search for the term "cannot extract (sym)link as file object](https://duplicity.readthedocs.io/en/latest/_modules/tarfile.html) and the workaround is to read from the location being pointed to. And python in general seems to provide limited support for symlink extraction from tarfiles, based on these links([Issue page](https://bugs.python.org/issue35483) [Stackoverflow post](https://stackoverflow.com/questions/10060069/safely-extract-zip-or-tar-using-python)). python seems to follow the path of the symlink to extract them which goes against the rule that program1 cannot perform disk IO access. So for now all symlinks(both hard and soft) are skipped. 
4. Archives with named pipes/fifos: Extracts the fifo without problems
5. No input/invalid input to program_1: Returns tar.ReadError and exits
6. Archives with different formats(zip, tar, etc): tarfile module supports gzip, bz2 and lzma ([link](https://docs.python.org/3/library/tarfile.html))
#### Verification
Verification was perfomed using the `diff -ur <dir1> <dir2>` bash command which recursively compares whether two directories and their corresponding files are the same.

#### Optimization
Since, only tarfile member functions were used to directly perform the streaming decompression there are no optimization parametes

#### Other Attempts
1. *Python3*: Initially I was writing the programs in python3, python3 had encoding issues when trying to read from the `stdin` stream. There are characters part of the `.tar.gz` format that cannot be represented with the `utf-8` encoding. Even after specifying the right encoding in the `open()` call, the `read()` operation still gave the errors. There were workarounds available but they required installing external modules. So upon switching to python2, the issue was resolved.
2. *gzip*: Used the gzip module instead of the tarfile module. It had only one function to decompress called `decompress()` and it accepted a `bytes` object containing the uncompressed data. This module didn't allow streaming so switched over to the tarfile module.
3. *shutil*: This was an external module that supported decompressing a larger range of archive types. But the `unpack_archive()` function only accepted full path names and not file object which is required for streaming.
