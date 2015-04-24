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

void FileSystem::LoadDirectory(char *directory) {
    // Does this directory exist... or is it even a directory?
    if (!boost::filesystem::exists(directory) || !boost::filesystem::is_directory(directory)) {
        throw std::runtime_error("directory is invalid or does not exist");
    }

    // Discover main_file_cache.dat2 (the '/' operator glues two paths)
    path mainfile = path(directory) / "main_file_cache.dat2";
    if (!boost::filesystem::exists(mainfile)) {
        throw std::runtime_error("filestore does not contain main_file_cache.dat2");
    }

    // Load all the *.idx files
    map<int, path> valid_indices; // Map to hold our .idx file paths
    for (directory_entry e : boost::make_iterator_range(directory_iterator(directory))) { // Loop
        path current = e.path();
        string extension = current.extension().string();

        if (boost::starts_with(extension, ".idx") && extension.size() > 4) { // Is this an index descriptor?
            int index_id = atoi(extension.substr(4).c_str()); // Substring from 4, parse as integer.

            // Check if the range is valid. If so, store it in our map.
            if (index_id >= 0 && index_id <= 255) {
                valid_indices[index_id] = current;
            }
        }
    }

    // Set this instance's map to the allocated one
    this->valid_indices = valid_indices;
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
