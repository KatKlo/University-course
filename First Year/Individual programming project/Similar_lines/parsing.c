#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include "structures.h"
#include "parsing.h"

/**
 * Reads integer number in base-based numeric system written in given word.
 * Returns true if the whole word is a proper number, false otherwise.
 */
static bool read_integer(char *word, uint64_t *number, int base) {
    char *rest;
    errno = 0;

    *number = strtoull(word, &rest, base);

    return (rest[0] == 0) && (errno != ERANGE);
}

/**
 * Reads floating number written in given word.
 * Returns true if the whole word is a proper number, false otherwise.
 */
static bool read_floating(char *word, long double *number) {
    char *rest;
    errno = 0;

    *number = strtold(word, &rest);

    return (rest[0] == 0) && (errno != ERANGE);
}

/**
 * Saves given integer number to a given structure.
 */
static void save_integer(Word *result, uint64_t number) {
    result->type = INTEGERS;
    result->integer = number;

    if (number == 0)
        result->is_positive = true;
}

/**
 * Saves given floating number to a given structure.
 */
static void save_floating(Word *result, long double number) {
    result->type = FLOATING;
    result->floating = number;

    if (number == 0)
        result->is_positive = true;
}

/**
 * Checks if the starting index can be increased before reading number if it starts with '+' or '-'.
 * sign - first sign after '+'/'-' sign.
*/
static bool move_after_sign(char sign) {
    if ((sign != END_OF_WORD) && (isdigit(sign) || sign == '.' || sign == 'i'))
        return true;
    else
        return false;
}

/**
 * Checks if the starting index can be increased before reading number if it starts with zero.
 * sign - first sign after '0'.
*/
static bool move_after_zero(char sign) {
    if ((sign != END_OF_WORD) && (isdigit(sign) || sign == 'x'))
        return true;
    else
        return false;
}


/**
 * Determines how far we should move starting index while reading word as number.
 * We want to skip '+'/'-' sign and leading zeros.
 */
static void move_index(char *word, size_t *move) {
    if ((word[0] == '-' || word[0] == '+') && (move_after_sign(word[1])))
        *move = 1;

    while (word[*move] == '0' && move_after_zero(word[*move + 1]))
        (*move)++;
}

/**
 * Checks and saves number if given word is proper octal or hexadecimal number.
 * Returns true if the word is proper octal or hexadecimal number, false otherwise.
 */
static bool is_octal_or_hexadecimal(char *word, uint64_t *number) {
    if (word[1] == 'x') {
        if (word[2] == END_OF_WORD) {
            *number = 0;
            return true;
        } else {
            return read_integer(word, number, 16);
        }
    } else {
        return read_integer(word, number, 8);
    }
}

/**
 * Changes word into number if that change is possible.
 */
void parse(Word *result) {
    char *word = result->not_a_number;
    uint64_t integer_value;
    long double floating_value;
    int is_success = false;
    result->is_positive = true;
    size_t move = 0;

    if (word[0] == '0')
        is_success = is_octal_or_hexadecimal(word, &integer_value);

    if (!(is_success)) {
        if (word[0] == '-')
            result->is_positive = false;

        move_index(word, &move);

        is_success = read_integer(word + move, &integer_value, 10);
    }

    if (is_success) {
        free(result->not_a_number);
        save_integer(result, integer_value);
    } else {
        if (read_floating(word + move, &floating_value)) {
            if (floating_value == floating_value) {
                free(result->not_a_number);
                save_floating(result, floating_value);
            }
        }
    }
}

