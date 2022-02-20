#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define VARIABLE_NUMBER 26
#define BASE 10000

/**
 * Important signs
 */
#define PRINT '='
#define LOOP_BEGIN '('
#define LOOP_END ')'
#define NEW_LINE '\n'
#define ASCII_A 'a'

/**
 * Enum struct for different types of commands.
 */
typedef enum command_t {
    INC, ADD, CLR, JMP, DJZ, HLT
} command_t;

/**
 * Struct for storing numbers in base-10000 numeral system.
 */
typedef struct {
    size_t size;
    short *digits;
} number;

/**
 * Struct for storing instructions.
 */
typedef struct {
    enum command_t command;
    size_t arg1;
    size_t arg2;
} instruction;

/**
 * Struct to store machine_code.
 */
typedef struct {
    instruction *tab;
    size_t capacity;
    size_t actual;
} machine_code;

/**
 * Prepares variables before starting, sets them all to 0.
 */
void prepare_variables(number *variables) {
    for (size_t i = 0; i < VARIABLE_NUMBER; i++) {
        variables[i].size = 1;
        variables[i].digits = malloc(variables[i].size * sizeof(short));
        variables[i].digits[0] = 0;
    }
}

/**
 * Changes size of table storing variable.
 */
void realloc_variable(number *variable, size_t new_size) {
    variable->size = new_size;
    variable->digits = realloc(variable->digits, variable->size * sizeof *variable->digits);
}

/**
 * Frees memory allocated for storing variables.
 */
void free_variables(number *variables) {
    for (size_t i = 0; i < VARIABLE_NUMBER; i++)
        free(variables[i].digits);
}

/**
 * Prepares struct for storing machine code.
 */
void prepare_machine_code(machine_code *code) {
    code->actual = 0;
    code->capacity = 0;
    code->tab = NULL;
}

/**
 * Changes size of table storing machine code.
 */
void realloc_machine_code(machine_code *code, size_t new_size) {
    code->capacity = new_size;
    code->tab = realloc(code->tab, code->capacity * sizeof *(code->tab));
}

/**
 * Frees memory allocated for storing machine code.
 */
void free_machine_code(machine_code *code) {
    free(code->tab);
}

void add_to_next_digit(size_t i, number *variable, short to_save) {
    variable->digits[i] = to_save;

    if (i + 1 == variable->size) {
        realloc_variable(variable, variable->size + 1);
        variable->digits[i + 1] = 0;
    }

    (variable->digits[i + 1])++;
}

/**
 * Adds variable2 to variable1.
 */
void add_two_variables(number *variable1, const number *variable2) {
    if (variable1->size < variable2->size) {
        size_t old_size1 = variable1->size;
        realloc_variable(variable1, variable2->size);

        while (old_size1 < variable1->size)
            variable1->digits[old_size1++] = 0;
    }

    bool continue_adding = true;
    size_t i = 0;

    while (continue_adding) {
        if (i < variable2->size)
            variable1->digits[i] = (short) (variable1->digits[i] + variable2->digits[i]);

        if (variable1->digits[i] >= BASE)
            add_to_next_digit(i, variable1, (short) (variable1->digits[i] - BASE));
        else if (i + 1 >= variable2->size)
            continue_adding = false;

        i++;
    }
}

/**
 * Adds 1 to given variable.
 */
void add_one(number *variable) {
    bool need_adding = true;
    size_t i = 0;
    (variable->digits[i])++;

    while (need_adding) {
        if (variable->digits[i] == BASE)
            add_to_next_digit(i, variable, 0);
        else
            need_adding = false;

        i++;
    }
}

/**
 * Subtracts 1 from given number.
 */
void subtract_one(number *variable) {
    bool need_subtracting = true;
    size_t i = 0;

    while (need_subtracting) {
        if (variable->digits[i] == 0) {
            variable->digits[i] = BASE - 1;
            i++;
        } else {
            (variable->digits[i])--;
            need_subtracting = false;
        }
    }

    if ((variable->size > 1) && (variable->digits[variable->size - 1] == 0))
        realloc_variable(variable, variable->size - 1);
}

