/** @file
  Implementacja parsera dla kalkulatora działającego na wielomianach
  i stosujący odwrotną notację polską

  @authors Katarzyna Kloc <kk429317@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include "poly.h"
#include "parser.h"

//Makra wykorzystywane w sprawdzaniu poprawności wielomianu

#define NEW_LINE '\n' ///< Nowa linia
#define START_MONO '(' ///< Oznaczenie początku jednomianu
#define END_MONO ')' ///< Oznaczenie końca jednomianu
#define PLUS '+' ///< Oznaczenie sumy jednomianów
#define MINUS '-' ///< Symbol liczby ujemnej
#define COMMA ',' ///< Symbol rozdzielający współczynnik od wykładnika
#define COEFF '0' ///< Oznaczenie współczynnika
#define EXP '1' ///< Oznaczenie wykładnika

// Obsługa tablicy jednomianów.

/**
 * Usuwa tablice jednomianów z pamięci.
 * @param[out] monos : tablica jednomianów
 * @param[in] amount : liczba zapisanych jednomianów
 */
static void DestroyMonos(Mono *monos, size_t amount) {
    for (size_t i = 0; i < amount; i++)
        MonoDestroy(&(monos[i]));

    free(monos);
}

/**
 * Powiększa dynamicznie tablice jednomianów.
 * @param[in,out] monos : tablica jednomianów
 * @param[in,out] size : obecny rozmiar tablicy
 */
static void ExtendMonos(Mono **monos, size_t *size) {
    *size = *size * 2 + 1;
    *monos = realloc(*monos, *size * sizeof **monos);

    if (*monos == NULL)
        exit(1);
}

// Funkcje do parsowania liczb.

bool ReadLong(long *result, const char *line, char **rest) {
    errno = 0;
    *result = strtol(line, rest, 10);
    return errno == 0;
}

bool ReadUnsignedLong(unsigned long *result, const char *line, char **rest) {
    errno = 0;
    *result = strtoul(line, rest, 10);

    return errno == 0;
}

bool ReadInt(int *result, const char *line, char **rest) {
    long temp;

    if (ReadLong(&temp, line, rest)) {
        if (temp <= INT_MAX && temp >= INT_MIN) {
            *result = (int) temp;
            return true;
        }
    }
    return false;
}

// Parsowanie wielomianu.

static bool ReadPolyHelp(const char **line, Poly *p);

/**
 * Wczytuje wykładnik jednomianu zapisany w linii.
 * @param[in,out] line : linia w której wykładnik jest zapisany,
 * po wykonaniu wskaźnik wskazuje na pierwszy znak po znaku kończącym jednomian
 * @param[out] exp : wskaźnik na miejsce gdzie zostanie zapisany wczytany wykładnik
 * @return true jeśli wykładnik jest poprawny, false wpp
 */
static bool ReadExp(const char **line, poly_exp_t *exp) {
    char *rest;

    if (ReadInt(exp, *line, &rest) && rest[0] == END_MONO) {
        *line = rest + 1;
        return true;
    } else {
        return false;
    }
}

/**
 * Wczytuje jednomian zapisany w linii.
 * @param[in,out] line : linia w której jednomian jest zapisany,
 * po wykonaniu wskaźnik wskazuje na pierwszy znak po znaku kończącym jednomian
 * @param[out] m : jednomian wyjściowy
 * @return true jeśli jednomian jest poprawny, false wpp
 */
static bool ReadMono(const char **line, Mono *m) {
    assert(**line == START_MONO);
    (*line)++;
    Poly coeff;

    if (!ReadPolyHelp(line, &coeff))
        return false;

    assert(**line == COMMA);
    (*line)++;

    assert(isdigit(**line));
    poly_exp_t exp;

    if (ReadExp(line, &exp)) {
        if (PolyIsZero(&coeff))
            exp = 0;

        *m = MonoFromPoly(&coeff, exp);
        return true;
    } else {
        PolyDestroy(&coeff);
        return false;
    }
}

/**
 * Wczytuje wielomian zapisany w linii będący stałą.
 * Funkcja pomocnicza do ReadPolyHelp.
 * @param[in,out] line : linia w której wielomian jest zapisany,
 * po wykonaniu wskaźnik wskazuje na pierwszy znak po liczbie
 * @param[out] p : wielomian wyjściowy
 * @return true jeśli wielomian jest poprawny, false wpp
 */
