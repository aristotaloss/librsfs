#include <iostream>
#include "file_system.h"

using namespace std;

int main() {
    FileSystem filesystem;
    FSResult result = filesystem.LoadDirectory(const_cast<char *>("C:\\Users\\Bart\\Documents\\osrs-server\\data\\filestore"));

    if (result == FileSystem::E_INVALID_DIRECTORY) {
        printf("Could not open directory.");
        return result;
    }

    printf("FS result: %d\n", result);
    printf("Number of indices: %d\n", filesystem.GetIndexCount());
    printf("Has index 15: %d, has 16: %d\n", filesystem.HasIndex(15), filesystem.HasIndex(16));
    return 0;
}