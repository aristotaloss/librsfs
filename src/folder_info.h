//
// Created by Bart on 4/24/2015.
//

#ifndef LIBRSFS_FOLDER_H
#define LIBRSFS_FOLDER_H

#include <stdint.h>

class FolderInfo {

private:
    int size;
    int offset;

public:
    FolderInfo(int size, int offset);
    ~FolderInfo();
    int GetSize();
    uint64_t GetOffset();
    bool Exists();

};


#endif //LIBRSFS_FOLDER_H
