#include "compression.h"

CompressionType Compression::GetCompressionType(char first_byte) {
	switch (first_byte) {
	case 0:
		return CompressionType::NONE;
	case 1:
		return CompressionType::BZIP2;
	case 2:
		return CompressionType::GZIP;
	case 3:
		return CompressionType::LZMA;
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
	} else if (compression_type == CompressionType::BZIP2) {
		// First of all, initialize the stream with just the missing header bytes
		bz_stream *strm = new bz_stream();
		strm->avail_in = 4;
		strm->next_in = const_cast<char*>(BZIP_HEADER);
		strm->avail_out = destination.capacity();
		strm->next_out = destination.data();

		// Let BZ2 decode that header for us..
		int init_error = BZ2_bzDecompressInit(strm, 0, 0);
		int bzipresult = BZ2_bzDecompress(strm);

		// .. And now feed the real data.
		strm->avail_in = original.size() - 9;
		strm->next_in = original.data() + 9;
		bzipresult = BZ2_bzDecompress(strm);

		// Free the stream and return the number of read bytes
		int read = strm->total_out_lo32;
		delete strm;

		return read;
	} else if (compression_type == CompressionType::LZMA) {
		std::vector<char> forged_input(original.size() - 9 + 8); // Properties, dict size and full size
		forged_input.resize(original.size() - 9 + 8);

		memcpy(forged_input.data(), original.data() + 9, 5); // Copy properties and dict size
		auto decompressed_size = compression_info.GetDecompressedSize();
		forged_input[5] = (char) (decompressed_size);
		forged_input[6] = (char) (decompressed_size >> 8);
		forged_input[7] = (char) (decompressed_size >> 16);
		forged_input[8] = (char) (decompressed_size >> 24);
		memcpy(forged_input.data() + 13, original.data() + 9 + 5, original.size() - 9 - 5);

		auto strm = new lzma_stream();
		lzma_alone_decoder(strm, UINT64_MAX);
		strm->avail_in = forged_input.size();
		strm->next_in = (uint8_t *) forged_input.data();
		strm->avail_out = destination.capacity();
		strm->next_out = (uint8_t *) destination.data();

		lzma_code(strm, LZMA_RUN);
		lzma_code(strm, LZMA_FINISH);

		lzma_end(strm);
		return (int) strm->total_out;
	} else if (compression_type == CompressionType::NONE) { // If no compression just copy the bytes from O to D
		memcpy(destination.data(), original.data() + 9, original.size() - 9);
		return (int) (original.size() - 9);
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
