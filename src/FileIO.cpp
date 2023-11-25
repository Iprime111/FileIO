#include <string.h>
#include <stdlib.h>
#include <sys/io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include "FileIO.h"
#include "CustomAssert.h"
#include "Logger.h"
#include "TextTypes.h"

static size_t LineLength (const char *line);
static ssize_t GetFileSize (const char *filename);

bool CreateFileBuffer (FileBuffer *buffer, const char *filename) {
    PushLog (3);

    buffer->buffer_size  = GetFileSize (filename);
    if (buffer->buffer_size <= 0) {
        RETURN false;
    }

    buffer->buffer = (char *) calloc ((size_t) buffer->buffer_size + 1, sizeof (char));
    if (buffer->buffer == NULL) {
        RETURN false;
    }

    RETURN true;
}

void DestroyFileBuffer (FileBuffer *buffer) {
    PushLog (3);

    free (buffer->buffer);

    RETURN;
}

static ssize_t GetFileSize (const char *filename) {
    PushLog (3);

    struct stat file_statistics;

    if (stat(filename, &file_statistics) != 0) {
        RETURN -1;
    }

    RETURN file_statistics.st_size;
}

bool ReadFile (const char *filename, FileBuffer *buffer) {
    PushLog (3);

    custom_assert (filename,                pointer_is_null,     false);
    custom_assert (buffer,                  pointer_is_null,     false);
    custom_assert (buffer->buffer_size > 0, invalid_value,       false);

    int file_descriptor = -1;

    if ((file_descriptor = open (filename, O_RDONLY)) == -1) {
        RETURN false;
    }

    if (read (file_descriptor, buffer->buffer, (size_t) buffer->buffer_size) != buffer->buffer_size) {
        RETURN false;
    }

    if (close (file_descriptor) != 0) {
        RETURN false;
    }

    RETURN true;
}

bool ReadFileLines (const char *filename, FileBuffer *file_buffer, TextBuffer *text_buffer, char delim) {
    PushLog (2);

    custom_assert (file_buffer->buffer_size > 0, invalid_value,   false);
    custom_assert (file_buffer->buffer,          pointer_is_null, false);

    if (!ReadFile (filename,file_buffer)) {
        RETURN false;
    }

    text_buffer->line_count = SplitBufferToLines (file_buffer->buffer, NULL, delim);
    text_buffer->lines = (TextLine *) calloc (text_buffer->line_count, sizeof (TextLine));

    SplitBufferToLines (file_buffer->buffer, text_buffer, delim);

    RETURN true;
}

size_t SplitBufferToLines (char *file_buffer, TextBuffer *text_buffer, char delim) {
    PushLog (3);

    custom_assert (file_buffer, pointer_is_null, 0);

    char *current_symbol  = file_buffer;
    char *previous_symbol = NULL;
    size_t current_line_index  = 0;

    while (*current_symbol != '\0') {
        if (text_buffer != NULL) {
            text_buffer->lines[current_line_index].pointer = current_symbol;
            if (previous_symbol != NULL) {
                text_buffer->lines[current_line_index - 1].length = (size_t) (current_symbol - previous_symbol - 1);
            }
            previous_symbol = current_symbol;
        }

        if ((current_symbol = strchr (current_symbol, delim)) == NULL)
            break;

        current_line_index++;
        current_symbol++;
    }

    if (text_buffer != NULL) {
        text_buffer->lines[current_line_index - 1].length = LineLength (previous_symbol);
        text_buffer->line_count = current_line_index;
    }


    RETURN current_line_index;

}

bool ChangeNewLinesToZeroes (TextBuffer *buffer) {
    PushLog (3);

    custom_assert (buffer, pointer_is_null, false);

    for (size_t lineIndex = 0; lineIndex < buffer->line_count; lineIndex++) {
        char *lastElement = buffer->lines [lineIndex].pointer + buffer->lines [lineIndex].length;

        if (!lastElement) {
            RETURN false;
        }

        *(buffer->lines [lineIndex].pointer + buffer->lines [lineIndex].length) = '\0';
    }

    RETURN true;
}

bool WriteLine(int file_descriptor, TextLine *line) {
    PushLog (3);

    custom_assert (line,                pointer_is_null, false);
    custom_assert (file_descriptor > 0, invalid_value,   false);

    if (line->length == 0) {
        RETURN true;
    }

    WriteBuffer (file_descriptor, line->pointer, (ssize_t) line->length);
    WriteBuffer (file_descriptor, "\n", 1);

    RETURN true;
}

bool WriteLines (int file_descriptor, TextBuffer *lines) {
    PushLog (3);

    custom_assert (lines,               pointer_is_null, false);
    custom_assert (file_descriptor > 0, invalid_value,   false);

    for (size_t line = 0; line < lines->line_count; line++) {
        if (!WriteLine (file_descriptor, lines->lines + line)) {
            RETURN false;
        }
    }

    RETURN true;
}

bool WriteBuffer (int file_descriptor, const char *buffer, ssize_t buffer_size) {
    PushLog (3);

    custom_assert (buffer,              pointer_is_null, false);
    custom_assert (file_descriptor > 0, invalid_value,   false);
    custom_assert (buffer_size >= 0,    invalid_value,   false);

    if (write (file_descriptor, buffer, (size_t)buffer_size) != buffer_size) {
        RETURN false;
    }

    RETURN true;
}

int OpenFileWrite (const char *filename) {
    PushLog (3);

    custom_assert (filename, pointer_is_null, -1);

    int file_descriptor = open (filename, O_WRONLY | O_RDONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    RETURN file_descriptor;
}

void CloseFile (int file_descriptor) {
    PushLog (4);

    if (close (file_descriptor) != 0) {
        custom_assert (0, file_close_error, (void) 0);
    }

    RETURN;
}

static size_t LineLength (const char *line) {
    PushLog (4);

    custom_assert (line != NULL, pointer_is_null, 0);

    size_t length = 0;

    while (line [length] != '\0' && line [length] != '\n') {
        length++;
    }

    RETURN length;
}

int IsRegularFile (const char *path) {
    struct stat stats;

    if (stat(path, &stats) < 0)
        return 0;

    return S_ISREG(stats.st_mode);
}
