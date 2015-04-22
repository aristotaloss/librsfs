//
// Created by Bart on 4/21/2015.
//

#include "FileSystem.h"

FileSystem::FileSystem() {
    // Stub
}

FileSystem::~FileSystem() {
    // Free up the path instances created with 'new'
    for (auto pair : validIndices) {
        delete pair.second;
    }
}

FSResult FileSystem::loadDirectory(char *directory) {
    // Does this directory exist... or is it even a directory?
    if (!exists(directory) || !is_directory(directory)) {
        return FileSystem::E_INVALID_DIRECTORY;
    }

    // Discover main_file_cache.dat2 (the '/' operator glues two paths)
    path mainFile = path(directory) / string("main_file_cache.dat2");
    if (!exists(mainFile)) {
        return E_NO_MAINFILE;
    }

    // Load all the *.idx files
    map<int, path*> validIndices; // Map to hold our .idx file paths
    for (directory_entry e : make_iterator_range(directory_iterator(directory))) { // Loop
        path current = e.path();
        string extension = current.extension().string();

        if (starts_with(extension, ".idx") && extension.size() > 4) { // Is this an index descriptor?
            int idxId = atoi(extension.substr(4).c_str()); // Substring from 4, parse as integer.

            // Check if the range is valid. If so, store it in our map.
            if (idxId >= 0 && idxId <= 255) {
                validIndices[idxId] = new path(current); // Copy, otherwise it falls out of scope
            }
        }
    }

    // Set this instance's map to the allocated one
    this->validIndices = validIndices;

    return FileSystem::RESULT_OK; // All good.
}