static bool ReadConstant(const char **line, Poly *p) {
    char *rest;
    poly_coeff_t coeff;

    if (ReadLong(&coeff, *line, &rest) && !isdigit(*rest)) {
        *line = rest;
        *p = PolyFromCoeff(coeff);
        return true;
    }

    return false;
}

/**
 * Wczytuje wielomian zapisany w linii będący sumą jednomianów lub jednomianem.
 * Funkcja pomocnicza do ReadPolyHelp.
 * @param[in,out] line : linia w której wielomian jest zapisany,
 * po wykonaniu wskaźnik wskazuje na pierwszy znak po znaku konczącym ostani z jednomianów
 * @param[out] p : wielomian wyjściowy
 * @return true jeśli wielomian jest poprawny, false wpp
 */
static bool ReadSumOfMonos(const char **line, Poly *p) {
    Mono *monos = NULL;
    size_t size = 0;
    size_t actual = 0;

    while (**line == START_MONO) {
        if (actual == size)
            ExtendMonos(&monos, &size);

        if (!ReadMono(line, &(monos[actual++]))) {
            DestroyMonos(monos, actual - 1);
            return false;
        }

        if (**line == PLUS)
            (*line)++;
    }

    *p = PolyAddMonos(actual, monos);
    free(monos);
    return true;
}

/**
 * Wczytuje wielomian zapisany w linii.
 * Funkcja pomocnicza do ReadPoly.
 * @param[in, out] line : linia w której wielomian jest zapisany,
 * po wykonaniu wskaźnik wskazuje na pierwszy znak po znaku kończącym wielomian
 * @param[out] p : wielomian wyjściowy
 * @return true jeśli wielomian jest poprawny, false wpp
 */
static bool ReadPolyHelp(const char **line, Poly *p) {
    assert(isdigit(**line) || **line == MINUS || **line == START_MONO);

    if (isdigit(**line) || **line == MINUS)
        return ReadConstant(line, p);

    return ReadSumOfMonos(line, p);
}

// Sprawdzanie poprawności zapisanego wielomianu.

/**
 * Sprawdza, czy znak jest poprawnie usytuowanym przecinkiem.
 * Funkcja pomocnicza do CheckPoly.
 * @param[in] actual : sprawdzany znak
 * @param[in] last : poprzedni znak
 * @return true jeśli jest poprawnym przecinkiem, false wpp
 */
static bool IsProperComma(char actual, char last) {
    return (actual == COMMA && (last == COEFF || last == END_MONO));
}

/**
 * Sprawdza, czy znak jest poprawnie usytuowanym zakończeniem jednomianu.
 * Funkcja pomocnicza do CheckPoly.
 * @param[in] actual : sprawdzany znak
 * @param[in] last : poprzedni znak
 * @return true jeśli jest poprawnym zakończeniem jednomianu, false wpp
 */
static bool IsProperEndMono(char actual, char last) {
    return (actual == END_MONO && last == EXP);
}

/**
 * Sprawdza, czy znak jest poprawnie usytuowanym początkiem jednomianu.
 * Funkcja pomocnicza do CheckPoly.
 * @param[in] actual : sprawdzany znak
 * @param[in] last : poprzedni znak
 * @return true jeśli jest poprawnym początkiem jednomianu, false wpp
 */
static bool IsProperStartMono(char actual, char last) {
    return (actual == START_MONO && (last == START_MONO || last == PLUS));
}

/**
 * Sprawdza, czy znak jest poprawnie usytuowanym minusem.
 * Funkcja pomocnicza do CheckPoly.
 * @param[in] actual : sprawdzany znak
 * @param[in] last : poprzedni znak
 * @return true jeśli jest poprawnym minusem, false wpp
 */
static bool IsProperMinus(char const *actual, char last) {
    return (*actual == MINUS && last == START_MONO && isdigit(actual[1]));
}

