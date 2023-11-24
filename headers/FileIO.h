#ifndef FILEIO_H_
#define FILEIO_H_

#include "TextTypes.h"

bool CreateFileBuffer  (FileBuffer *buffer, const char *filename);
void DestroyFileBuffer (FileBuffer *buffer);

bool ReadFile       (const char *filename, FileBuffer *buffer);
bool ReadFileLines  (const char *filename, FileBuffer *file_buffer, TextBuffer *text_buffer, char delim='\n');

size_t SplitBufferToLines (char *file_buffer, TextBuffer *text_buffer, char delim='\n');

bool ChangeNewLinesToZeroes (TextBuffer *buffer);

int OpenFileWrite (const char *filename);
void CloseFile (int file_descriptor);

bool WriteLine   (int file_descriptor, TextLine *line);
bool WriteLines  (int file_descriptor, TextBuffer *lines);
bool WriteBuffer (int file_descriptor, const char *buffer, ssize_t buffer_size);

int IsRegularFile (const char *path);

#endif
