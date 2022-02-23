/** @file
  Interfejs stosu do przechowywania wielomianów rzadkich

  @authors Katarzyna Kloc <kk429317@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#ifndef POLYNOMIALS_STACK_H
#define POLYNOMIALS_STACK_H

#include <stdbool.h>
#include "poly.h"

/**
 * To jest struktura przechowująca stos wielomianów.
 */
typedef struct Stack {
    Poly *tab; ///< tablica zawierająca wielomiany
    size_t capacity; ///< pojemność tablicy
    size_t size; ///< aktualna liczba wielomianów
} Stack;

/**
 * Tworzy nowy pusty stos.
 * @return nowy stos
 */
static inline Stack NewStack() {
    Stack memory = {NULL, 0, 0};
    return memory;
}

/**
 * Sprawdza czy stos jest pusty.
 * @param[in] memory : stos wielomianów
 * @return true jeśli stos jest pusty, false wpp
 */
static inline bool IsEmptyStack(const Stack *memory) {
    return memory->size == 0;
}

/**
 * Podaje liczbę wielomianów zapisanych na stosie.
 * @param[in] memory : stos wielomianów
 * @return liczba wielomianów zapisanych na stosie
 */
static inline size_t GetSizeOfStack(const Stack *memory) {
    return memory->size;
}

/**
 * Usuwa ostatni wielomian ze stosu wielomianów.
 * @param[in,out] memory : stos wielomianów
 */
void RemoveLast(Stack *memory);

/**
 * Dodaje wielomian do stosu wielomianów.
 * Przejmuje wielomian na własność.
 * @param[out] memory : stos wielomianów
 * @param[in] p : wielomian
 */
void PushPoly(Stack *memory, Poly p);

/**
 * Zwraca wielomian ze stosu wielomianów z podanej pozycji od końca.
 * @param[in] memory : stos wielomianów
 * @param[in] index : pozycja od końca
 * @return wielomian, ale nie na własność
 */
Poly GetPolyAtIndex(const Stack *memory, size_t index);

/**
 * Zwraca ostatni wielomian ze stosu wielomianów i usuwa go ze stosu.
 * @param[in] memory : stos wielomianów
 * @return wielomian
 */
Poly PullPoly(Stack *memory);

/**
 * Usuwa stos wielomianów z pamięci.
 * @param[out] memory : stos wielomianów
 */
void ClearStack(Stack *memory);

#endif //POLYNOMIALS_STACK_H
