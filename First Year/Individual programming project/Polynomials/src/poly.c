/** @file
  Implementacja biblioteki podstawowych operacji
  na wielomianach rzadkich wielu zmiennych

  @authors Katarzyna Kloc <kk429317@students.mimuw.edu.pl>
  @copyright Uniwersytet Warszawski
  @date 2021
*/

#include "poly.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

//Makra wykorzystywane w wypisywaniu wielomianu

#define NEW_LINE '\n' ///< Nowa linia
#define START_MONO '(' ///< Oznaczenie początku jednomianu
#define END_MONO ')' ///< Oznaczenie końca jednomianu
#define PLUS '+' ///< Oznaczenie sumy jednomianów
#define COMMA ',' ///< Symbol rozdzilający wpółczynnik od wykładnika

/**
 * Dynamicznie alokuje pamięc podanego rozmiaru.
 * Jeżeli zabraknie pamięci to kończy program awaryjnie kodem 1.
 * @param[in] size : rozmiar pamięci do zaalokowania
 * @return wskaźnik na zaalokowaną pamięć
 */
static void *MyMalloc(size_t size) {
    assert(size > 0);

    void *res = malloc(size);

    if (res == NULL)
        exit(1);

    return res;
}

/**
 * Dynamicznie realokuje tablice jednomianów.
 * Jeżeli zabraknie pamięci to kończy program awaryjnie kodem 1.
 * @param[in] size : nowy rozmiar tablicy
 * @param[in] monos : wskaźnik na tablicę jednomianów
 */
static void MyRealloc(Mono **monos, size_t size) {
    if (size == 0) {
        free(*monos);
        *monos = NULL;
    } else {
        *monos = realloc(*monos, size);

        if (*monos == NULL)
            exit(1);
    }
}

/**
 * Funkcja porównująca do funkcji qsort.
 * Porównuje wykładniki jednomianoów.
 * @param[in] a : pierwszy jednomian
 * @param[in] b : drugi jednomian
 * @return >0 jeśli wykładnik pierwszy jest większy,
 * 0 jeśli równe, <0 jeśli drugi jest większy
 */
static int CompareMonosExp(const void *a, const void *b) {
    const Mono arg1 = *(Mono *) a;
    const Mono arg2 = *(Mono *) b;

    return MonoGetExp(&arg1) - MonoGetExp(&arg2);
}

/**
 * Upraszcza wielomian jeśli może.
 * @param[in] p : wskaźnik na wielomian
 */
static void PolySimplify(Poly *p) {
    if (!PolyIsCoeff(p) && p->size == 1) {
        Mono current = p->arr[0];

        if (MonoGetExp(&current) == 0 && PolyIsCoeff(&(current.p))) {
            Poly res = current.p;
            free(p->arr);
            *p = res;
        } else if (PolyIsZero(&(current.p))) {
            PolyDestroy(p);
            *p = PolyZero();
        }
    }
}

static void PolyPrintHelp(const Poly *p);

/**
 * Wypisuje jednomian.
 * @param[in] m : jednomian do wypisania
 */
static void MonoPrint(const Mono *m) {
    putchar(START_MONO);
    PolyPrintHelp(&(m->p));
    putchar(COMMA);
    printf("%d", MonoGetExp(m));
    putchar(END_MONO);
}

/**
 * Wypisuje wielomian.
 * @param[in] p : wielomian do wypisania
 */
static void PolyPrintHelp(const Poly *p) {
    if (PolyIsCoeff(p)) {
        printf("%ld", p->coeff);
    } else {
        for (size_t i = 0; i < p->size - 1; i++) {
            MonoPrint(&(p->arr[i]));
            putchar(PLUS);
        }

        MonoPrint(&(p->arr[p->size - 1]));
    }
}

void PolyPrint(const Poly *p) {
    PolyPrintHelp(p);
    putchar(NEW_LINE);
}

void PolyDestroy(Poly *p) {
    if (!PolyIsCoeff(p)) {
        for (size_t i = 0; i < p->size; i++)
            MonoDestroy(&(p->arr[i]));

        free(p->arr);
    }

}

Poly PolyClone(const Poly *p) {
    if (PolyIsCoeff(p))
        return *p;

    Poly res;
    res.size = p->size;
    res.arr = (Mono *) MyMalloc(res.size * sizeof *res.arr);

    for (size_t i = 0; i < res.size; i++)
        res.arr[i] = MonoClone(&(p->arr[i]));

    return res;
}

