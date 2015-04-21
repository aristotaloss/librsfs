//
// Created by Bart on 4/21/2015.
//

#ifndef LIBRSFS_FILESYSTEM_H
#define LIBRSFS_FILESYSTEM_H

#include "librsfs.h"
#include <stdio.h>
#include <map>
#include <boost/filesystem.hpp>
#include <boost/range.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;
using namespace boost::filesystem;

typedef int FSResult;

class RSFS_EXPORT FileSystem {

public:
    FileSystem();
    ~FileSystem();
    RSFS_EXPORT FSResult loadDirectory(char *);

    static const FSResult RESULT_OK = 0;
    static const FSResult E_INVALID_DIRECTORY = 1;
    static const FSResult E_NO_MAINFILE = 2;

private:
    std::map<int, filesystem::path*> validIndices;

};


#endif //LIBRSFS_FILESYSTEM_H
