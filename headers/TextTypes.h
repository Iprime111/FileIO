#ifndef TEXT_TYPES_H_
#define TEXT_TYPES_H_

#include <sys/types.h>
struct FileBuffer {
    char *buffer;
    ssize_t buffer_size;
};

struct TextLine {
    char *pointer;
    size_t length;
};

struct TextBuffer {
    TextLine *lines;
    size_t line_count;
};

#endif
