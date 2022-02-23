/** @file
  Interfejs silnika dla kalkulatora działającego na wielomianach
  i stosujący odwrotną notację polską

  @authors Katarzyna Kloc <kk429317@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#ifndef POLYNOMIALS_MAKER_H
#define POLYNOMIALS_MAKER_H

#include "stack.h"

/**
 * To jest struktura wyliczeniowa przechowująca możliwe wyniki działania funkcji.
 */
typedef enum Result {
    GOOD, ///< wszystko poszło zgodnie z planem
    WRONG_POLY, ///< zapisany wielomian jest niepoprawny
    WRONG_COMMAND, ///< zapisana nazwa polecenie jest niepoprawna
    WRONG_DEG_BY, ///< brak lub niepoprawny argument w poleceniu DEG_BY
    WRONG_POLY_AT, ///< brak lub niepoprawny argument w poleceniu POLY_AT
    STACK_UNDERFLOW, ///< na stosie jest za mało wielomianów do wykonania polecenia
    WRONG_COMPOSE /// brak lub niepoprawny argument w poleceniu COMPOSE
} Result;

/**
 * Definicja stałej tablicy przechowującej napisy
 * wypisywane przez poszczególne errory
 */
static const char *const res_names[] = {
        [WRONG_POLY] = "WRONG POLY",
        [WRONG_COMMAND] = "WRONG COMMAND",
        [WRONG_DEG_BY] = "DEG BY WRONG VARIABLE",
        [WRONG_POLY_AT] = "AT WRONG VALUE",
        [STACK_UNDERFLOW] = "STACK UNDERFLOW",
        [WRONG_COMPOSE] = "COMPOSE WRONG PARAMETER",
};

/**
 * Wczytuje i dodaje do stosu wielomianów wielomian zapisany w podanej linii.
 * @param[in] line : linia w której zapisany jest wielomian
 * @param[out] memory : stos wielomianów do którego będzie dodany wielomian
 * @param[in] number : numer linii do wypisania w razie błędu
 */
void ReadAndSavePoly(const char *line, Stack *memory, size_t number);

/**
 * Wczytuje i wykonuje polecenie zapisane w podanej linii.
 * @param[in] line : linia w której zapisane jest polecenie
 * @param[in,out] memory : stos wielomianów na którym polecenie jest wykonywane
 * @param[in] number : numer linii do wypisania w razie błędu
 */
void ReadAndMakeAction(const char *line, Stack *memory, size_t number);

#endif //POLYNOMIALS_MAKER_H
