#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <ctype.h>
#include "structures.h"
#include "parsing.h"
#include "reading.h"

/**
 * Initializes given new line.
 */
static void initialize_line(Line *line, size_t number, size_t index) {
    line->words = NULL;
    line->capacity = 0;
    line->size = 0;
    line->number = number;
    line->index = index;
}

/**
 * Initializes default values for given new word.
 */
static void initialize_word(Word *word) {
    word->type = NOT_A_NUMBER;
    word->not_a_number = malloc(BEGIN_WORD_SIZE * sizeof *word->not_a_number);

    if (errno != 0 || word->not_a_number == NULL)
        exit(1);
}

/**
 * Reallocs table to make it 2 times bigger.
 */
static void *custom_realloc(void *pointer, size_t *capacity, size_t size) {
    errno = 0;
    *capacity = (*capacity * 2) + 1;
    void *wynik = realloc(pointer, *capacity * size);

    if (errno != 0 || wynik == NULL)
        exit(1);

    return wynik;
}

/**
 * Checks if given sign can be first sign of number.
 */
static bool is_number_beginning(char sign) {
    if (isdigit(sign) || sign == '-' || sign == '+' || sign == '.' || sign == 'i')
        return true;
    else
        return false;
}

/**
 * Prints information about wrong sign, frees memory of given line and reads rest of the line as garbage.
 */
static void handle_bad_sign(Line *line, size_t line_number, int *sign) {
    fprintf(stderr, "ERROR %zu\n", line_number);

    while ((*sign != NEW_LINE) && (*sign != EOF))
        *sign = getchar();

    for (size_t i = 0; i < line->size; i++)
        if (line->words[i].type == NOT_A_NUMBER)
            free(line->words[i].not_a_number);

    free(line->words);
}

/**
 * Reads one word and changes size of allocated memory as needed.
 */
static void read_word(Word *word, int *sign) {
    size_t capacity = BEGIN_WORD_SIZE;
    size_t index = 0;

    while (*sign <= END_CORRECT_SIGNS && *sign >= BEGIN_CORRECT_SIGNS) {
        if (capacity == index)
            word->not_a_number = (char *) custom_realloc((word->not_a_number), &capacity, sizeof(char));

        *sign = tolower(*sign);
        word->not_a_number[index] = (char) *sign;
        index++;
        *sign = getchar();
    }

    if (capacity == index)
        word->not_a_number = (char *) custom_realloc((word->not_a_number), &capacity, sizeof(char));

    word->not_a_number[index] = END_OF_WORD;
}

/**
 * Reads single line from input, changes size of allocated memory as needed and handle error if bad sign appears.
 * Returns true if there's at least one word and there's no bad sign, false otherwise.
 */
static bool read_line(Line *line, size_t line_number, int *sign) {
    while ((*sign != NEW_LINE) && (*sign != EOF)) {
        while (isspace(*sign) && *sign != NEW_LINE && *sign != EOF)
            *sign = getchar();

        if (*sign <= END_CORRECT_SIGNS && *sign >= BEGIN_CORRECT_SIGNS) {
            if (line->capacity == line->size)
                line->words = (Word *) custom_realloc(line->words, &(line->capacity), sizeof *(line->words));

            initialize_word(&line->words[line->size]);
            read_word(&line->words[line->size], sign);

            if (is_number_beginning(line->words[line->size].not_a_number[0]))
                parse(&line->words[line->size]);

            (line->size)++;
        }

        if ((isspace(*sign) == 0) && (*sign != EOF)) {
            handle_bad_sign(line, line_number, sign);
            return false;
        }
    }

    if (line->size > 0)
        return true;
    else
        return false;
}


/**
 * Reads all the lines from input and changes size of allocated memory as needed.
 */
void read_data(Lines *data) {
    int sign;
    size_t lines_counter = 1;
    sign = getchar();

    while (sign != EOF) {
        if (sign != COMMENT) {
            if (data->capacity == data->size)
                data->lines = (Line *) custom_realloc(data->lines, &(data->capacity), sizeof *(data->lines));

            initialize_line(&data->lines[data->size], lines_counter, data->size);

            if (read_line(&data->lines[data->size], lines_counter, &sign))
                (data->size)++;
        }

        while ((sign != NEW_LINE) && (sign != EOF))
            sign = getchar();

        if (sign == NEW_LINE)
            sign = getchar();

        lines_counter++;
    }
}