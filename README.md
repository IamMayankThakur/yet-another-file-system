# file-system-os

File System Project for UE16CS302.

## Initialise

* Clone repository using `git clone 
* Install fuse using `sudo apt-get update && sudo apt-get install libfuse-dev` and `sudo apt install pkg-config`
* Compile file using ``gcc -Wall  -o opfile_name file_name.c `pkg-config fuse --cflags --libs` ``
* Run file using `./opfile_name /path_to_/FuseFiles` (The directory FuseFiles must already exist)
* `cd FuseFiles` to use the commands on the new file system.
* To check the system calls and debug logs while the file system is mounted, use `./opfile_name -f /path_to_Fuse_File`
* To unmount the file system `sudo umount /path_to/Fuse_Files	`
* All function declarations and structures are added in `file_system.h`
* 