/**
 * Sets given variable to 0.
 */
void set_to_zero(number *variable) {
    realloc_variable(variable, 1);
    variable->digits[0] = 0;
}

/**
 * Computes instructions from machine code.
 */
void compute_machine_code(const machine_code *code, number *variables) {
    size_t i = 0;
    bool has_ended = false;

    while (!has_ended) {
        switch (code->tab[i].command) {
            case INC:
                add_one(&variables[code->tab[i].arg1]);
                i++;
                break;
            case ADD:
                add_two_variables(&variables[code->tab[i].arg1], &variables[code->tab[i].arg2]);
                i++;
                break;
            case CLR:
                set_to_zero(&variables[code->tab[i].arg1]);
                i++;
                break;
            case JMP:
                i = code->tab[i].arg1;
                break;
            case DJZ:
                if ((variables[code->tab[i].arg1].size > 1) || (variables[code->tab[i].arg1].digits[0] != 0)) {
                    subtract_one(&variables[code->tab[i].arg1]);
                    i++;
                } else {
                    i = code->tab[i].arg2;
                }
                break;
            case HLT:
                has_ended = true;
        }
    }
}

/**
 * Saves command type and argument as next instruction in machine_code.
 */
void save_instruction(machine_code *code, command_t command, size_t arg1) {
    code->tab[code->actual].command = command;
    code->tab[code->actual].arg1 = arg1;
    (code->actual)++;
}

/**
 * Funkcja wykonująca optymalizację kodu maszynowego
 */
void make_optimization(machine_code *code, size_t saved_number) {
    size_t end = code->actual - 1;
    code->actual = saved_number;
    size_t argument = code->tab[saved_number].arg1;

    while (code->actual < end) {
        code->tab[code->actual].arg2 = argument;
        save_instruction(code, ADD, code->tab[code->actual + 1].arg1);
    }

    save_instruction(code, CLR, argument);
}

/**
 * Reads program in 'petlik' language, convert it into machine code and optimize if possible.
 */
void compile(machine_code *code, size_t saved_number) {
    bool to_optimize = true;
    size_t sign;

    while ((sign = (size_t) getchar()) != NEW_LINE) {
        if (code->actual == code->capacity)
            realloc_machine_code(code, 2 * code->capacity + 1);

        if (sign == LOOP_BEGIN) {
            to_optimize = false;
            sign = (size_t) getchar() - ASCII_A;
            save_instruction(code, DJZ, sign);

            compile(code, (code->actual) - 1);
        } else if (sign == LOOP_END) {
            if (to_optimize == true) {
                make_optimization(code, saved_number);
            } else {
                save_instruction(code, JMP, saved_number);
                code->tab[saved_number].arg2 = code->actual;
            }

            return;
        } else {
            if ((saved_number < code->actual) && (sign == code->tab[saved_number].arg1 + ASCII_A))
                to_optimize = false;

            save_instruction(code, INC, (size_t) (sign - ASCII_A));
        }
    }

    if (code->actual == code->capacity)
        realloc_machine_code(code, code->capacity + 1);

    code->tab[code->actual].command = HLT;
}

/**
 * Compiles and computes program in 'petlik' language.
 */
void compute_program(number *variables) {
    machine_code code;
    prepare_machine_code(&code);
    compile(&code, 0);
    compute_machine_code(&code, variables);
    free_machine_code(&code);
}

/**
 * Prints given variable.
 */
void print_variable(const number *variable) {
    printf("%hd", variable->digits[variable->size - 1]);

    for (int i = (int) variable->size - 2; i >= 0; i--) {
        printf("%04hd", variable->digits[i]);
    }

    putchar(NEW_LINE);
}

int main(void) {
    number variables[VARIABLE_NUMBER];
    prepare_variables(variables);
    int sign;

    while ((sign = getchar()) != EOF) {
        if (sign == PRINT) {
            int to_print = getchar() - ASCII_A;
            print_variable(&variables[to_print]);
            getchar(); // new line sign
        } else {
            ungetc(sign, stdin);
            compute_program(variables);
        }
    }

    free_variables(variables);

    return 0;
}
