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
    if (!filesystem::exists(directory) || !filesystem::is_directory(directory)) {
        return FileSystem::E_INVALID_DIRECTORY;
    }

    // Discover main_file_cache.dat2 (the '/' operator glues two paths)
    filesystem::path mainFile = filesystem::path(directory) / std::string("main_file_cache.dat2");
    if (!filesystem::exists(mainFile)) {
        return FileSystem::E_NO_MAINFILE;
    }

    // Load all the *.idx files
    std::map<int, filesystem::path*> validIndices; // Map to hold our .idx file paths
    for (filesystem::directory_entry e : make_iterator_range(filesystem::directory_iterator(directory))) { // Loop
        if (starts_with(e.path().extension().string(), ".idx") && e.path().string().size() > 4) { // Is this an index descriptor?
            int idxId = atoi(e.path().extension().string().substr(4).c_str()); // Substring from 4, parse as integer.

            // Check if the range is valid. If so, store it in our map.
            if (idxId >= 0 && idxId <= 255) {
                validIndices[idxId] = new filesystem::path(e); // Copy, otherwise it falls out of scope
            }
        }
    }

    // Set this instance's map to the allocated one
    this->validIndices = validIndices;


    return FileSystem::RESULT_OK; // All good.
}
