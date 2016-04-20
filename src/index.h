//
// Created by Bart on 4/24/2015.
//

#ifndef LIBRSFS_DIRECTORYINDEX_H
#define LIBRSFS_DIRECTORYINDEX_H

#include "file_system.h"
#include "folder_info.h"
#include <vector>
#include <string>
#include <memory.h>

using namespace std;

// Predeclarations
class FileSystem;
class FolderInfo;

RSFS_EXPORT class Index {
private:
	FileSystem* file_system;
	std::string index_file;
	std::string main_file;

public:
	Index(FileSystem*, std::string);
	~Index();

	/**
         * Gets the (expected) number of entries in this directory. Note: This does not correspond to the actual precise
         * number of non-empty data entries as a directory may contain a folder with ID 4 and one with ID 6 for example,
         * which will make this method return 6 even though only 2 are valid.
         */
	RSFS_EXPORT int get_entry_count();

	RSFS_EXPORT FolderInfo get_folder_info(int id);

	RSFS_EXPORT std::string get_file();

	RSFS_EXPORT int get_raw(int id, vector<char> &dest);

	RSFS_EXPORT int get_decompressed(int id, vector<char> &dest);
};


#endif //LIBRSFS_DIRECTORYINDEX_H

