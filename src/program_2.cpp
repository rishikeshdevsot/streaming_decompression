#include <archive.h>
#include <archive_entry.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <cstring>


std::string readUntilEOF(){ 
    std::stringstream ss;
    std::string archiveInputLine;
    
    //Read until EOF
    while(getline(std::cin, archiveInputLine)){ 
        ss << archiveInputLine << "\n";
    }
    // Convert the string stream to a string
    std::string fullArchive(ss.str());
    return fullArchive;



}

// Resources
//
int main(){
    struct archive *a;
    struct archive_entry *entry;
    struct archive_entry *ae;
    int r;

    //std::string archive_compressed = readUntilEOF();
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);

    // Read the string as a void *
    //r = archive_read_open_memory(a, &archive_compressed[0], archive_compressed.length());
    r = archive_read_open_fd(a, 0, 10240); 
    if (r != ARCHIVE_OK){
        fprintf(stderr, "Couldn't create archive object from stdin\n");
        exit(1);
    }
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
  	//printf("%s \n",archive_entry_pathname(entry));
  	archive_read_extract(a, entry, ARCHIVE_EXTRACT_TIME);
	//archive_read_data_skip(a);  // Note 2
    }
    r = archive_read_free(a);  // Note 3
    if (r != ARCHIVE_OK)
	    exit(1);
}
