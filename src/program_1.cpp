#include <archive.h>
#include <archive_entry.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <cstring>

// Resources
// http://www.alecjacobson.com/weblog/?p=2075
int main(){
    struct archive *a;
    struct archive_entry *entry;
    int r;

    std::stringstream ss;
    std::string archiveInputLine;

    while(getline(std::cin, archiveInputLine)){ 
        ss << archiveInputLine << "\n";
    }
    std::string fullArchive(ss.str());
    char fullArchiveBuf[fullArchive.length() + 1];
    
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);
    r = archive_read_open_memory(a, &fullArchive[0], fullArchive.length());
    //r = archive_read_open_filename(a, "../samples/archive.tar.gz", 10240); // Note 1
    if (r != ARCHIVE_OK)
        exit(1);
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        printf("%s \n",archive_entry_pathname(entry));
        archive_read_data_skip(a);  // Note 2
    }
    r = archive_read_free(a);  // Note 3
    if (r != ARCHIVE_OK)
        exit(1);
}
