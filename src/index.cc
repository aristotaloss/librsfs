//
// Created by Bart on 4/24/2015.
//

#include "index.h"

Index::Index(FileSystem *file_system, std::string index_file) {
	this->file_system = file_system;
	this->index_file = std::string(index_file.length() + 1, '\0');

	memcpy((void*) this->index_file.data(), index_file.data(), index_file.length() + 1);
}

Index::~Index() {

}

int Index::get_entry_count() {
	std::ifstream in(index_file.c_str(), std::ifstream::ate | std::ifstream::binary);
	int count = static_cast<int>(in.tellg() / 6L);
	in.close();
	return count;
}

FolderInfo Index::get_folder_info(int id) {
	if (id >= get_entry_count())
		return FolderInfo(0, 0, 0);

	// Create a new stream and position it at the start of the file
	ifstream *stream = new ifstream(index_file.c_str(), std::ios_base::in | std::ios_base::binary);
	stream->seekg(id * 6, stream->beg);

	// Were we able to seek there?
	if (id * 6 != stream->tellg())
		return FolderInfo(0, 0, 0);

	// Read the six bytes of info from the stream
	char size_buf[3], offset_buf[3];
	stream->read(size_buf, 3);
	stream->read(offset_buf, 3);
	stream->close();

	// Turn data into integers and return info
	auto size_in_bytes = ((size_buf[0] & 0xFF) << 16) | ((size_buf[1] & 0xFF) << 8) | (size_buf[2] & 0xFF);
	auto offset_in_blocks = ((offset_buf[0] & 0xFF) << 16) | ((offset_buf[1] & 0xFF) << 8) | (offset_buf[2] & 0xFF);

	delete stream;
	return FolderInfo(id, size_in_bytes, offset_in_blocks);
}

std::string Index::get_file() {
	return index_file;
}

int Index::get_raw(int id, vector<char> &dest) {
	FolderInfo info = get_folder_info(id);
	if (!info.exists())
		return 0;

	return file_system->read(info, dest);
}

int Index::get_decompressed(int id, vector<char> &dest) {
	FolderInfo info = get_folder_info(id);
	if (!info.exists())
		return 0;

	return file_system->read_decompressed(info, dest);
}
