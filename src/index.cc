//
// Created by Bart on 4/24/2015.
//

#include "index.h"

Index::Index(FileSystem *file_system, path index_file) {
	this->file_system = file_system;
	this->index_file = index_file;
}

Index::~Index() {

}

int Index::GetEntryCount() {
	return static_cast<int>(boost::filesystem::file_size(index_file) / 6);
}

FolderInfo Index::GetFolderInfo(int id) {
	if (id >= GetEntryCount())
		return FolderInfo(0, 0, 0);

	// Create a new stream and position it at the start of the file
	boost::filesystem::ifstream stream(index_file, std::ios_base::in | std::ios_base::binary);
	stream.seekg(id * 6, stream.beg);

	// Were we able to seek there?
	if (id * 6 != stream.tellg())
		return FolderInfo(0, 0, 0);

	// Read the six bytes of info from the stream
	char size_buf[3], offset_buf[3];
	stream.read(size_buf, 3);
	stream.read(offset_buf, 3);

	// Turn data into integers and return info
	auto size_in_bytes = ((size_buf[0] & 0xFF) << 16) | ((size_buf[1] & 0xFF) << 8) | (size_buf[2] & 0xFF);
	auto offset_in_blocks = ((offset_buf[0] & 0xFF) << 16) | ((offset_buf[1] & 0xFF) << 8) | (offset_buf[2] & 0xFF);

	// Close the stream to release the file lock
	stream.close();

	return FolderInfo(id, size_in_bytes, offset_in_blocks);
}

path Index::GetFile() {
	return index_file;
}

int Index::GetRaw(int id, vector<char> &dest) {
	FolderInfo info = GetFolderInfo(id);
	if (!info.Exists())
		return 0;

	return file_system->Read(info, dest);
}

int Index::GetDecompressed(int id, vector<char> &dest) {
	FolderInfo info = GetFolderInfo(id);
	if (!info.Exists())
		return 0;

	return file_system->ReadAndDecompress(info, dest);
}
