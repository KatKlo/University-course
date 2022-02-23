#include <stdio.h>
#include "structures.h"
#include "reading.h"
#include "sorting.h"
#include "printing.h"

int main(void) {
    Lines data = {NULL, 0, 0};
    read_data(&data);
    sort_data(&data);
    print_result(&data);
    free_memory(&data);
    return 0;
}
