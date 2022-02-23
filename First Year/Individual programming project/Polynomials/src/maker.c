/** @file
  Implementacja silnika dla kalkulatora działającego na wielomianach
  i stosujący odwrotną notację polską

  @authors Katarzyna Kloc <kk429317@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include "poly.h"
#include "stack.h"
#include "parser.h"
#include "maker.h"

//Makra wykorzystywane w sprawdzaniu poprawnosci i wypisywaniu

#define NEW_LINE '\n' ///< Nowa linia
#define SPACE ' ' ///< Rozdziela polecenie od parametru
#define MINUS '-' ///< Symbol liczby ujemnej
#define FALSE "0\n" ///< Kominikat jeśli false
#define TRUE "1\n" ///< Komunikat jeśli true

/**
 * Obsługuje wyniki działania funkcji.
 * W razie błędu wypisuje stosowne komunikaty.
 * @param[in] res : wynik działania funkcji
 * @param[in] number : numer linii z wejścia
 */
static void HandleError(Result res, size_t number) {
    if (res != GOOD)
        fprintf(stderr, "ERROR %zu %s\n", number, res_names[res]);
}

// Funckcja z interfejsu do wielomianów

void ReadAndSavePoly(const char *line, Stack *memory, size_t number) {
    Poly p;

    if (ReadPoly(line, &p))
        PushPoly(memory, p);
    else
        HandleError(WRONG_POLY, number);
}

// Wykonywanie poleceń.

/**
 * Wykonuje instrukcję, która potrzebuje dwóch argumentów.
 * @param[in,out] memory : stos wielomianów
 * @param[in] ins : typ instrukcji
 */
static void MakeTwoArgIns(Stack *memory, Instruction ins) {
    assert(ins == ADD || ins == MUL || ins == SUB || ins == IS_EQ);

    Poly temp1 = GetPolyAtIndex(memory, 1);
    Poly temp2 = GetPolyAtIndex(memory, 2);
    Poly res;

    switch (ins) {
        case ADD:
            res = PolyAdd(&temp1, &temp2);
            break;
        case MUL:
            res = PolyMul(&temp1, &temp2);
            break;
        case SUB:
            res = PolySub(&temp1, &temp2);
            break;
        case IS_EQ:
            if (PolyIsEq(&temp1, &temp2)) fprintf(stdout, TRUE);
            else fprintf(stdout, FALSE);
            return;
        default:
            return;
    }

    RemoveLast(memory);
    RemoveLast(memory);
    PushPoly(memory, res);
}

/**
 * Wykonuje instrukcję, która potrzebuje jednego argumentu.
 * @param[in,out] memory : stos wielomianów
 * @param[in] ins : typ instrukcji
 */
static void MakeOneArgIns(Stack *memory, Instruction ins) {
    assert(ins == PRINT || ins == IS_COEFF || ins == IS_ZERO || ins == NEG
           || ins == CLONE || ins == DEG || ins == POP);

    Poly temp = GetPolyAtIndex(memory, 1);
    Poly res;

    switch (ins) {
        case PRINT:
            PolyPrint(&temp);
            break;
        case IS_COEFF:
            if (PolyIsCoeff(&temp)) fprintf(stdout, TRUE);
            else fprintf(stdout, FALSE);
            break;
        case IS_ZERO:
            if (PolyIsZero(&temp)) fprintf(stdout, TRUE);
            else fprintf(stdout, FALSE);
            break;
        case NEG:
            res = PolyNeg(&temp);
            RemoveLast(memory);
            PushPoly(memory, res);
            break;
        case CLONE:
            res = PolyClone(&temp);
            PushPoly(memory, res);
            break;
        case DEG:
            fprintf(stdout, "%d\n", PolyDeg(&temp));
            break;
        case POP:
            RemoveLast(memory);
            break;
        default:
            break;
    }
}

/**
 * Wykonuje instrukcje ZERO.
 * @param[out] memory : stos wielomianów
 */
static void MakeZeroArgIns(Stack *memory) {
    Poly res = PolyZero();
    PushPoly(memory, res);
}

/**
 * Wczytuje parametr instrukcji DEG_BY lub COMPOSE.
 * @param[in] line : linia z której będzie wczytany parametr instrukcji
 * @param[out] argument: wczytany parametr instrukcji
 * @param[in] ins : typ instrukcji
 * @return true jeśli wczytany parametr jest poprawny, false wpp
 */
