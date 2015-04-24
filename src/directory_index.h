//
// Created by Bart on 4/24/2015.
//

#ifndef LIBRSFS_DIRECTORYINDEX_H
#define LIBRSFS_DIRECTORYINDEX_H

#include "file_system.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>

using boost::filesystem::path;

// Predeclarations
class FileSystem;

class DirectoryIndex {

private:
    FileSystem *file_system;
    path index_file;
    path main_file;

public:
    DirectoryIndex(FileSystem *, path);
    ~DirectoryIndex();

};


#endif //LIBRSFS_DIRECTORYINDEX_H
