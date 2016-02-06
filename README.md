# librsfs
C++ library for the RuneScape file store

# Filesystem structure
The RuneScape file system consists of a three-level hierarchy, which can be illustrated as follows:

 - Filesystem
   - Index
     - Folder
         - File

In this hierarchy, the `filesystem` is simply put the directory containing the `main_file_cache.dat2` file 
(which contains the actual file data). The file system also has files in that directory ending with `.idx*` which 
ranges from `.idx0` (which would be a simple `index` with ID 0) to `.idx255` (which is a special one; index 
index 255 contains the root reference table). These indices contain `folder` entries. These folder entries can 
either be plain (compressed) data themselves, or contain individual files. 

# Building

## librsfs
Since 0.1.0, building has become significantly easier because the bzip2 and gzip dependencies are now included in the project and compiled on the go. This means that this project no longer has any required external dependencies, besides the optional LZMA which is only required on very recent versions of the RuneScape 3 file store.

First of all, make a new directory. Out-of-source builds are so much nicer (and you don't get all the Git clobbing!).

```
mkdir librsfs-build
cd librsfs-build
```

Now invoke the CMake command with the defines to ensure everything's easy to find:

```
cmake ../librsfs -G "Visual Studio 12 Win64"
```

The above command builds for an x64 release. If you are looking for the 32-bit build, leave out Win64 when passing the generator to CMake. If CMake can find the LZMA libraries on your system, it will include them in the build and librsfs will then support LZMA-compressed archives. If not, no problem. Almost all file systems work without LZMA.

This will create a solution file, a few project files and the other good stuff for you ready to open & build.


# Usage

## Loading from a folder

```c++
try {
	FileSystem filesystem;
	filesystem.load_index(your_directory);
} catch (const std::exception &exc) {
	printf("Error loading directory: %s\n", exc.what());
}
```

## Checking the number of indices

```c++
auto count = filesystem.get_index_count();
```

## Checking the (assumed) number of entries in an index

This does not necessarily check the actual number of 'valid' entries, but guesses the number based on the highest entry ID. Since the entries could look like `1, 4, 77, 78, 79` this does not accurately match the actual number of entries containing data. The method would, using above data, return 79, while it actually only has 5 entries.
```c++
auto highest_entry_id = filesystem.get_index(0).GetEntryCount();
```

## Getting information on an entry

To acquire information about an entry (its starting offset and its size), use the method shown below.
```c++
auto model_folder_1234 = filesystem.get_index(7).GetFolderInfo(1234);
auto size = model_folder_0.GetSize(); // Full size of this entry (in compressed form, that is)
auto offset = model_folder_0.GetOffset(); // Absolute file offset where the data starts
```

## Reading data from an entry

Reading data is easy to do. Note that this data may be compressed!

```c++
vector<char> entry_data;
// see above on how to get model_folder_1234 defined
auto num_read = filesystem.read(model_folder_1234, entry_data);
```

## Decompressing an entry

Some entries are compressed. Actually, most of them are. They are either compressed with gzip or with bzip2, or LZMA in the newest RS3. Librsfs supports gzip, bzip2 and, when found on the build path, lzma (which is used in the later versions of RuneScape 3).

```c++
vector<char> decompressed;
int num = Compression::Decompress(entry_data, decompressed);
```

## Getting compression information

If you're simply curious on the compression type or the inflated/deflated sizes, you can do that with:
```c++
CompressionInfo compression_info(entry_data);
auto compression_type = compression_info.GetCompressionType();
auto decompressed_size = compression_info.GetDecompressedSize();
```
