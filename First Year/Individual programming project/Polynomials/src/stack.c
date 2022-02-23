/** @file
  Implementacja stosu do przechowywania wielomianów rzadkich

  @authors Katarzyna Kloc <kk429317@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include <stdlib.h>
#include <assert.h>
#include "stack.h"
#include "poly.h"

/**
 * Zwiększa stos wielomianów.
 * @param[in,out] memory : stos wielomianów
 */
static void ExtendStack(Stack *memory) {
    memory->capacity = (memory->capacity * 2) + 1;
    memory->tab = realloc(memory->tab, memory->capacity * sizeof *(memory->tab));

    if (memory->tab == NULL)
        exit(1);
}

/**
 * Zmniejsza stos wielomianów.
 * @param[in,out] memory : stos wielomianów
 */
static void ReduceStack(Stack *memory) {
    memory->capacity = memory->capacity / 2;
    memory->tab = realloc(memory->tab, memory->capacity * sizeof *(memory->tab));

    if (memory->tab == NULL && memory->capacity != 0)
        exit(1);
}

void RemoveLast(Stack *memory) {
    assert(memory->size > 0);
    Poly temp = memory->tab[memory->size - 1];
    PolyDestroy(&temp);
    (memory->size)--;

    if (memory->size < memory->capacity / 4)
        ReduceStack(memory);
}

void PushPoly(Stack *memory, Poly p) {
    if (memory->size == memory->capacity)
        ExtendStack(memory);

    memory->tab[memory->size++] = p;
}

Poly GetPolyAtIndex(const Stack *memory, size_t index) {
    assert(index <= memory->size);
    return memory->tab[memory->size - index];
}

Poly PullPoly(Stack *memory) {
    assert(memory->size > 0);
    (memory->size)--;
    return memory->tab[memory->size];
}

void ClearStack(Stack *memory) {
    for (size_t i = 0; i < memory->size; i++)
        PolyDestroy(&(memory->tab[i]));

    free(memory->tab);
}
