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

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
 DIR *dp;
 struct dirent *de;
 char fpath[1000];
 sprintf(fpath,"%s%s",dirpath,path);
 (void) offset;
 (void) fi;
 dp = opendir(fpath);
 if (dp == NULL)
 	return -errno;
 while ((de = readdir(dp)) != NULL) {
 	struct stat st;
 	memset(&st, 0, sizeof(st));
 	st.st_ino = de->d_ino;
 	st.st_mode = de->d_type << 12;
 	if (filler(buf, de->d_name, &st, 0))
 		break;
 }
 closedir(dp);
 return 0;
}

int flags(const char *namafile)

{
 int x = strlen(namafile);
 char file[100];
 strcpy(file,namafile+x-4);
 if(strcmp(file,".txt")==0||strcmp(file,".doc")==0||strcmp(file,".pdf")==0)
 return 1;
 else
 return 0;
}



static int xmp_open(const char *path, struct fuse_file_info *fi)
{
 int res;
 char fpath[1000];
 sprintf(fpath,"%s%s",dirpath,path);
 if(flags (fpath))
 {
 	char command[100];
 	sprintf(command,"zenity --error --text='Terjadi kesalahan! File berisi konten berbahaya.'");
 	system(command);
 	char ch, source_file[1000], target_file[1000];
 FILE *source, *target;
 sprintf(source_file,"%s",fpath);
 source = fopen(source_file, "r");
 sprintf(target_file,"%s.ditandai",fpath);
 remove(source_file);
 int ada;
 ada = access(target_file,F_OK);
 if(ada==0)
 {
 remove(target_file);
 }
 target = fopen(target_file, "w");
 while( ( ch = fgetc(source) ) != EOF ) fputc(ch, target);
 sprintf(command,"chmod 000 '%s.ditandai'",fpath);
 system(command);
 fclose(source);
 fclose(target);
 res = open(fpath, fi->flags);

 if (res == -1)
 return -errno; }

 close(res);
  return 0;

 

}

static int xmp_read(const char *path, char *buf,size_t size, off_t offset, struct fuse_file_info *fi)

{
 int fd;
 int res;
 char fpath[1000];
 sprintf(fpath,"%s%s",dirpath,path);
 (void) fi;
 fd = open(fpath, O_RDONLY);
 if (fd == -1)
 return -errno;
 res = pread(fd, buf, size, offset);
 if (res == -1)
 res = -errno;
 close(fd);
 return res;
}

static struct fuse_operations xmp_oper = {
 .getattr = xmp_getattr,
 .readdir = xmp_readdir,
 .open = xmp_open,
 .read = xmp_read,
};

int main(int argc, char *argv[])
{
 fuse_main(argc, argv, &xmp_oper, NULL);
 return 0;
}
