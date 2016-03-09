#include "capi.h"

FileSystem *filesystem_open(char *path) {
	FileSystem *loaded = new FileSystem();
	loaded->load_directory(path);

	return loaded;
}

void filesystem_free(FileSystem *fs) {
	if (fs) {
		delete fs;
	}
}

int filesystem_index_count(FileSystem *fs) {
	if (fs) {
		return fs->get_index_count();
	}

	return 0;
}

char* filesystem_read_raw(FileSystem *fs, int index, int folder) {
	if (fs) {
		vector<char> dest;
		fs->get_index(index)->get_raw(folder, dest);
	}

	return 0;
}

void filesystem_free_data(char *buffer) {
	if (buffer) {
		free(buffer);
	}
}
