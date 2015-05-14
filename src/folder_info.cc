//
// Created by Bart on 4/24/2015.
//

#include "folder_info.h"


FolderInfo::FolderInfo(int id, int size, int offset) {
	this->id = id;
	this->size = size;
	this->offset = offset;
}

FolderInfo::~FolderInfo() {
	// Stub
}

int FolderInfo::GetSize() {
	return size;
}

uint64_t FolderInfo::GetOffset() {
	return static_cast<uint64_t>(offset) * 520L;
}

bool FolderInfo::Exists() {
	return size > 0;
}

int FolderInfo::GetId() {
	return id;
}
