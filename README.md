# yet-another-file-system

File System Project for UE16CS302.

## Initialization and set up

* Clone repository
* `cd yet-another-file-system`
* Switch to your branch and commit using ``git checkout -b `branch_name` `` and commit commands given below.
* Install fuse using `sudo apt-get update && sudo apt-get install libfuse-dev` and `sudo apt install pkg-config`

## Maintainance and development

* Compile file using ``gcc -Wall  -o opfile_name file_name.c `pkg-config fuse --cflags --libs` ``
* Run file using `./opfile_name /path_to_/fuse_files` (The directory fuse_files must already exist)
* `cd FuseFiles` to use the commands on the new file system.
* To check the system calls and debug logs while the file system is mounted, use `./opfile_name -f /path_to_fuse_files`
* To unmount the file system `sudo umount /path_to/fuse_files	`
* All function declarations and structures are added in `file_system.h`


