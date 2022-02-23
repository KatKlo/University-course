#ifndef SIMILAR_LINES_STRUCTURES_H
#define SIMILAR_LINES_STRUCTURES_H

#include <stdbool.h>
#include <stdio.h>

/**
 * Enum struct defining word's type.
 */
typedef enum type_t {
    INTEGERS, FLOATING, NOT_A_NUMBER
} type_t;

/**
 * Struct for storing word.
 */
typedef struct Word {
    enum type_t type;
    bool is_positive;
    union {
        u_int64_t integer;
        long double floating;
        char *not_a_number;
    };
} Word;

/**
 * Struct for storing one line of words.
 */
typedef struct Line {
    Word *words;
    size_t capacity;
    size_t size;
    size_t number; // number given in the input
    size_t index; // index in which line was saved
} Line;


/**
 * Struct for storing all the lines.
 */
typedef struct Lines {
    Line *lines;
    size_t capacity;
    size_t size;
} Lines;

#endif //SIMILAR_LINES_STRUCTURES_H
