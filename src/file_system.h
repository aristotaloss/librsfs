//
// Created by Bart on 4/21/2015.
//

#ifndef LIBRSFS_FILESYSTEM_H
#define LIBRSFS_FILESYSTEM_H

#include "librsfs.h"
#include "folder_info.h"
#include "directory_index.h"
#include <stdio.h>
#include <map>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/range.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

using std::map;
using std::vector;
using std::string;
using boost::filesystem::path;
using boost::filesystem::directory_entry;
using boost::filesystem::directory_iterator;

typedef int FSResult;

// Predeclarations
class DirectoryIndex;

/**
 * Filesystem base class which will hold the main file reference and also contains a map of all the indices.
 * After constructing a new FileSystem, use FileSystem#LoadDirectory(char*).
 */
class RSFS_EXPORT FileSystem {

public:
    FileSystem();
    ~FileSystem();

    /**
     * Load a file system from the specified path. A valid file system requires at least a main_file_cache.dat2
     * file and anywhere from one to 255 .idx<num> files, .idx255 being mandatory.
     *
     * @param directory The directory the file store can be found in.
     */
    void LoadDirectory(char *directory);

    /**
     * Gets the number of (valid) indices in the file store. This method will yield 0 if the file system has not
     * yet been loaded. This does not take the .idx255 file into the final result. Only 0-254 indices are counted.
     *
     * @return The number of valid indices found in this file system, excluding the .idx255 file.
     */
    int GetIndexCount();

    /**
     * Checks if this file system contains the specified index. If this file store was not loaded, this will always return
     * <code>false</code>. If an index is present, this will return <code>true</code> unless the passed index id is 255,
     * since the .idx255 file is not taken into consideration.
     */
    bool HasIndex(int);

    DirectoryIndex GetIndex(int directory_id);
    int Read(FolderInfo info, vector<char> &dest);

    static const FSResult RESULT_OK = 0;
    static const FSResult E_INVALID_DIRECTORY = 1;
    static const FSResult E_NO_MAINFILE = 2;

private:
    path main_file;
    map<int, DirectoryIndex> indices;

};


#endif //LIBRSFS_FILESYSTEM_H
