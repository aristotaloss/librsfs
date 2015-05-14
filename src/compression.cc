
#include "compression.h"

CompressionType Compression::GetCompressionType(char first_byte) {
	switch (first_byte) {
	case 0:
		return CompressionType::NONE;
	case 1:
		return CompressionType::BZIP2;
	case 2:
		return CompressionType::GZIP;
	default:
		throw std::runtime_error(std::string("invalid compression type ") + boost::lexical_cast<std::string>(first_byte));
	}
}

CompressionInfo::CompressionInfo(std::vector<char> &data) {
	compression_type = Compression::GetCompressionType(data[0]);
	compressed_size = ((data[1] & 0xFF) << 24) | ((data[2] & 0xFF) << 16) | ((data[3] & 0xFF) << 8) | (data[4] & 0xFF);
	decompressed_size = ((data[5] & 0xFF) << 24) | ((data[6] & 0xFF) << 16) | ((data[7] & 0xFF) << 8) | (data[8] & 0xFF);
}

int Compression::Decompress(std::vector<char> &original, std::vector<char> &destination) {
	CompressionInfo compression_info(original);
	auto compression_type = compression_info.GetCompressionType();

	destination.resize(compression_info.GetDecompressedSize());

	if (compression_type == CompressionType::GZIP) { // Decompress using gzip (or well, skip the gzip header and use plain lzma inflating)
		auto strm = new z_stream();
		strm->avail_in = original.size();
		strm->next_in = reinterpret_cast<Byte*>(original.data() + 19);
		strm->avail_out = destination.capacity();
		strm->next_out = reinterpret_cast<Byte*>(destination.data());

		inflateInit2(strm, -15);
		inflate(strm, Z_FINISH);
		inflateEnd(strm);
		int size = strm->total_out;
		delete strm;

		return size;
	}

	return 0;
}

CompressionType CompressionInfo::GetCompressionType() {
	return compression_type;
}

int CompressionInfo::GetCompressedSize() {
	return compressed_size;
}

int CompressionInfo::GetDecompressedSize() {
	return decompressed_size;
}
