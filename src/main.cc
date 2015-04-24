#include <iostream>
#include "file_system.h"

using namespace std;

int main() {
    FileSystem filesystem;
    try {
        filesystem.LoadDirectory(const_cast<char *>("C:\\cache_530"));
    } catch (const std::exception &exc) {
        printf("Error loading directory: %s\n", exc.what());
        return 0;
    }

    printf("Number of indices: %d\n", filesystem.GetIndexCount());
    printf("Has index 15: %d, has 16: %d\n", filesystem.HasIndex(15), filesystem.HasIndex(16));
    return 0;
}