//
// Created by xjz on 2022/5/25.
//

#ifndef SDXX_STREAM_H
#define SDXX_STREAM_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#define stream_buffer 256
typedef void * file;
typedef void * file_stream;
typedef void * file_mode;
typedef void **stream2;
#define free_stream2(p) { \
    int n = 0;            \
    while(p[n])           \
    {                     \
        free(p[n]);\
        n++;\
    }                     \
    free(p[n]);           \
    free(p);\
}
typedef struct __attribute__((packed)){
    stream2 (*name)();
    stream2 (*dir)();
    stream2 (*file)();
    _Bool (*open)();
    _Bool (*close)();
}dir;

typedef struct __attribute__((packed)) {
    FILE *fp;
    file file;
    DIR *pdir;
    dir (*dir)();
    void (*path)(file);
    _Bool (*open)(file_mode mode);
    _Bool (*write)(file_stream);
    file_stream (*read)();
    file_stream (*readline)();
    _Bool (*remove)();
    _Bool (*rename)(file);
    _Bool (*close)();
}stream;

extern stream os;

#endif //SDXX_STREAM_H
