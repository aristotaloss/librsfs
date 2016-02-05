#include <iostream>
#include <fstream>
#include "file_system.h"
#include "compression.h"

using namespace std;

int main() {
    FileSystem filesystem;

	try {
		filesystem.LoadDirectory(const_cast<char *>("/home/server/rsfs/librsfs/data/"));

		for (auto a = 0; a < filesystem.GetIndexCount(); a++) {
			//int a = 255;
			printf("Index %d has %d folders\n", a, filesystem.GetIndex(a)->GetEntryCount());

			for (int f = 0; f < filesystem.GetIndex(a)->GetEntryCount(); f++) {
				try {
					FolderInfo info = filesystem.GetIndex(a)->GetFolderInfo(f);
					printf("%d,%d,%d\n", info.GetId(), info.GetOffset(), info.GetSize());
					vector<char> data0_11;
					int rdnum = filesystem.Read(info, data0_11);

					if (rdnum >= 5) {
						auto compress = CompressionInfo(data0_11);

						if (compress.GetCompressionType() == LZMA) {
							printf("FOUND\n");
						}
					}

					//if (rdnum > 0)
					// 	printf("it worked for %d but %d..\n", f, data0_11[0]);
					//else
					// 	printf("folder %d seems empty? %d %d %ull\n", f, info.GetSize(), info.GetId(), info.GetOffset());
				} catch (const std::exception &exc) {
					printf("error with %d: %s\n", f, exc.what());
				}
			}
		}

		printf("Number of indices: %d\n", filesystem.GetIndexCount());
		printf("Has index 15: %d, has 16: %d\n", filesystem.HasIndex(15), filesystem.HasIndex(16));
		printf("Number of files in model directory: %d\n", filesystem.GetIndex(7)->GetEntryCount());
		//filesystem.GetIndex(7)
    
		FolderInfo info = filesystem.GetIndex(2)->GetFolderInfo(10);
		vector<char> data;
		int rdnum = filesystem.Read(info, data);
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
