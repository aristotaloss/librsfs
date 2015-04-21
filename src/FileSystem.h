//
// Created by Bart on 4/21/2015.
//

#ifndef LIBRSFS_FILESYSTEM_H
#define LIBRSFS_FILESYSTEM_H

#include "librsfs.h"
#include <stdio.h>
#include <boost/filesystem.hpp>
#include <boost/range.hpp>
#include <boost/algorithm/string.hpp>

using namespace boost; // Save some typing

typedef int FSResult;

class RSFS_EXPORT FileSystem {

public:
    FileSystem();
    ~FileSystem();
    RSFS_EXPORT FSResult loadDirectory(char *);

    static const FSResult RESULT_OK = 0;
    static const FSResult E_INVALID_DIRECTORY = 1;
    static const FSResult E_NO_MAINFILE = 2;

};


#endif //LIBRSFS_FILESYSTEM_H
