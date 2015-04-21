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
    std::vector<filesystem::path> validIndices; // Vector to hold our .idx file paths
    for (filesystem::directory_entry e : make_iterator_range(filesystem::directory_iterator(directory))) { // Loop
        if (starts_with(e.path().extension().string(), ".idx")) { // Is this an index descriptor?
            printf(".idx entry: %s\n", e.path().string().c_str());
        }
    }

    return FileSystem::RESULT_OK;
}