static bool ReadUnsignedArgument(const char *line, size_t *argument, Instruction ins) {
    line += strlen(ins_names[ins]);

    if (line[0] != SPACE || !isdigit(line[1]))
        return false;

    char *rest;

    return (ReadUnsignedLong(argument, line, &rest) && rest[0] == NEW_LINE);
}

/**
 * Wczytuje parametr instrukcji AT.
 * @param[in] line : linia z której będzie wczytany parametr instrukcji
 * @param[out] value_x : wczytany parametr instrukcji
 * @return true jeśli wczytany parametr jest poprawny, false wpp
 */
static bool ReadAtArgument(const char *line, poly_coeff_t *value_x) {
    line += strlen(ins_names[AT]);

    if (line[0] != SPACE || (!isdigit(line[1]) && line[1] != MINUS))
        return false;

    char *rest;

    return (ReadLong(value_x, line, &rest) && rest[0] == NEW_LINE);
}

/**
 * Wykonuje instrukcje DEG_BY.
 * @param[in] line : linia z której będzie wczytany parametr instrukcji
 * @param[in] memory : stos wielomianów
 * @return wynik działania funkcji
 */
static Result MakeDegBy(const char *line, Stack *memory) {
    size_t id_x;

    if (!ReadUnsignedArgument(line, &id_x, DEG_BY))
        return WRONG_DEG_BY;

    if (IsEmptyStack(memory))
        return STACK_UNDERFLOW;

    Poly temp = GetPolyAtIndex(memory, 1);
    fprintf(stdout, "%d\n", PolyDegBy(&temp, id_x));

    return GOOD;
}

/**
 * Wykonuje instrukcje AT.
 * @param[in] line : linia z której będzie wczytany parametr instrukcji
 * @param[in] memory : stos wielomianów
 * @return wynik działania funkcji
 */
static Result MakeAt(const char *line, Stack *memory) {
    poly_coeff_t value_x;

    if (!ReadAtArgument(line, &value_x))
        return WRONG_POLY_AT;

    if (IsEmptyStack(memory))
        return STACK_UNDERFLOW;

    Poly temp = GetPolyAtIndex(memory, 1);
    Poly res = PolyAt(&temp, value_x);
    RemoveLast(memory);
    PushPoly(memory, res);

    return GOOD;
}

/**
 * Wykonuje instrukcje COMPOSE.
 * @param[in] line : linia z której będzie wczytany parametr instrukcji
 * @param[in] memory : stos wielomianów
 * @return wynik działania funkcji
 */
static Result MakeCompose(const char *line, Stack *memory) {
    size_t k;
    if (!ReadUnsignedArgument(line, &k, COMPOSE))
        return WRONG_COMPOSE;

    size_t size = GetSizeOfStack(memory);
    if (size - 1 < k || size == 0)
        return STACK_UNDERFLOW;

    Poly p = PullPoly(memory);
    Poly *q = malloc(k * sizeof *q);

    /// ściągane wielomiany ze stosu to kolejno q[k - 1], q[k - 2], ..., q[0]
    for (size_t i = 0; i < k; i++)
        q[k - i - 1] = PullPoly(memory);

    Poly res = PolyCompose(&p, k, q);
    PushPoly(memory, res);

    PolyDestroy(&p);
    for (size_t i = 0; i < k; i++)
        PolyDestroy(&(q[i]));

    free(q);

    return GOOD;
}

// Funckcja z interfejsu do poleceń

void ReadAndMakeAction(const char *line, Stack *memory, size_t number) {
    Instruction ins;

    if (!ReadAction(line, &ins)) {
        HandleError(WRONG_COMMAND, number);
        return;
    }

    Result res = GOOD;

    if (ins == ZERO) {
        MakeZeroArgIns(memory);
    } else if (ins == AT) {
        res = MakeAt(line, memory);
    } else if (ins == DEG_BY) {
        res = MakeDegBy(line, memory);
    } else if (ins == COMPOSE) {
        res = MakeCompose(line, memory);
    } else if (ins == ADD || ins == MUL || ins == SUB || ins == IS_EQ) {
        if (GetSizeOfStack(memory) < 2)
            res = STACK_UNDERFLOW;
        else
            MakeTwoArgIns(memory, ins);
    } else {
        if (IsEmptyStack(memory))
            res = STACK_UNDERFLOW;
        else
            MakeOneArgIns(memory, ins);
    }

    HandleError(res, number);
}


