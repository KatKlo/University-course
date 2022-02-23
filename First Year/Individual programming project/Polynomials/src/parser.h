/** @file
  Interfejs parsera dla kalkulatora działającego na wielomianach
  i stosujący odwrotną notację polską

  @authors Katarzyna Kloc <kk429317@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#ifndef POLYNOMIALS_PARSER_H
#define POLYNOMIALS_PARSER_H

#include "poly.h"

/**
 * To jest struktura wyliczeniowa przechowująca możliwe polecenia.
 */
typedef enum Instruction {
    ZERO, /**< wstawia na wierzchołek stosu wielomian tożsamościowo równy zeru */
    IS_COEFF, /**< sprawdza, czy wielomian na wierzchołku stosu jest
 * tożsamościowo równy zeru – wypisuje na standardowe wyjście 0 lub 1 */
    IS_ZERO, /**< sprawdza, czy wielomian na wierzchołku stosu jest
 * tożsamościowo równy zeru – wypisuje na standardowe wyjście 0 lub 1 */
    CLONE, /**< wstawia na stos kopię wielomianu z wierzchołka */
    ADD, /**< dodaje dwa wielomiany z wierzchu stosu,
 * usuwa je i wstawia na wierzchołek stosu ich sumę */
    MUL, /**< mnoży dwa wielomiany z wierzchu stosu,
 * usuwa je i wstawia na wierzchołek stosu ich iloczyn */
    NEG, /**< neguje wielomian na wierzchołku stosu */
    SUB, /**< odejmuje od wielomianu z wierzchołka wielomian pod wierzchołkiem,
 * usuwa je i wstawia na wierzchołek stosu różnicę */
    IS_EQ, /**< sprawdza, czy dwa wielomiany na wierzchu stosu są równe
 * – wypisuje na standardowe wyjście 0 lub 1 */
    DEG, /**< wypisuje na standardowe wyjście stopień wielomianu
 * (−1 dla wielomianu tożsamościowo równego zeru) */
    DEG_BY, /**< wypisuje na standardowe wyjście stopień wielomianu ze względu
 * na zmienną o numerze idx (−1 dla wielomianu tożsamościowo równego zeru) */
    AT, /**< wylicza wartość wielomianu w punkcie x, usuwa wielomian
 * z wierzchołka i wstawia na stos wynik operacji */
    PRINT, /**< wypisuje na standardowe wyjście wielomian z wierzchołka stosu */
    POP, /**< usuwa wielomian z wierzchołka stosu */
    COMPOSE /*** zdejmuje z wierzchołka stosu najpierw wielomian p, a potem
 * kolejno wielomiany q[k - 1], q[k - 2], ..., q[0] i umieszcza na stosie
 * wynik operacji złożenia.*/
} Instruction;

/**
 * Definicja stałej tablicy przechowującej napisy
 * reprezentujące dane instrukcje na wejsciu
 */
static const char *const ins_names[] = {
        [ZERO] = "ZERO\n",
        [IS_COEFF] = "IS_COEFF\n",
        [IS_ZERO] = "IS_ZERO\n",
        [CLONE] = "CLONE\n",
        [ADD] = "ADD\n",
        [MUL] = "MUL\n",
        [NEG] = "NEG\n",
        [SUB] = "SUB\n",
        [IS_EQ] = "IS_EQ\n",
        [DEG] = "DEG\n",
        [DEG_BY] = "DEG_BY",
        [AT] = "AT",
        [PRINT] = "PRINT\n",
        [POP] = "POP\n",
        [COMPOSE] = "COMPOSE",
};

/**
 * Wczytuje wielomian zapisany w podanej linii.
 * @param[in] line : linia w której zapisany jest wielomian
 * @param[out] p : wczytany wielomian
 * @return true jeśli wczytany wielomian jest poprawny, false wpp
 */
bool ReadPoly(const char *line, Poly *p);

/**
 * Wczytuje polecenie zapisane w podanej linii.
 * @param[in] line : linia w której zapisane jest polecenie
 * @param[out] ins : wczytana instrukcja
 * @return true jeśli wczytana instrukcja jest poprawna, false wpp
 */
bool ReadAction(const char *line, Instruction *ins);

/**
 * Wczytuje liczbę typu long zapisaną w lini.
 * @param[out] result : liczba wyjściowa
 * @param[in] line : linia w której liczba jest zapisana
 * @param[out] rest : wskaźnik na pierwszy znak po wczytanej liczbie
 * @return true jeżeli zapisana liczba była poprawna, false wpp
 */
bool ReadLong(long *result, const char *line, char **rest);

/**
 * Wczytuje liczbę typu unsigned long zapisaną w lini.
 * @param[out] result : liczba wyjściowa
 * @param[in] line : linia w której liczba jest zapisana
 * @param[out] rest : wskaźnik na pierwszy znak po wczytanej liczbie
 * @return true jeżeli zapisana liczba była poprawna, false wpp
 */
bool ReadUnsignedLong(unsigned long *result, const char *line, char **rest);

/**
 * Wczytuje liczbę typu int zapisaną w lini.
 * @param[out] result : liczba wyjściowa
 * @param[in] line : linia w której liczba jest zapisana
 * @param[out] rest : wskaźnik na pierwszy znak po wczytanej liczbie
 * @return true jeżeli zapisana liczba była poprawna, false wpp
 */
bool ReadInt(int *result, const char *line, char **rest);

#endif //POLYNOMIALS_PARSER_H
