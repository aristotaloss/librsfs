//
// Created by Bart on 4/21/2015.
//

#include "file_system.h"

FileSystem::FileSystem() {
    // Stub
}

FileSystem::~FileSystem() {
    // Stub
}

FSResult FileSystem::LoadDirectory(char *directory) {
    // Does this directory exist... or is it even a directory?
    if (!boost::filesystem::exists(directory) || !boost::filesystem::is_directory(directory)) {
        return FileSystem::E_INVALID_DIRECTORY;
    }

    // Discover main_file_cache.dat2 (the '/' operator glues two paths)
    path mainfile = path(directory) / string("main_file_cache.dat2");
    if (!exists(mainfile)) {
        return E_NO_MAINFILE;
    }

    // Load all the *.idx files
    map<int, path> valid_indices; // Map to hold our .idx file paths
    for (directory_entry e : boost::make_iterator_range(directory_iterator(directory))) { // Loop
        path current = e.path();
        string extension = current.extension().string();

        if (boost::starts_with(extension, ".idx") && extension.size() > 4) { // Is this an index descriptor?
            int idxId = atoi(extension.substr(4).c_str()); // Substring from 4, parse as integer.

            // Check if the range is valid. If so, store it in our map.
            if (idxId >= 0 && idxId <= 255) {
                valid_indices[idxId] = current;
            }
        }
    }

    // Set this instance's map to the allocated one
    this->valid_indices = valid_indices;

    return FileSystem::RESULT_OK; // All good.
}

int FileSystem::GetIndexCount() {
    int count = 0;

    for (auto &entry : valid_indices) {
        if (entry.first != 255) // We don't see the fs descriptor as an index
            count++;
    }

    return count;
}

bool FileSystem::HasIndex(int index) {
    return valid_indices.count(index) != 0;
}
