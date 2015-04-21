//
// Created by Bart on 4/21/2015.
//

#include "FileSystem.h"

FileSystem::FileSystem() {
    // Stub
}

FileSystem::~FileSystem() {
    // Stub
}

FSResult FileSystem::loadDirectory(char *directory) {
    // Does this directory exist... or is it even a directory?
    if (!exists(directory) || !is_directory(directory)) {
        return FileSystem::E_INVALID_DIRECTORY;
    }

    // Discover main_file_cache.dat2 (the '/' operator glues two paths)
    path mainFile = path(directory) / string("main_file_cache.dat2");
    if (!filesystem::exists(mainFile)) {
        return FileSystem::E_NO_MAINFILE;
    }

    // Load all the *.idx files
    map<int, path*> validIndices; // Map to hold our .idx file paths
    for (directory_entry e : make_iterator_range(directory_iterator(directory))) { // Loop
        if (starts_with(e.path().extension().string(), ".idx") && e.path().string().size() > 4) { // Is this an index descriptor?
            int idxId = atoi(e.path().extension().string().substr(4).c_str()); // Substring from 4, parse as integer.

            // Check if the range is valid. If so, store it in our map.
            if (idxId >= 0 && idxId <= 255) {
                validIndices[idxId] = new path(e); // Copy, otherwise it falls out of scope
            }
        }
    }

    // Set this instance's map to the allocated one
    this->validIndices = validIndices;


    return FileSystem::RESULT_OK; // All good.
}
