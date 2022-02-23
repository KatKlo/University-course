#include <string.h>
#include <stdlib.h>
#include "structures.h"
#include "sorting.h"

/**
 * Compares 2 integers numbers.
 * Returns 1 if arg1 > arg2, 0 if arg1 = arg2 and -1 if arg1 < arg2.
 */
static int compare_integers(uint64_t arg1, uint64_t arg2) {
    if (arg1 > arg2)
        return 1;
    else if (arg1 == arg2)
        return 0;
    else
        return -1;
}

/**
 * Compares 2 floating numbers.
 * Returns 1 if arg1 > arg2, 0 if arg1 = arg2 and -1 if arg1 < arg2.
 */
static int compare_floating(long double arg1, long double arg2) {
    if (arg1 > arg2)
        return 1;
    else if (arg1 == arg2)
        return 0;
    else
        return -1;
}

/**
 * Compares any two numbers.
 */
static int compare_numbers(Word *arg1, Word *arg2) {
    if (arg1->type == arg2->type) {
        if (arg1->type == INTEGERS)
            return compare_integers(arg1->integer, arg2->integer);
        else
            return compare_floating(arg1->floating, arg2->floating);
    } else if (arg1->type == INTEGERS) {
        return compare_floating((long double) arg1->integer, arg2->floating);
    } else {
        return compare_floating(arg1->floating, (long double) arg2->integer);
    }
}

/**
 * Compare any two words (numbers and not_a_numbers).
 * Sets that not_a_numbers are always larger than numbers.
 * Returns number >1 if arg1 > arg2, 0 if arg1 = arg2 and <1 if arg1 < arg2.
 */
static int compare_words(const void *arg1, const void *arg2) {
    Word argument1 = *(Word *) arg1;
    Word argument2 = *(Word *) arg2;

    if (argument1.type == NOT_A_NUMBER) {
        if (argument2.type == NOT_A_NUMBER)
            return strcmp(argument1.not_a_number, argument2.not_a_number);
        else
            return 1;
    } else if (argument2.type == NOT_A_NUMBER) { // argument1.type == NUMBER
        return -1;
    } else { // argument1.type == argument2.type == NUMBER
        if (argument1.is_positive != argument2.is_positive) {
            if (argument1.is_positive)
                return 1;
            else
                return -1;
        } else if (argument1.is_positive) {
            return compare_numbers(&argument1, &argument2);
        } else {
            return compare_numbers(&argument2, &argument1);
        }
    }
}

/**
 * Sorts words in every line in increasing order.
 */
static void sort_words(Lines *data) {
    for (size_t i = 0; i < data->size; i++)
        qsort(data->lines[i].words, data->lines[i].size, sizeof(Word), compare_words);
}

/**
 * Compare two lines.
 * Returns number >1 if arg1 > arg2, 0 if arg1 = arg2 and <1 if arg1 < arg2.
 */
int compare_lines(const void *arg1, const void *arg2) {
    Line argument1 = *(Line *) arg1;
    Line argument2 = *(Line *) arg2;
    int result;

    if (argument1.size < argument2.size) {
        return -1;
    } else if (argument1.size > argument2.size) {
        return 1;
    } else {
        for (size_t i = 0; i < argument1.size; i++) {
            result = compare_words(&argument1.words[i], &argument2.words[i]);
            if (result != 0)
                return result;
        }

        return 0;
    }
}


/**
 * Compare two lines in stable manner.
 * Returns number >1 if arg1 > arg and <1 if arg1 < arg2,
 * and if they are equal than -1 or 1 appropriately to line number.
 */
int compare_lines_for_sort(const void *arg1, const void *arg2) {
    int result = compare_lines(arg1, arg2);

    if (result == 0)
        return ((*(Line *) arg1).number < (*(Line *) arg2).number ? -1 : 1);

    return result;
}

/**
 * Sorts lines in increasing order.
 */
static void sort_lines(Lines *data) {
    qsort(data->lines, data->size, sizeof *data->lines, compare_lines_for_sort);
}

/**
 * Sorts all data in proper way (every line separately, and then lines).
 */
void sort_data(Lines *data) {
    sort_words(data);
    sort_lines(data);
}