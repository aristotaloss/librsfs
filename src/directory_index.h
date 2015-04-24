//
// Created by Bart on 4/24/2015.
//

#ifndef LIBRSFS_DIRECTORYINDEX_H
#define LIBRSFS_DIRECTORYINDEX_H


#include "file_system.h"
#include <boost/filesystem.hpp>

using boost::filesystem::path;

class DirectoryIndex {

private:
    FileSystem *file_system;
    path index_file;
    path main_file;

public:
    DirectoryIndex(FileSystem *);
    ~DirectoryIndex();

};


#endif //LIBRSFS_DIRECTORYINDEX_H
