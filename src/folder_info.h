//
// Created by Bart on 4/24/2015.
//

#ifndef LIBRSFS_FOLDER_H
#define LIBRSFS_FOLDER_H

#include <stdint.h>

class FolderInfo {

private:
	int id;
	int size;
	int offset;

public:
	FolderInfo(int id, int size, int offset);
	~FolderInfo();
	int GetSize();
	int GetId();
	uint64_t GetOffset();
	bool Exists();
};


#endif //LIBRSFS_FOLDER_H

