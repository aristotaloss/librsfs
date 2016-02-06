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

int FolderInfo::get_size() {
	return size;
}

uint64_t FolderInfo::get_offset() {
	return static_cast<uint64_t>(offset) * 520L;
}

bool FolderInfo::exists() {
	return size > 0;
}

int FolderInfo::get_id() {
	return id;
}
