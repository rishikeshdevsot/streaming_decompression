/*
 * Some parts taken from libarchives/examples.
 *
 */


#include <sys/stat.h>
#include <archive.h>
#include <archive_entry.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

void die(std::string fmt)
{
	fprintf(stderr, fmt.c_str());
	fprintf(stderr, "\n");
	exit(1);
}

int
main(int argc, char **argv)
{
	char buff[256];
	ssize_t len;
	int r;
	mode_t m;
	struct archive *ina;
	struct archive *outa;
	struct archive_entry *entry;

	// To Read an archive from stdin, with automatic format detection.
	ina = archive_read_new();
	archive_read_support_filter_all(ina);
	archive_read_support_format_all(ina);
	r = archive_read_open_fd(ina, 0, 10240);
	if(r != ARCHIVE_OK)
		die("Couldn't open archive fd to stdin\n");

	// To write an uncompressed ustar archive to stdout.
	outa = archive_write_new();
	archive_write_set_compression_none(outa);
	archive_write_set_format_ustar(outa);
	r = archive_write_open_fd(outa, 1);
	if(r != ARCHIVE_OK)
		die("Couldn't open fd to stdout\n");

	// Examine each entry in the input archive.
	while ((r = archive_read_next_header(ina, &entry)) == ARCHIVE_OK) {
		fprintf(stderr, "%s: ", archive_entry_pathname(entry));

		// Copy input entries to stdout
		if (archive_write_header(outa, entry) != ARCHIVE_OK)
			die("Error writing output archive");

		// If entry valid, extract and write to stdout until end of entry
		if (archive_entry_size(entry) > 0) {
			len = archive_read_data(ina, buff, sizeof(buff));
			while (len > 0) {
				if (archive_write_data(outa, buff, len) != len)
					die("Error writing output archive");
				len = archive_read_data(ina, buff, sizeof(buff));
			}
			if (len < 0)
				die("Error reading input archive");
		}
		fprintf(stderr, "copied\n");
	}
	if (r != ARCHIVE_EOF)
		die("Error reading archive");
	/* Close the archives.  */
	if (archive_read_free(ina) != ARCHIVE_OK)
		die("Error closing input archive");
	if (archive_write_free(outa) != ARCHIVE_OK)
		die("Error closing output archive");
	return (0);
}
