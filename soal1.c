#define FUSE_USE_VERSION 26
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef linux
#define _XOPEN_SOURCE 700
#endif
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#ifdef HAVE_SETXATTR
#include <sys/xattr.h>
#endif
#include<stdlib.h>
#include <sys/types.h>
static const char *dirpath = "/home/dewisekar/Documents";

static int xmp_getattr(const char *path, struct stat *stbuf)

{
 int res;
 char fpath[1000];
 sprintf(fpath,"%s%s",dirpath,path);
 res = lstat(fpath, stbuf);
 if (res == -1)
	 return -errno;
 return 0;
}

static struct fuse_operations xmp_oper = {
 .getattr = xmp_getattr,
 .readdir = xmp_readdir,
 .open = xmp_open,
 .read = xmp_read,
};