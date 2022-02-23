/** @file
  Kalkulator działający na wielomianach i stosujący odwrotną notację polską

  @authors Katarzyna Kloc <kk429317@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#define _GNU_SOURCE ///< makro potrzebne do działania funkcji getline

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "maker.h"
#include "stack.h"

#define NEW_LINE '\n' ///< Nowa linia
#define COMMENT '#' ///< Znak oznaczający komentarz

/**
 * Wczytuje linie sprawdzając, czy nie zabrakło pamięci.
 * Jeżeli linia nie kończy się znakiem nowej linii to go dopisuje.
 * @param[out] line : wskaźnik na miejsce, gdzie ma być zapisana linia
 * @param[in, out] capacity : zaalokowana pojemność dostarczonego wskaźnika
 */
static void ReadLine(char **line, size_t *capacity) {
    errno = 0;
    ssize_t size = getline(line, capacity, stdin);

    if (size == -1 || errno == ENOMEM)
        exit(1);

    if ((*line)[size - 1] != NEW_LINE) {
        if ((size_t) size == *capacity) {
            *line = realloc(*line, *capacity * sizeof *(*line));

            if (*line == NULL)
                exit(1);
        }

        (*line)[size] = '\n';
    }
}

/**
 * Wykonuje kolejne linie i wykonuje odpowiednie działania.
 * @return 0 jeśli wszystko pójdzie zgodnie z planem
 */
int main(void) {
    Stack memory = NewStack();
    size_t number_of_line = 1;
    char *line = NULL;
    size_t line_capacity = 0;

    int c = getchar();
    while (c != EOF) {
        if (c == COMMENT) {
            while ((c != NEW_LINE) && (c != EOF))
                c = getchar();
        } else if (c != NEW_LINE) {
            ungetc(c, stdin);
            ReadLine(&line, &line_capacity);

            if (isalpha(c))
                ReadAndMakeAction(line, &memory, number_of_line);
            else
                ReadAndSavePoly(line, &memory, number_of_line);
        }

        c = getchar();
        number_of_line++;
    }

    free(line);
    ClearStack(&memory);

    return 0;
}

