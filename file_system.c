#include "file_system.h"

int offset_no_next = 1;
int data_offset_next = 1;
static struct fuse_operations operations = {
    // .mkdir= mkdir_f,
    // .readdir = readdir_f,
    // .getattr = getattr_f,
    // .rmdir = rmdir_f,
    // .rename = mv_f,
    // .open = open_f,
    // .read = read_f,
    // .write = write_f,
    // .truncate = truncate_f,
    // .mknod = mknod_f,
    // .chmod = chmod_f,
    // .unlink = unlink_f,
};



int main(int argc,char *argv[])
{    
    fuse_main( argc, argv, &operations, NULL );
    return 0;
}

