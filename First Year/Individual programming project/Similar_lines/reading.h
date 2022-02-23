#ifndef SIMILAR_LINES_READING_H
#define SIMILAR_LINES_READING_H

#include "structures.h"

#define COMMENT '#'
#define BEGIN_CORRECT_SIGNS 33
#define END_CORRECT_SIGNS 126
#define NEW_LINE '\n'
#define END_OF_WORD '\0'
#define BEGIN_WORD_SIZE 7

extern void read_data(Lines *data);

#endif //SIMILAR_LINES_READING_H
