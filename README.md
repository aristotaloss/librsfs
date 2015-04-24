# librsfs
C++ library for the RuneScape file store

# Filesystem structure
The RuneScape file system consists of a three-level hierarchy, which can be illustrated as follows:

 - Filesystem
   - Directory
     - Folder
         - File

In this hierarchy, the `filesystem` is simply put the directory containing the `main_file_cache.dat2` file 
(which contains the actual file data). The file system also has files in that directory ending with `.idx*` which 
ranges from `.idx0` (which would be a simple `directory` with ID 0) to `.idx255` (which is a special one; directory 
index 255 contains the root reference table). These directories contain `folder` entries. These folder entries can 
either be plain (compressed) data themselves, or contain individual files. 