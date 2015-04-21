#include <iostream>
#include "FileSystem.h"

using namespace std;

int main() {
    FileSystem fs;
    FSResult result = fs.loadDirectory(const_cast<char*>("C:\\Users\\Bart\\Documents\\osrs-server\\data\\filestore"));

    if (result == FileSystem::E_INVALID_DIRECTORY) {
        printf("Could not open directory.");
        return result;
    }

    printf("FS result: %d\n", result);
    return 0;
}