#pragma once

// c api interface for e.g. JNA
#include "librsfs.h"
#include "file_system.h"

extern "C" {
	RSFS_EXPORT FileSystem *filesystem_open(char *path);
	RSFS_EXPORT void filesystem_free(FileSystem *);
	RSFS_EXPORT int filesystem_index_count(FileSystem *);
	RSFS_EXPORT char *filesystem_read_raw(FileSystem *, int, int);
	RSFS_EXPORT void filesystem_free_data(char *);
}