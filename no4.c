
    #include <fuse.h>

    #include <stdio.h>

    #include <string.h>

    #include <syslog.h>

    #include <stdlib.h>

    #include <unistd.h>

    #include <fcntl.h>

    #include <dirent.h>

    #include <errno.h>

    #include <sys/statfs.h>

    #include <sys/types.h>

    #include <sys/stat.h>



    static const char *dirpath = "/home/taufiq/Downloads";



    static int xmp_getattr(const char *path, struct stat *stbuf)

    {

     int res;

     char fpath[1000];

     sprintf(fpath,"%s%s",dirpath,path);

     res = lstat(fpath, stbuf);



     if(res == -1)

     {

      return -errno;

     }



     return 0;

    }



    static int xmp_chmod(const char *path, mode_t mode)

    {

        int res;

        char fpath[1000];

        char direktori[] = "/home/taufiq/Downloads/simpanan";

        //sprintf(fpath,"%s%s", dirpath, path);

        sprintf(fpath,"%s%s", direktori, path);

        res = chmod(fpath, mode);

        if(res == -1)

            return -errno;



        return 0;

    }



    static int xmp_chown(const char *path, uid_t uid, gid_t gid)

    {

        int res;

        char fpath[1000];

        sprintf(fpath,"%s%s", dirpath, path);

        res = lchown(fpath, uid, gid);

        if(res == -1)

            return -errno;



        return 0;

    }



    static int xmp_getdir(const char *path, fuse_dirh_t h, fuse_dirfil_t filler)

    {

     char fpath[1000];

     if(strcmp(path,"/") == 0)

     {

      path=dirpath;

      sprintf(fpath,"%s",path);

     }

     else sprintf(fpath, "%s%s",dirpath,path);

     int res = 0;

     DIR *dp;

     struct dirent *de;

     dp = opendir(fpath);

     if(dp==NULL)

     {

      return -errno;

     }

     while((de = readdir(dp))!=NULL)

     {

      res = filler(h, de->d_name, de->d_type);

      if(res!=0) break;

     }

     closedir(dp);

     return res;

    }



    static int xmp_mkdir(const char *path,mode_t mode)

    {

        int res;

        char fpath[1000];

        sprintf(fpath,"%s%s",dirpath,path);

        res = mkdir (fpath,mode);

        if(res == -1)

            return -errno;

        return 0;

    }



    static int xmp_symlink(const char *from, const char *to)

    {

        int res;

     char ffrom[1000];

     char fto[1000];

     sprintf(ffrom,"%s%s",dirpath,from);

     sprintf(fto,"%s%s",dirpath,to);

        res = symlink(ffrom, fto);

        if(res == -1)

            return -errno;



        return 0;

    }



    static int xmp_unlink(const char *path)

    {

        int res;

     char fpath[1000];

     sprintf(fpath,"%s%s", dirpath, path);

        res = unlink(fpath);

        if(res == -1)

            return -errno;



        return 0;

    }



    static int xmp_truncate(const char *path, off_t size)

    {

        int res;

         char fpath[1000];

     sprintf(fpath,"%s%s", dirpath, path);

        res = truncate(fpath, size);

        if(res == -1)

            return -errno;



        return 0;

    }



    static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)

    {

        int res;

     char fpath[1000];

     sprintf(fpath,"%s%s", dirpath, path);

        res = mknod(fpath, mode, rdev);

        if(res == -1)

            return -errno;



        return 0;

    }



    static int xmp_open(const char *path, int flags)

    {

        int res;

        int counter=0;

        char fpath[1000];

        char path2[1000];

        char path3[1000];

        sprintf(fpath,"%s%s", dirpath, path);

        sprintf(path2,"%s%s","/home/taufiq/Downloads",path);

        int i,count,idx=4;
        char temp[5];


        for(i=0;i<strlen(path);i++)

        {

            if(path[i]=='/')

            {

                counter++;

            }

        }



        if(counter==2)

        {           
            sprintf(path3,"%s%s","chmod 444 ",path2);
            system(path3);

            for(i=strlen(fpath)-1,count=1;count<=4;i--,count++){
                temp[idx--]=fpath[i];
            }
            
            if(strcmp(temp,".copy")==0){
                char temp[]="zenity --info --text=\"";
                strcat(temp,"File yang anda buka adalah file hasil salinan. File tidak bisa diubah maupun disalin kembali!");
                strcat(temp,"\"");
                system(temp);
                return -1;
            }
        }
        else{
            res = open(fpath, flags);

            if(res == -1)
                return -errno;

            close(res);
        }

        return 0;

    }



    static int xmp_read(const char *path, char *buf, size_t size, off_t offset)

    {

    int x,harga,temp;
    int res = 0;
    int fd = 0 ;
    char fpath[1000];
    char tukar[1000];
    
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);

    sprintf(tukar,"%s%s","/home/taufiq/Downloads/simpanan",path);

    temp=3;
    char curr[15];
    for(x=strlen(tukar)-1,harga=1;harga<=5;x--,harga++)
    {
        curr[temp--]=tukar[x];
    }
    
    if(strcmp(curr,".copy")==0)
    {
        char perintah[2002];
        sprintf(perintah,"chmod 000 %s.ditandai",tukar);
        system(perintah);
        system("zenity --error --text=\"File yang anda buka adalah file hasil salinan. File tidak bisa diubah maupun disalin kembali!\n\" --title=\"Warning!\"");
        return -errno;
    }
    else
    {
        fd = open(fpath, O_RDONLY);
        if (fd != -1)
        {}
        else
        return -errno;
        res = pread(fd, buf, size, offset);
        if (res != -1)
        {}
        else
        res = -errno;
        close(fd);
        return res;
    }

    }



    static int xmp_write(const char *path, const char *buf, size_t size,

                         off_t offset)

    {

        int fd;

        int res;

     char fpath[1000];

     sprintf(fpath,"%s%s", dirpath, path);

        fd = open(fpath, O_WRONLY);

        if(fd == -1)

            return -errno;



        res = pwrite(fd, buf, size, offset);

        if(res == -1)

            res = -errno;



        close(fd);

        return res;

    }



    static int xmp_rename(const char *from, const char *to)

    {

        int res;

        char ffrom[1000];

        char fto[1000];

        system("mkdir /home/taufiq/Downloads/simpanan");



        char direktori[] = "/home/taufiq/Downloads/simpanan";



        sprintf(ffrom,"%s%s",dirpath,from);

        sprintf(fto,"%s%s.copy",direktori,to);

        res = rename(ffrom, fto);



        if(res == -1)

        return -errno;



        return 0;

    }



    static struct fuse_operations xmp_oper =

    {

     .getattr = xmp_getattr,

     .getdir = xmp_getdir,

     .mknod = xmp_mknod,

     .mkdir = xmp_mkdir,

     .symlink = xmp_symlink,

     .unlink = xmp_unlink,

     .rename = xmp_rename,


     .chmod = xmp_chmod,

     .chown = xmp_chown,

     .truncate = xmp_truncate,

     .open = xmp_open,

     .read = xmp_read,

     .write = xmp_write,

    };



    int main(int argc, char *argv[])

    {

     return fuse_main(argc, argv, &xmp_oper);

    }

