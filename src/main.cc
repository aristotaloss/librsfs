#include <iostream>
#include <fstream>
#include "file_system.h"
#include "compression.h"

using namespace std;

int main() {
    FileSystem filesystem;

	try {
		filesystem.load_directory(const_cast<char *>("C:\\Users\\Bart\\Documents\\osrs-server\\data\\filestore"));

		for (auto a = 0; a < filesystem.get_index_count(); a++) {
			printf("Index %d has %d folders\n", a, filesystem.get_index(a)->get_entry_count());

			for (int f = 0; f < filesystem.get_index(a)->get_entry_count(); f++) {
				try {
					FolderInfo info = filesystem.get_index(a)->get_folder_info(f);
					vector<char> data0_11;
					int rdnum = filesystem.read(info, data0_11);

					if (rdnum >= 5) {
						auto compress = CompressionInfo(data0_11);

						if (compress.GetCompressionType() == LZMA) {
							printf("FOUND\n");
						}
					}
				} catch (const std::exception &exc) {
					printf("error with %d: %s\n", f, exc.what());
				}
			}
		}

		printf("Number of indices: %d\n", filesystem.get_index_count());
		printf("Has index 15: %d, has 16: %d\n", filesystem.has_index(15), filesystem.has_index(16));
		printf("Number of files in model directory: %d\n", filesystem.get_index(7)->get_entry_count());
    
		FolderInfo info = filesystem.get_index(2)->get_folder_info(10);
		vector<char> data;
		int rdnum = filesystem.read(info, data);
		vector<char> decompressed;
		int num = Compression::Decompress(data, decompressed);

		ofstream fout;
		fout.open("file.bin", ios::binary | ios::out);
		fout.write(decompressed.data(), decompressed.size());
		fout.close();
		printf("DONE! %d\n", decompressed.size());
	} catch (const std::exception &exc) {
		printf("Error loading directory: %s\n", exc.what());
		return 0;
	}

	return 0;
}
