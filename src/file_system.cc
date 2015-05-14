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
    // Do not allow to reload a directory
    if (!main_file.empty()) {
        throw std::runtime_error("file system has already been loaded");
    }

    // Does this directory exist... or is it even a directory?
    if (!boost::filesystem::exists(directory) || !boost::filesystem::is_directory(directory)) {
        throw std::runtime_error("directory is invalid or does not exist");
    }

    // Discover main_file_cache.dat2 (the '/' operator glues two paths)
	auto main_file = path(directory) / "main_file_cache.dat2";
    if (!boost::filesystem::exists(main_file)) {
        throw std::runtime_error("filestore does not contain main_file_cache.dat2");
    }
    this->main_file = main_file;

    // Load all the *.idx files
    for (auto e : boost::make_iterator_range(directory_iterator(directory))) { // Loop
	    auto current = e.path();
	    auto extension = current.extension().string();

        if (boost::starts_with(extension, ".idx") && extension.size() > 4) { // Is this an index descriptor?
            int index_id = atoi(extension.substr(4).c_str()); // Substring from 4, parse as integer.

            // Check if the range is valid. If so, store it in our map.
            if (index_id >= 0 && index_id <= 255) {
                indices.insert(std::make_pair(index_id, DirectoryIndex(this, current)));
            }
        }
    }
}

int FileSystem::GetIndexCount() {
	auto count = 0;

    for (auto &entry : indices) {
        if (entry.first != 255) // We don't see the fs descriptor as an index
            count++;
    }

    return count;
}

bool FileSystem::HasIndex(int index) {
    return indices.count(index) != 0;
}

DirectoryIndex FileSystem::GetIndex(int directory_id) {
    return indices.at(directory_id);
}

int FileSystem::Read(FolderInfo info, vector<char> &dest) {
    // Make sure this operation isn't going to fail miserably
    if (!info.Exists())
        return 0;

    boost::filesystem::ifstream data_stream(main_file, std::ios_base::binary);
    data_stream.seekg(static_cast<int>(info.GetOffset()), data_stream.beg);

    // The big format was introduced around revision 667 because the IDs began to exceed 65535.
    bool big_format = info.GetId() > 0xFFFF;
    int header_size = big_format ? 10 : 8;
	int data_size = BLOCK_SIZE - header_size;
    int remaining = info.GetSize();

	char scratch_buffer[BLOCK_SIZE];
    int current_index = 0;
    int current_id = info.GetId();
	int current_part = -1;

    while (remaining > 0) {
		data_stream.read(scratch_buffer, BLOCK_SIZE); // Grab some data to use

        uint32_t folder_id;
		int part_id;
        int next_offset;
        int next_index;

        if (big_format) {
            folder_id = static_cast<uint32_t>((scratch_buffer[0] & 0xFF << 24) | (scratch_buffer[1] & 0xFF << 16) | (scratch_buffer[2] & 0xFF << 8) | (scratch_buffer[3] & 0xFF));
			part_id = ((scratch_buffer[4] & 0xFF) << 8) | (scratch_buffer[5] & 0xFF);
            next_offset = (((scratch_buffer[6] & 0xFF) << 16) | ((scratch_buffer[7] & 0xFF) << 8) | ((scratch_buffer[8]) & 0xFF));
            next_index = scratch_buffer[9] & 0xFF;
        } else {
            folder_id = static_cast<uint32_t>(((scratch_buffer[0] & 0xFF) << 8) | ((scratch_buffer[1]) & 0xFF));
			part_id = ((scratch_buffer[2] & 0xFF) << 8) | (scratch_buffer[3] & 0xFF);
            next_offset = (((scratch_buffer[4] & 0xFF) << 16) | ((scratch_buffer[5] & 0xFF) << 8) | ((scratch_buffer[6]) & 0xFF));
            next_index = scratch_buffer[7] & 0xFF;
        }

		// Check if everything is still OK
        if (folder_id != current_id || part_id != current_part + 1) {
            throw std::runtime_error("malformed folder (sequence does not complete)");
        }

        // Fill dest buffer with remaining data
	    auto num_left = remaining > data_size ? data_size : remaining;
        dest.insert(dest.end(), &scratch_buffer[header_size], &scratch_buffer[num_left + header_size]);

        // Position us to the next block
		data_stream.seekg(static_cast<long>(next_offset) * BLOCK_SIZE, data_stream.beg);

		current_part = part_id;
        remaining -= data_size;
    }

	data_stream.close();
    return dest.size();
}
