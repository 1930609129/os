//
// Created by xjz on 2022/5/25.
//

#include "stream.h"

#define dir_flag 1
#define file_flag 0
static void os_path(file);
static _Bool os_open(file_mode);
static _Bool os_write(file_stream);
static file_stream os_read();
static file_stream os_readline();
static _Bool os_remove();
static _Bool os_rename(file);
static _Bool os_close();
static dir os_dir();
static stream2 os_dir_name();
static stream2 os_dir_dir();
static stream2 os_dir_file();
static _Bool os_dir_open();
static _Bool os_dir_close();
stream os = {
    .fp = (void*)0,
    .file = (void*)0,
    .pdir = (void*)0,
    .path = os_path,
    .open = os_open,
    .write = os_write,
    .read = os_read,
    .readline = os_readline,
    .remove = os_remove,
    .rename = os_rename,
    .dir = os_dir,
    .close = os_close
};

static void os_path(file path)
{
    os.file = path;
}

static _Bool os_open(file_mode mode)
{
    if(os.fp)
        fclose(os.fp);
    os.fp = fopen(os.file,mode);
    if(os.fp)
        return 1;
    return 0;
}
static _Bool os_write(file_stream data)
{
    if(os.fp==NULL) return 0;
    return fwrite(data,strlen(data),1,os.fp);
}
static file_stream os_read()
{
    if(os.fp==NULL) return NULL;
    fseek(os.fp,0,SEEK_END);
    long len = ftell(os.fp);
    file_stream res = malloc(len+1);
    memset(res+len,0,1);
    rewind(os.fp);
    fread(res,len,1,os.fp);
    return res;
}

static file_stream os_readline()
{
    if(os.fp==NULL) return NULL;
    static char buffer[stream_buffer];
//    memset(buffer,0,stream_buffer);
    fgets(buffer,stream_buffer,os.fp);
    return buffer;
}

static _Bool os_remove()
{
    if(os.file==NULL) return 0;
    return remove(os.file) == 0;
}
static _Bool os_rename(file name)
{
    if(os.file==NULL) return 0;
    return rename(os.file,name)==0;
}
static _Bool os_close()
{
    if(os.fp==NULL) return 0;
    return fclose(os.fp)==0;
}

static dir os_dir()
{
    static dir res = {
        .dir = os_dir_dir,
        .file = os_dir_file,
        .name = os_dir_name,
        .close = os_dir_close,
        .open = os_dir_open
    };
    return res;
}

static _Bool os_dir_open()
{
    if(os.pdir)
        closedir(os.pdir);
    os.pdir = opendir(os.file);
    if(os.pdir)
        return 1;
    return 0;
}

static stream2 os_dir_name()
{
    rewinddir(os.pdir);
    static struct dirent *ptr;
    stream2 name;
    int n = 0;
    while ((ptr= readdir(os.pdir)))
        n++;
    name = malloc((n+1)*8);
    name[n] = NULL;
    rewinddir(os.pdir);
    n = 0;
    while ((ptr= readdir(os.pdir)))
    {
        name[n] = malloc(ptr->d_namlen+1);
        memset(name[n]+ptr->d_namlen,0,1);
        memcpy(name[n],ptr->d_name,ptr->d_namlen);
        n++;
    }
    return name;
}

static stream2 os_dir_or_file(char flag)
{
    rewinddir(os.pdir);
    static struct dirent *ptr;
    stream2 name = NULL;
    struct stat s_buf;
    int n = 0;
    static char file_path[260];
    strcpy(file_path,os.file);
    strcat(file_path,"/");
    int len = strlen(file_path);
    while ((ptr= readdir(os.pdir)))
    {
        memcpy(file_path+len,ptr->d_name,ptr->d_namlen);
        stat(file_path,&s_buf);
        memset(file_path+len,0,ptr->d_namlen);
        if(S_ISREG(s_buf.st_mode)&&flag==dir_flag)
            continue;
        if(S_ISDIR(s_buf.st_mode)&&flag==file_flag)
            continue;
        n++;
        name = realloc(name,n*8);
        name[n-1] = malloc(ptr->d_namlen+1);
        memset(name[n-1]+ptr->d_namlen,0,1);
        memcpy(name[n-1],ptr->d_name,ptr->d_namlen);
    }
    name = realloc(name,(n+1)*8);
    name[n] = NULL;
    return name;
}

//    rewinddir(os.pdir);
//    static struct dirent *ptr;
//    stream2 name = NULL;
//    struct stat s_buf;
//    int n = 0;
//    static char file_path[260];
//    strcpy(file_path,os.file);
//    strcat(file_path,"/");
//    int len = strlen(file_path);
//    while ((ptr= readdir(os.pdir)))
//    {
//        memcpy(file_path+len,ptr->d_name,ptr->d_namlen);
//        stat(file_path,&s_buf);
//        if(S_ISDIR(s_buf.st_mode))
//        {
//            n++;
//            name = realloc(name,n*8);
//            name[n-1] = malloc(ptr->d_namlen+1);
//            memset(name[n-1]+ptr->d_namlen,0,1);
//            memcpy(name[n-1],ptr->d_name,ptr->d_namlen);
//        }
//        memset(file_path+len,0,ptr->d_namlen);
//    }
//    name = realloc(name,(n+1)*8);
//    name[n] = NULL;
//    return name;

static stream2 os_dir_dir()
{
    return os_dir_or_file(dir_flag);
}
static stream2 os_dir_file()
{
    return os_dir_or_file(file_flag);
}
static _Bool os_dir_close()
{
    return closedir(os.pdir) == 0;
}