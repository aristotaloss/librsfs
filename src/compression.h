//
// Created by Bart on 4/24/2015.
//

#ifndef LIBRSFS_COMPRESSION_H
#define LIBRSFS_COMPRESSION_H

#include <boost/lexical_cast.hpp>
#include <vector>
#include <zlib.h>
#include <bzlib.h>
#include <lzma.h>

const static char *BZIP_HEADER = "BZh1";

enum CompressionType {
	NONE = 0,
	BZIP2 = 1,
	GZIP = 2,
	LZMA = 3
};

class Compression {
public:
	static CompressionType GetCompressionType(char first_byte);
	static int Decompress(std::vector<char>& original, std::vector<char>& destination);
};

class CompressionInfo {
private:
	CompressionType compression_type;
	int compressed_size;
	int decompressed_size;
public:
	CompressionInfo(std::vector<char>& data);
	CompressionType GetCompressionType();
	int GetCompressedSize();
	int GetDecompressedSize();
};

#endif //LIBRSFS_COMPRESSION_H

