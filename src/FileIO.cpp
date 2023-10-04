#include <string.h>
#include <stdlib.h>
#include <sys/io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include "FileIO.h"
#include "CustomAssert.h"

size_t line_len (const char *line);

void create_file_buffer (FILE_BUFFER *buffer, char *filename){
    PushLog (3);

    buffer->buffer_size  = get_file_size (filename);
    custom_assert  (buffer->buffer_size > 0, invalid_value, (void)0);

    buffer->buffer = (char *) calloc ((size_t) buffer->buffer_size + 1, sizeof (char));
    custom_assert (buffer->buffer != NULL, pointer_is_null, (void)0);

    RETURN;
}

void destroy_file_buffer (FILE_BUFFER *buffer){
    PushLog (3);

    free (buffer->buffer);

    RETURN;
}

ssize_t get_file_size (const char *filename){
    PushLog (3);

    struct stat file_statistics;

    custom_assert (stat(filename, &file_statistics) == 0, cannot_open_file, 0);

    RETURN file_statistics.st_size;
}

void read_file (const char *filename, FILE_BUFFER *buffer){
    PushLog (3);

    custom_assert (filename != NULL,        pointer_is_null,     (void)0);
    custom_assert (buffer   != NULL,        pointer_is_null,     (void)0);
    custom_assert (buffer->buffer_size > 0, invalid_value,       (void)0);

    int file_descriptor = -1;

    custom_assert ((file_descriptor = open (filename, O_RDONLY)) != -1, cannot_open_file, (void)0);

    custom_assert (read (file_descriptor, buffer->buffer, (size_t) buffer->buffer_size) ==
            buffer->buffer_size, cannot_open_file, (void)0);

    custom_assert (close (file_descriptor) == 0, file_close_error, (void) 0);

    RETURN;
}

void read_file_lines (const char *filename, FILE_BUFFER *file_buffer, TEXT_BUFFER *text_buffer){
    PushLog (2);

    custom_assert (file_buffer->buffer_size > 0, invalid_value,   (void)0);
    custom_assert (file_buffer->buffer != NULL,  pointer_is_null, (void)0);

    read_file (filename,file_buffer);

    text_buffer->line_count = split_buffer_to_lines (file_buffer->buffer, NULL);
    text_buffer->lines = (TEXT_LINE *) calloc (text_buffer->line_count, sizeof (TEXT_LINE));

    split_buffer_to_lines (file_buffer->buffer, text_buffer);

    RETURN;
}

size_t split_buffer_to_lines (char *file_buffer, TEXT_BUFFER *text_buffer){
    PushLog (3);

    custom_assert (file_buffer != NULL, pointer_is_null, 0);

    char *current_symbol  = file_buffer;
    char *previous_symbol = NULL;
    size_t current_line_index  = 0;

    while (*current_symbol != '\0'){
        if (text_buffer != NULL){
            text_buffer->lines[current_line_index].pointer = current_symbol;
            if (previous_symbol != NULL){
                text_buffer->lines[current_line_index - 1].length = (size_t) (current_symbol - previous_symbol - 1);
            }
            previous_symbol = current_symbol;
        }

        if ((current_symbol = strchr (current_symbol, '\n')) == NULL)
            break;

        current_line_index++;
        current_symbol++;
    }

    if (text_buffer != NULL){
        text_buffer->lines[current_line_index - 1].length = line_len (previous_symbol);
        text_buffer->line_count = current_line_index;
    }


    RETURN current_line_index;

}

void write_line(int file_descriptor, TEXT_LINE *line){
    PushLog (3);

    custom_assert (line != NULL,        pointer_is_null, (void)0);
    custom_assert (file_descriptor > 0, invalid_value,   (void)0);

    if (line->length == 0){
        RETURN;
    }

    write_buffer (file_descriptor, line->pointer, (ssize_t) line->length);
    write_buffer (file_descriptor, "\n", 1);

    RETURN;
}

void write_lines (int file_descriptor, TEXT_BUFFER *lines){
    PushLog (3);

    custom_assert (lines != NULL,       pointer_is_null, (void)0);
    custom_assert (file_descriptor > 0, invalid_value,   (void)0);

    for (size_t line = 0; line < lines->line_count; line++){
        write_line (file_descriptor, lines->lines + line);
    }

    RETURN;
}

void write_buffer (int file_descriptor, const char *buffer, ssize_t buffer_size){
    PushLog (3);

    custom_assert (buffer != NULL,      pointer_is_null, (void)0);
    custom_assert (file_descriptor > 0, invalid_value,   (void)0);
    custom_assert (buffer_size >= 0,    invalid_value,   (void)0);

    custom_assert (write (file_descriptor, buffer, (size_t)buffer_size) == buffer_size, cannot_open_file, (void)0);

    RETURN;
}

int open_file_write (const char *filename){
    PushLog (3);

    int file_descriptor = -1;

    custom_assert ((file_descriptor = open (filename, O_WRONLY | O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR)) != -1, cannot_open_file, -1);

    RETURN file_descriptor;
}

void close_file (int file_descriptor){
    PushLog (4);

    custom_assert (close (file_descriptor) == 0, file_close_error, (void) 0);

    RETURN;
}

size_t line_len (const char *line){
    PushLog (4);

    custom_assert (line != NULL, pointer_is_null, 0);

    size_t length = 0;

    while (line [length] != '\0' && line [length] != '\n'){
        length++;
    }

    RETURN length;
}
