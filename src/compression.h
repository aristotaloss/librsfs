//
// Created by Bart on 4/24/2015.
//

#pragma once

#include <vector>
#include "zlib/zlib.h"
#include "bzip2/bzlib.h"
#include <exception>
#include <stdexcept>
#include <memory.h>

#ifdef LZMA_FOUND
#include <lzma.h>
#pragma message("Compiling with LZMA support enabled.")
#else
#pragma message("LZMA libraries not detected by cmake, LZMA support not enabled.")
#endif

const static char *BZIP_HEADER = "BZh1";

enum CompressionType {
	NONE = 0,
	BZIP2 = 1,
	GZIP = 2,
	LZMA = 3
};

class Compression {
public:
	static CompressionType get_compression_type(char first_byte);
	static int decompress(std::vector<char>& original, std::vector<char>& destination);
};

class CompressionInfo {
private:
	CompressionType compression_type;
	int compressed_size;
	int decompressed_size;
public:
	CompressionInfo(std::vector<char>& data);
	CompressionType get_compression_type();
	int get_compressed_size();
	int get_decompressed_size();
};