/**
* Sprawdza, czy znak jest poprawnie usytuowanym plusem.
* Funkcja pomocnicza do CheckPoly.
* @param[in] actual : sprawdzany znak
* @param[in] last : poprzedni znak
* @return true jeśli jest poprawnym plusem, false wpp
*/
static bool IsProperPlus(char const *actual, char last) {
    return (*actual == PLUS && last == END_MONO && actual[1] == START_MONO);
}

/**
* Sprawdza, czy znak jest poprawnie usytuowaną cyfrą.
* Sprawdza od razu czy jest to współczynnik, czy wykładnik jednomianu.
* Funkcja pomocnicza do CheckPoly.
* @param[in] actual : sprawdzany znak
* @param[in,out] last : poprzedni znak
* @return true jeśli jest poprawną cyfrą, false wpp
*/
static bool IsProperDigit(char actual, char *last) {
    if (isdigit(actual)) {
        if (*last == START_MONO || *last == MINUS) {
            *last = COEFF;
            return true;
        } else if (*last == COMMA) {
            *last = EXP;
            return true;
        } else {
            return false;
        }
    }

    return false;
}


/**
 * Sprawdza, czy wielomian zapisany w linii jest poprawny.
 * Ignoruje zakresy współczynników i wykładników, sprawdza tylko
 * kolejność części, z których się składa i niepoprawne znaki.
 * @param[in] line : linia w której wielomian jest zapisany
 * @return true jeśli wielomian jest poprawnie zapisany, false wpp
 */
static bool CheckPoly(const char *line) {
    char last = START_MONO;
    int balance = 0;

    while (*line != NEW_LINE) {
        if (balance < 0) return false;

        if (IsProperStartMono(*line, last)) {
            balance += 2;
        } else if (IsProperComma(*line, last) || IsProperEndMono(*line, last)) {
            balance--;
        } else if (IsProperDigit(*line, &last)) {
            while (isdigit(*line))
                line++;

            continue;
        } else {
            if (!IsProperMinus(line, last) && !IsProperPlus(line, last))
                return false;
        }

        last = *line++;
    }

    return balance == 0;
}

// Funkcja z interfejsu do wczytywania wielomianu.

bool ReadPoly(const char *line, Poly *p) {
    assert(!isalpha(*line));

    const char *pointer = line;

    if (!CheckPoly(line))
        return false;

    if (ReadPolyHelp(&pointer, p))
        return true;

    return false;
}

/**
 * Sprawdza, czy w linijce zapisane jest podane polecenie.
 * @param[in] line : linia w której zapisane jest polecenie
 * @param[in] ins : polecenie
 * @return true jeśli w lini zapisane jest podane polecenie, false wpp
 */
static bool IsInstruction(const char *line, Instruction ins) {
    const char *name = ins_names[ins];
    size_t size = strlen(name);

    if (ins == DEG_BY || ins == AT || ins == COMPOSE)
        return strncmp(line, name, size) == 0 && isspace(*(line + size));

    return (strncmp(line, name, size) == 0);

}

// Funckcja z interfejsu do poleceń

bool ReadAction(const char *line, Instruction *ins) {
    assert(isalpha(*line));

    if (IsInstruction(line, AT))
        *ins = AT;
    else if (IsInstruction(line, ADD))
        *ins = ADD;
    else if (IsInstruction(line, MUL))
        *ins = MUL;
    else if (IsInstruction(line, NEG))
        *ins = NEG;
    else if (IsInstruction(line, SUB))
        *ins = SUB;
    else if (IsInstruction(line, DEG))
        *ins = DEG;
    else if (IsInstruction(line, POP))
        *ins = POP;
    else if (IsInstruction(line, ZERO))
        *ins = ZERO;
    else if (IsInstruction(line, IS_EQ))
        *ins = IS_EQ;
    else if (IsInstruction(line, PRINT))
        *ins = PRINT;
    else if (IsInstruction(line, CLONE))
        *ins = CLONE;
    else if (IsInstruction(line, DEG_BY))
        *ins = DEG_BY;
    else if (IsInstruction(line, IS_ZERO))
        *ins = IS_ZERO;
    else if (IsInstruction(line, IS_COEFF))
        *ins = IS_COEFF;
    else if (IsInstruction(line, COMPOSE))
        *ins = COMPOSE;
    else
        return false;

    return true;
}
