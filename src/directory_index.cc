//
// Created by Bart on 4/24/2015.
//

#include "directory_index.h"

DirectoryIndex::DirectoryIndex(FileSystem *file_system, path index_file) {
    this->file_system = file_system;
    this->index_file = index_file;
}

DirectoryIndex::~DirectoryIndex() {

}
