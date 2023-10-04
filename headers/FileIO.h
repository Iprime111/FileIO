#ifndef FILEIO_H_
#define FILEIO_H_

#include "TextTypes.h"

void create_file_buffer  (FILE_BUFFER *buffer, char *filename);
void destroy_file_buffer (FILE_BUFFER *buffer);

ssize_t get_file_size (const char *filename);
void read_file        (const char *filename, FILE_BUFFER *buffer);
void read_file_lines  (const char *filename, FILE_BUFFER *file_buffer, TEXT_BUFFER *text_buffer);

size_t split_buffer_to_lines (char *file_buffer, TEXT_BUFFER *text_buffer);

int open_file_write (const char *filename);
void close_file (int file_descriptor);

void write_line(int file_descriptor, TEXT_LINE *line);
void write_lines (int file_descriptor, TEXT_BUFFER *lines);
void write_buffer (int file_descriptor, const char *buffer, ssize_t buffer_size);

#endif
