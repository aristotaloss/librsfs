//
// Created by Bart on 4/24/2015.
//

#ifndef LIBRSFS_DIRECTORYINDEX_H
#define LIBRSFS_DIRECTORYINDEX_H

#include "file_system.h"
#include "folder_info.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>

using boost::filesystem::path;

// Predeclarations
class FileSystem;
class FolderInfo;

class DirectoryIndex {
private:
	FileSystem* file_system;
	path index_file;
	path main_file;

public:
	DirectoryIndex(FileSystem*, path);
	~DirectoryIndex();

	/**
     * Gets the (expected) number of entries in this directory. This does not correspond to the actual precise
     * number of non-empty data entries as a directory may contain a folder with ID 4 and one with ID 6 for example,
     * which will make this method return 6 even though only 2 are valid.
     */
	int GetEntryCount();

	FolderInfo GetFolderInfo(int id);

	path GetFile();
};


#endif //LIBRSFS_DIRECTORYINDEX_H

