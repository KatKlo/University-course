#include <stdio.h>
#include <stdbool.h>

#define DECK_SIZE 81
#define SET_SIZE 3
#define MIN_ON_BOARD 12
#define DIVIDER 10

// Reads at most max_to_read cards from the input and saves them in table.
// Returns how many cards were loaded.
int read_cards(int cards[]) {
    int card, cards_counter = 0;

    while (scanf("%d", &card) > 0) {
        cards[cards_counter++] = card;
    }

    return cards_counter;
}

// Prints cards which are currently on table.
void show_table(const int cards[], int number_of_cards) {
    printf("=");

    for (int i = 0; i < number_of_cards; i++) {
        printf(" %d", cards[i]);
    }

    printf("\n");
}

// Prints cards which create set.
void print_set(const int cards[], int index1, int index2, int index3) {
    printf("- %d %d %d\n", cards[index1], cards[index2], cards[index3]);
}

// Sets cards creating set to 0 in table.
void reset_set(int cards[], int index1, int index2, int index3) {
    cards[index1] = 0;
    cards[index2] = 0;
    cards[index3] = 0;
}

// Checks if given 3 cards create set.
// Uses observation that only proper combinations of 1, 2, 3 sums to number divisible by 3
bool check_set(const int cards[], int index1, int index2, int index3) {
    int sum;

    for (int x = 1; x <= 1000; x *= DIVIDER) {
        sum = (cards[index1] / x) % DIVIDER
                + (cards[index2] / x) % DIVIDER
                + (cards[index3] / x) % DIVIDER;

        if (sum % 3 != 0) {
            return false;
        }
    }

    return true;
}

// Checks every combination of cards until finding proper set which is then printed and set to 0.
// Return true if proper set is found, false otherwise.
bool search_and_print_set(int cards[], int number_of_cards) {
    for (int i = 0; i < number_of_cards - 2; i++) {
        for (int j = i + 1; j < number_of_cards - 1; j++) {
            for (int k = j + 1; k < number_of_cards; k++) {
                if (check_set(cards, i, j, k)) {
                    print_set(cards, i, j, k);
                    reset_set(cards, i, j, k);
                    
                    return true;
                }
            }
        }
    }

    return false;
}

// Moves cards so there's no gap (with number 0) between correct cards.
void move_cards(int cards[], int number_of_cards) {
    int cards_counter = 0;

    for (int i = 0; i < number_of_cards; i++) {
        while (cards[i + cards_counter] == 0) {
            cards_counter++;
        }

        cards[i] = cards[i + cards_counter];
    }
}

// Simulates the game of set.
void simulate_game(int cards[], int number_of_cards) {
    bool has_ended = false;
    int cards_on_table = (number_of_cards > 12 ? 12 : number_of_cards);

    while (has_ended == false) {
        show_table(cards, cards_on_table);

        if (search_and_print_set(cards, cards_on_table)) {
            // there's a set
            move_cards(cards, number_of_cards);
            cards_on_table -= SET_SIZE;
            number_of_cards -= SET_SIZE;

            if (cards_on_table <= MIN_ON_BOARD && number_of_cards > cards_on_table)
                cards_on_table += SET_SIZE;
        } else if (number_of_cards > cards_on_table) {
            // there's no set but there's more cards to add on table
            cards_on_table += SET_SIZE;

            printf("+\n");
        } else {
            // there's no set and no more cards to add
            has_ended = true;
        }
    }
}

int main(void) {
    int cards[DECK_SIZE];
    int number_of_cards = read_cards(cards);

    simulate_game(cards, number_of_cards);

    return 0;
}