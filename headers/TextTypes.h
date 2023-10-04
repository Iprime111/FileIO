#ifndef TEXT_TYPES_H_
#define TEXT_TYPES_H_

#include <sys/types.h>
struct FILE_BUFFER{
    char *buffer;
    ssize_t buffer_size;
};

struct TEXT_LINE{
    char *pointer;
    size_t length;
};

struct TEXT_BUFFER{
    TEXT_LINE *lines;
    size_t line_count;
};

#endif
