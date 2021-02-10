# sample_compression

## Versions
1. Version1: Using C++
2. Version2: Using python2


## Version 1
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

### Edge cases
1. Empty archives : extracts to an empty folder
2. Archives with archives: Keeps the inner arhives i.e. doesn't perform recursive decompression
3. Archives with symlinks: Extracts the symlinks which still contain the same data(for both hard and soft links)
4. Archives with names pipes/fifos: Extracts the fifo without problems
5. 
