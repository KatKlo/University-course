#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "structures.h"
#include "sorting.h"
#include "printing.h"

/**
 * Prints result of the program - numbers of similar lines.
 */
void print_result(Lines *data) {
    size_t i, j;
    size_t searched = 0;
    errno = 0;
    bool *visited = (bool *) calloc(data->size, sizeof(bool));

    if (visited == NULL || errno != 0)
        exit(1);

    while (searched < data->size) {
        i = 0;

        while (data->lines[i].index != searched)
            i++;

        visited[data->lines[i].index] = true;
        fprintf(stdout, "%zu", data->lines[i].number);
        j = i + 1;

        while ((j < data->size) && (compare_lines(&data->lines[j], &data->lines[i]) == 0)) {
            fprintf(stdout, " %zu", data->lines[j].number);
            visited[data->lines[j].index] = true;
            j++;
        }

        fprintf(stdout, "\n");

        while ((searched < data->size) && (visited[searched]))
            searched++;
    }

    free(visited);
}

/**
 * Frees all the allocated and not freed earlier memory.
 */
void free_memory(Lines *data) {
    for (size_t i = 0; i < data->size; i++) {
        for (size_t j = 0; j < data->lines[i].size; j++)
            if (data->lines[i].words[j].type == NOT_A_NUMBER)
                free(data->lines[i].words[j].not_a_number);

        free(data->lines[i].words);
    }

    free(data->lines);
}
