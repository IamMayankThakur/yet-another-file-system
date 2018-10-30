# file-system-os

File System Project for UE16CS302.

## Initialization and set up

* Clone repository using `git clone https://TheYoungMonk@bitbucket.org/TheYoungMonk/file-system-os.git`
* Switch to your branch and commit.
* Install fuse using `sudo apt-get update && sudo apt-get install libfuse-dev` and `sudo apt install pkg-config`

## Maintainance and development

* Compile file using ``gcc -Wall  -o opfile_name file_name.c `pkg-config fuse --cflags --libs` ``
* Run file using `./opfile_name /path_to_/fuse_files (The directory fuse_files must already exist)
* `cd FuseFiles` to use the commands on the new file system.
* To check the system calls and debug logs while the file system is mounted, use `./opfile_name -f /path_to_fuse_files`
* To unmount the file system `sudo umount /path_to/fuse_files	`
* All function declarations and structures are added in `file_system.h`
* Use readable variable and method names which follow the `underscore_case` notation, not camelCase.

#### * Work only on your branch.
#### * Do not merge anything with the master branch.

## Basic git commands

* ``git checkout -b `branch_name` `` to create a new branch, do this before your first commit. (I have already used the branch name `mayank_develop`)
* ``git checkout `branch_name` `` to switch to another already existing branch.
* `` git pull origin `branch_name` `` to get the latest version of that branch.

#### Following commands for commiting your work.

* `git add .`
* ``git commit -m `commit_message` ``
* `` git push origin `branch_name` ``
