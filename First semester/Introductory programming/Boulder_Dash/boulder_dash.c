#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Field sign
 */
#define ROCKFORD '@'
#define EMPTY ' '
#define SOIL '+'
#define ROCK 'O'
#define DIAMOND '$'
#define EXIT 'X'
#define NEW_LINE '\n'

/**
 * Commands
 */
#define PRINT '\n'
#define UP 'w'
#define LEFT 'a'
#define DOWN 's'
#define RIGHT 'd'

/**
 * Struct to store coordinates of Rockford_place numbered from 0.
 */
typedef struct {
    size_t row;
    size_t column;
} coordinates;

/**
 * Struct to store important information about current game.
 */
typedef struct {
    char **board;
    size_t row_number;
    size_t column_number;
    int diamond_number;
    coordinates Rockford_place;
    bool is_Rockford_alive;
} state;

/**
 * Reads dimensions of board.
 */
void read_dimensions(state *game_state) {
    scanf("%zu%zu", &game_state->row_number, &game_state->column_number);
}

/**
 * Reads board, remembers Rockford position and number of diamonds.
 */
void read_board(state *game_state) {
    for (size_t row = 0; row < game_state->row_number; row++) {
        getchar(); // Reads sign of new line

        for (size_t column = 0; column < game_state->column_number; column++) {
            game_state->board[row][column] = (char) getchar();

            if (game_state->board[row][column] == DIAMOND) {
                (game_state->diamond_number)++;
            }

            if (game_state->board[row][column] == ROCKFORD) {
                game_state->Rockford_place.row = row;
                game_state->Rockford_place.column = column;
                game_state->is_Rockford_alive = true;
            }
        }
    }
}

/**
 * Prints actual state of the board
 */
void print_board(state *game_state) {
    printf("%zu %zu\n", game_state->row_number, game_state->column_number);

    for (size_t row = 0; row < game_state->row_number; row++) {
        for (size_t column = 0; column < game_state->column_number; column++) {
            putchar(game_state->board[row][column]);
        }

        putchar(NEW_LINE);
    }
}

/**
 * Stabilizes the board according to gravity.
 */
void stabilize_board(state *game_state, size_t begin, size_t end) {
    for (size_t column = begin; column <= end; column++) {
        for (size_t row = game_state->row_number - 2; row > 0; row--) {
            size_t k = row;
            char sign = game_state->board[k][column];
            
            while ((sign == ROCK || sign == DIAMOND) && (game_state->board[k + 1][column] == EMPTY)) {
                game_state->board[k + 1][column] = sign;
                game_state->board[k][column] = EMPTY;
                k++;
            }
        }
    }
}

/**
 * Prepares the board before game (allocates memory for board).
 */
void prepare_game(state *game_state) {
    game_state->diamond_number = 0;
    game_state->is_Rockford_alive = false;
    
    read_dimensions(game_state);
    game_state->board = malloc(game_state->row_number * sizeof(char *));
    
    for (size_t row = 0; row < game_state->row_number; row++) {
        game_state->board[row] = malloc(game_state->column_number * sizeof(char));
    }
    
    read_board(game_state);
    
    if (game_state->column_number > 2) {
        stabilize_board(game_state, 1, game_state->column_number - 2);
    }
}

/**
 * Free memory allocated for the board.
 */
void free_board(state *game_state) {
    for (size_t i = 0; i < game_state->row_number; i++) {
        free(game_state->board[i]);
    }
    
    free(game_state->board);
}

/**
 * Sets Rockford's target coordinates according to command.
 * Returns true if command is correct, false otherwise.
 */
bool compute_command(char sign, coordinates *target, state *game_state) {
    target->row = game_state->Rockford_place.row;
    target->column = game_state->Rockford_place.column;

    switch (sign) {
        case LEFT:
            target->column -= 1;
            break;
        case RIGHT:
            target->column += 1;
            break;
        case UP:
            target->row -= 1;
            break;
        case DOWN:
            target->row += 1;
            break;
        default:
            return false;
    }

    return true;
}

/**
 * Moves object from one field to another, it can be Rockford or rock.
 */
void move_object(state *game_state, coordinates from, coordinates to) {
    if (game_state->board[to.row][to.column] != EXIT) {
        game_state->board[to.row][to.column] = game_state->board[from.row][from.column];
    }

    game_state->board[from.row][from.column] = EMPTY;
}

/**
 * Moves Rockford, changes his coordinates and stabilize the board.
 */
void move_Rockford(state *game_state, coordinates target) {
    move_object(game_state, game_state->Rockford_place, target);

    game_state->Rockford_place.row = target.row;
    game_state->Rockford_place.column = target.column;

    stabilize_board(game_state, target.column - 1, target.column + 1);
}

/**
 * Makes Rockford move if it's possible and actualize board state.
 */
void make_move(state *game_state, coordinates target) {
    char sign = game_state->board[target.row][target.column];

    if (sign == EMPTY || sign == SOIL || sign == DIAMOND) { // Rockford just enters target field
        move_Rockford(game_state, target);

        if (sign == DIAMOND) {
            (game_state->diamond_number)--;
        }
    } else if (sign == ROCK && target.row == game_state->Rockford_place.row) { // There's rock on the field
        coordinates behind_rock;
        behind_rock.row = target.row;
        behind_rock.column = 2 * target.column - game_state->Rockford_place.column;

        if (game_state->board[behind_rock.row][behind_rock.column] == EMPTY) {
            move_object(game_state, target, behind_rock);
            move_Rockford(game_state, target);
        }
    } else if (sign == EXIT && game_state->diamond_number == 0) { // Rockford ends the game
        game_state->is_Rockford_alive = false;
        move_Rockford(game_state, target);
    }
}

int main(void) {
    state game_state;
    prepare_game(&game_state);

    char sign;
    coordinates target;

    while (scanf("%c", &sign) != EOF) {
        if (sign == PRINT) {
            print_board(&game_state);
        } else if (game_state.is_Rockford_alive && compute_command(sign, &target, &game_state)) {
            make_move(&game_state, target);
        }
    }

    free_board(&game_state);

    return 0;
}
