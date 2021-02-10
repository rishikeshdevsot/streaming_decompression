#include <archive.h>
#include <archive_entry.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <cstring>


void die(std::string fmt)
{
	fprintf(stderr, fmt.c_str());
	fprintf(stderr, "\n");
	exit(1);
}

int main(){
    struct archive *a;
    struct archive_entry *entry;
    int r;

    // Setup reader with only uncompressed filter
    // Confirms that the stdin input is uncompressed
    a = archive_read_new();
    archive_read_support_filter_none(a);
    archive_read_support_format_all(a);

    // Read from stdin	    	    
    r = archive_read_open_fd(a, 0, 10240);
    // Invalid input or no input
    if(r != ARCHIVE_OK)
	   die("Couldn't open input archive\n"); 
    // Write all files to disk
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
  	if(archive_read_extract(a, entry, ARCHIVE_EXTRACT_TIME) != ARCHIVE_OK)
		die("Couldn't extract to diski\n");

    }
    r = archive_read_free(a);  // Note 3
    if (r != ARCHIVE_OK)
	    die("Error closing archive\n");
	    exit(1);
}