/**
 * Funkcja pomocnicza do PolyAdd.
 * Dodaje dwa wielominay gdy drugi z nich jest współczynnikiem.
 * @param[in] p : wielomian nie bedący współczynnikiem
 * @param[in] coeff : wielomian bedący współczynnikiem
 * @return wielomian będący sumą @f$p + coeff@f$
 */
static Poly PolyAddCoeff(const Poly *p, const Poly *coeff) {
    Poly res;
    Poly temp;
    temp.size = 1;
    temp.arr = (Mono *) MyMalloc(sizeof *temp.arr);
    temp.arr[0] = MonoFromPoly(coeff, 0);
    res = PolyAdd(p, &temp);
    PolyDestroy(&temp);
    PolySimplify(&res);

    return res;
}

/**
 * Funkcja pomocnicza do PolyAdd.
 * Dodaje dwa wielominay, przy czym oba nie są współczynnikami.
 * @param[in] p : wielomian
 * @param[in] q : wielomian
 * @return wielomaian będący sumą @f$p + q@f$
 */
static Poly PolyAddPolys(const Poly *p, const Poly *q) {
    Poly res;
    res.arr = (Mono *) MyMalloc((p->size + q->size) * sizeof *res.arr);

    size_t i = 0, j = 0, z = 0;
    while (i < p->size && j < q->size) {
        int diff_exp = CompareMonosExp(&(p->arr[i]), &(q->arr[j]));

        if (diff_exp < 0) {
            res.arr[z] = MonoClone(&(p->arr[i++]));
        } else if (diff_exp > 0) {
            res.arr[z] = MonoClone(&(q->arr[j++]));
        } else {
            res.arr[z].exp = MonoGetExp(&(q->arr[j]));
            res.arr[z].p = PolyAdd(&(p->arr[i++].p), &(q->arr[j++].p));
        }

        if (!PolyIsZero(&(res.arr[z].p)))
            z++;
        else
            MonoDestroy(&(res.arr[z]));
    }

    while (i < p->size)
        res.arr[z++] = MonoClone(&(p->arr[i++]));

    while (j < q->size)
        res.arr[z++] = MonoClone(&(q->arr[j++]));

    res.size = z;
    MyRealloc(&(res.arr), res.size * sizeof *res.arr);
    PolySimplify(&res);

    return res;
}

Poly PolyAdd(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) && PolyIsCoeff(q))
        return PolyFromCoeff(p->coeff + q->coeff);
    if (PolyIsCoeff(p))
        return PolyAddCoeff(q, p);
    if (PolyIsCoeff(q))
        return PolyAddCoeff(p, q);

    return PolyAddPolys(p, q);
}

/**
 * Upraszcza zawartość tablicy jednomianów.
 * @param[in, out] count : liczba jednomianów
 * @param[in, out] monos : tablica jednomianów (przejęta na własność)
 */
static void MonosSimplify(size_t *count, Mono *monos) {
    qsort((void *) monos, *count, sizeof *monos, CompareMonosExp);

    size_t size = 0;
    for (size_t act = 1; act < *count; act++) {
        if (MonoGetExp(&(monos[size])) < MonoGetExp(&(monos[act]))) {
            if (!PolyIsZero(&(monos[size].p)))
                size++;

            monos[size] = monos[act];
        } else {
            Poly temp = PolyAdd(&(monos[size].p), &(monos[act].p));
            PolyDestroy(&(monos[size].p));
            MonoDestroy(&(monos[act]));
            monos[size].p = temp;
        }
    }

    if (!PolyIsZero(&(monos[size].p)))
        size++;

    *count = size;
}

Poly PolyOwnMonos(size_t count, Mono *monos) {
    if (count == 0 || monos == NULL) {
        if (monos != NULL)
            free(monos);

        return PolyZero();
    }

    Poly res;
    res.size = count;
    res.arr = monos;

    MonosSimplify(&res.size, res.arr);
    MyRealloc(&(res.arr), res.size * sizeof *res.arr);
    PolySimplify(&res);

    return res;
}

Poly PolyAddMonos(size_t count, const Mono monos[]) {
    if (count == 0 || monos == NULL)
        return PolyZero();

    Mono *copyMonos = (Mono *) MyMalloc(count * sizeof *copyMonos);
    for (size_t i = 0; i < count; i++)
        copyMonos[i] = monos[i];

    return PolyOwnMonos(count, copyMonos);
}

