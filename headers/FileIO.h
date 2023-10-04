#ifndef FILEIO_H_
#define FILEIO_H_

#include "TextTypes.h"

void CreateFileBuffer  (FileBuffer *buffer, char *filename);
void DestroyFileBuffer (FileBuffer *buffer);

void ReadFile       (const char *filename, FileBuffer *buffer);
void ReadFileLines  (const char *filename, FileBuffer *file_buffer, textBuffer *text_buffer);

size_t SplitBufferToLines (char *file_buffer, textBuffer *text_buffer);

int OpenFileWrite (const char *filename);
void CloseFile (int file_descriptor);

void WriteLine   (int file_descriptor, TextLine *line);
void WriteLines  (int file_descriptor, textBuffer *lines);
void WriteBuffer (int file_descriptor, const char *buffer, ssize_t buffer_size);

#endif