Poly PolyCloneMonos(size_t count, const Mono monos[]) {
    if (count == 0 || monos == NULL)
        return PolyZero();

    Mono *copyMonos = (Mono *) MyMalloc(count * sizeof *copyMonos);
    for (size_t i = 0; i < count; i++)
        copyMonos[i] = MonoClone(&monos[i]);

    return PolyOwnMonos(count, copyMonos);
}

/**
 * Funkcja pomocnicza do PolyMul.
 * Mnoży wielomian razy współczynnik.
 * @param[in] p : wielomian
 * @param[in] scalar : współczynnik
 * @return wielomaian będący iloczynem @f$p * scalar@f$
 */
static Poly PolyMulScalar(const Poly *p, poly_coeff_t scalar) {
    if (PolyIsCoeff(p))
        return PolyFromCoeff(scalar * p->coeff);

    Poly res;
    res.arr = (Mono *) MyMalloc(p->size * sizeof *res.arr);

    size_t z = 0;
    for (size_t i = 0; i < p->size; i++) {
        Poly temp = PolyMulScalar(&(p->arr[i].p), scalar);

        if (!PolyIsZero(&temp))
            res.arr[z++] = MonoFromPoly(&temp, MonoGetExp(&(p->arr[i])));
    }

    res.size = z;
    MyRealloc(&(res.arr), res.size * sizeof *res.arr);

    PolySimplify(&res);

    return res;
}

/**
 * Funkcja pomocnicza do PolyMul.
 * Mnoży dwa wielomiany, przy czym oba nie są współczynnikami.
 * @param[in] p : wielomian
 * @param[in] q : wielomian
 * @return wielomian będący iloczynem @f$p * q@f$
 */
static Poly PolyMulPolys(const Poly *p, const Poly *q) {
    Poly res;
    Poly temp;
    temp.size = p->size * q->size;
    temp.arr = (Mono *) MyMalloc(temp.size * sizeof *temp.arr);

    for (size_t z = 0, i = 0; i < p->size; i++) {
        for (size_t j = 0; j < q->size; j++, z++) {
            temp.arr[z].p = PolyMul(&(p->arr[i].p), &(q->arr[j].p));
            temp.arr[z].exp = MonoGetExp(&(p->arr[i])) + MonoGetExp(&(q->arr[j]));
        }
    }

    res = PolyOwnMonos(temp.size, temp.arr);
    PolySimplify(&res);

    return res;
}

Poly PolyMul(const Poly *p, const Poly *q) {
    if (PolyIsZero(p) || PolyIsZero(q))
        return PolyZero();
    if (PolyIsCoeff(p))
        return PolyMulScalar(q, p->coeff);
    if (PolyIsCoeff(q))
        return PolyMulScalar(p, q->coeff);

    return PolyMulPolys(p, q);

}

Poly PolyNeg(const Poly *p) {
    if (PolyIsCoeff(p))
        return PolyFromCoeff(0 - p->coeff);

    Poly res;
    res.size = p->size;
    res.arr = (Mono *) MyMalloc(res.size * sizeof *res.arr);

    for (size_t i = 0; i < p->size; i++) {
        res.arr[i].exp = MonoGetExp(&(p->arr[i]));
        res.arr[i].p = PolyNeg(&(p->arr[i].p));
    }

    return res;
}

Poly PolySub(const Poly *p, const Poly *q) {
    Poly res;
    Poly negQ = PolyNeg(q);
    res = PolyAdd(p, &negQ);
    PolyDestroy(&negQ);

    return res;
}

poly_exp_t PolyDegBy(const Poly *p, size_t var_idx) {
    if (PolyIsZero(p))
        return -1;

    if (PolyIsCoeff(p))
        return 0;

    if (var_idx == 0)
        return MonoGetExp(&(p->arr[p->size - 1]));

    poly_exp_t temp;
    poly_exp_t res = -1;

    for (size_t i = 0; i < p->size; i++) {
        temp = PolyDegBy(&(p->arr[i].p), var_idx - 1);

        if (temp > res)
            res = temp;
    }

    return res;
}

poly_exp_t PolyDeg(const Poly *p) {
    if (PolyIsZero(p))
        return -1;
    if (PolyIsCoeff(p))
        return 0;

    poly_exp_t res = -1;
    poly_exp_t temp;

    for (size_t i = 0; i < p->size; i++) {
        temp = PolyDeg(&(p->arr[i].p)) + MonoGetExp(&(p->arr[i]));
        if (temp > res)
            res = temp;
    }

    return res;
}

bool PolyIsEq(const Poly *p, const Poly *q) {
    if (PolyIsCoeff(p) != PolyIsCoeff(q))
        return false;
    if (PolyIsCoeff(p) && PolyIsCoeff(q))
        return p->coeff == q->coeff;
    if (p->size != q->size)
        return false;

    for (size_t i = 0; i < p->size; i++) {
        if (MonoGetExp(&(p->arr[i])) != MonoGetExp(&(q->arr[i])))
            return false;
        if (!PolyIsEq(&(p->arr[i].p), &(q->arr[i].p)))
            return false;
    }

    return true;
}


/**
 * Funkcja pomocnicza do PolyAt.
 * Szybkie podnoszenie do potęgi.
 * @param[in] x : podstawa
 * @param[in] n : wykładnik
 * @return wynik potęgowania @f$x ^ n@f$
 */
static poly_coeff_t FastExp(poly_coeff_t x, poly_exp_t n) {
    if (n == 0)
        return 1;
    if (n % 2 == 1)
        return x * FastExp(x, n - 1);

    poly_coeff_t temp = FastExp(x, n / 2);

    return temp * temp;
}


Poly PolyAt(const Poly *p, poly_coeff_t x) {
    if (PolyIsCoeff(p))
        return *p;

    Poly res = PolyZero();
    Poly temp_Poly1;
    Poly temp_Poly2;
    poly_coeff_t scalar;

    for (size_t i = 0; i < p->size; i++) {
        scalar = FastExp(x, MonoGetExp(&(p->arr[i])));
        temp_Poly1 = PolyMulScalar(&(p->arr[i].p), scalar);
        temp_Poly2 = PolyAdd(&res, &temp_Poly1);
        PolyDestroy(&res);
        PolyDestroy(&temp_Poly1);
        res = temp_Poly2;
    }

    return res;
}

/**
 * Szybkie podnoszenie wielominaów do potęgi.
 * Funkcja pomocnicza do MonoCompose.
 * @param[in] p : podstawa
 * @param[in] exp : wykładnik
 * @return wynik potęgowania @f$p ^ exp@f$
 */
static Poly PolyExp(const Poly *p, poly_exp_t exp) {
    Poly res;

    if (exp == 0) {
        res = PolyFromCoeff(1);
    } else if (exp % 2 == 1) {
        Poly temp = PolyExp(p, exp - 1);
        res = PolyMul(p, &temp);
        PolyDestroy(&temp);
    } else {
        Poly temp = PolyExp(p, exp / 2);
        res = PolyMul(&temp, &temp);
        PolyDestroy(&temp);
    }

    return res;
}

/**
 * Wykonuje operację składania jednomianu.
 * Funkcja pomocnicza do PolyCompose.
 * @param[in] m : jednomian w którym dokonujemy podstawnia
 * @param[in] k : liczba wielomianów do podstawienia
 * @param[in] q : tablica wielomianów do podstawenia
 * @return wielomian powstały w wyniku składania
 */
static Poly MonoCompose(const Mono *m, size_t k, const Poly q[]) {
    Poly res;

    if (k == 0) {
        if (MonoGetExp(m) != 0)
            return PolyZero();

        res = PolyCompose(&(m->p), k, q);
    } else {
        Poly temp1 = PolyCompose(&(m->p), k - 1, q + 1);
        Poly temp2 = PolyExp(&(q[0]), m->exp);
        res = PolyMul(&temp1, &temp2);
        PolyDestroy(&temp1);
        PolyDestroy(&temp2);
    }

    return res;
}

Poly PolyCompose(const Poly *p, size_t k, const Poly q[]) {
    if (PolyIsCoeff(p))
        return PolyClone(p);

    Poly res = PolyZero();

    for (size_t i = 0; i < p->size; i++) {
        Poly temp1 = MonoCompose(&(p->arr[i]), k, q);
        Poly temp2 = PolyAdd(&temp1, &res);
        PolyDestroy(&temp1);
        PolyDestroy(&res);
        res = temp2;
    }

    return res;
}
