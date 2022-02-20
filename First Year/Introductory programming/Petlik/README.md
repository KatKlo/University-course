# Assigment project 3 - Language Pętlik
Compile to machine code and compute abstract programming language - Petlik

## Full description of the task in polish:

### Wprowadzenie
Rozważamy realizację języka programowania Pętlik. Składnię języka opisuje gramatyka z symbolem początkowym Program:

- Program → CiągInstrukcji
- CiągInstrukcji → ε | CiągInstrukcji Instrukcja
- Instrukcja → Zwiększ | Powtarzaj
- Zwiększ → Zmienna
- Powtarzaj → '(' Zmienna CiągInstrukcji ')'
- Zmienna → 'a' | .. | 'z'

Program składa się ze znaków, które w gramatyce są ujęte w apostrofy. Oprócz nich żadne inne znaki, nawet spacje lub końce wiersza, w kodzie źródłowym nie mogą wystąpić.

Program ma dostęp do 26 zmiennych, których wartości są nieujemnymi liczbami całkowitymi.

Instrukcje wyprowadzone z symbolu CiągInstrukcji są wykonywane w kolejności od pierwszej do ostatniej.

Instrukcja Zwiększ w postaci zmienna jest równoważna instrukcji języka C:

```c
++zmienna;
```

Instrukcja Powtarzaj w postaci (zmienna...) jest równoważna instrukcji języka C:

```c
while (zmienna > 0) {
    --zmienna;
    ...
}
```

Realizacja języka składa się z kompilatora optymalizującego, generującego kod na maszynę wirtualną, oraz z interpretera kodu tej maszyny.

Maszyna wykonuje instrukcje:

* INC Zmienna (increment) - zwiększ o jeden wartość zmiennej Zmienna,

* ADD Zmienna0 Zmienna1 (add) - dodaj do zmiennej Zmienna0 wartość zmiennej Zmienna1,

* CLR Zmienna (clear) - wyzeruj zmienną Zmienna,

* JMP Adres (jump) -skocz do instrukcji o adresie Adres,

* DJZ Zmienna Adres (decrement or jump if zero) - jeśli Zmienna ma wartość 0 to skocz do instrukcji o adresie Adres, w przeciwnym przypadku zmniejsz o jeden wartość zmiennej Zmienna,

* HLT (halt) - zakończ wykonywanie programu.

Wykonanie programu w języku maszynowym zaczyna się od pierwszej instrukcji.

Jeśli instrukcja nie określa inaczej, to po jej wykonaniu przechodzimy do instrukcji, która jest w kodzie bezpośrednio za nią.

Dla ciągu instrukcji kompilator generuje kod w kolejności od pierwszej do ostatniej. Kod wygenerowany dla programu kończy instrukcją HLT.

Jeżeli w instrukcji Powtarzaj nie jest zagnieżdżona inna instrukcja Powtarzaj, czyli w nawiasach jest ciąg zmiennych Zmienna0, ..., ZmiennaN, dla N >= 0, i jeżeli żadna ze zmiennych Zmienna1, ..., ZmiennaN nie jest zmienną Zmienna0, to kompilator generuje kod zoptymalizowany postaci:

```c
ADD Zmienna1 Zmienna0
...
ADD ZmiennaN Zmienna0
CLR Zmienna0
```

Jeśli nie są spełnione warunki umożliwiające wygenerowanie dla instrukcji kodu zoptymalizowanego, to generujemy kod zwykły.

Kodem zwykłym dla instrukcji Zwiększ postaci Zmienna jest:
```c
INC Zmienna
```
Kodem zwykłym dla instrukcji Powtarzaj postaci (Zmienna...) jest:
```c
DJZ Zmienna Koniec
...
JMP Początek
```
gdzie Początek to adres instrukcji DJZ w tym ciągu a Koniec to adres instrukcji bezpośrednio za instrukcją JMP.

### Polecenie
Napisz program będący realizacją języka Pętlik.

Program czyta i wykonuje polecenia:

* wypisania wartości zmiennej,

* wykonania programu w języku Pętlik.

Przed wykonaniem pierwszego polecenia wartości wszystkich zmiennych są równe 0.

Zmienne zachowują wartości po wykonaniu polecenia. Nie są zerowane przed każdym wykonaniem programu.

### Postać danych
Dane programu to ciąg wierszy, w każdym po jednym poleceniu.

Polecenie wypisania wartości zmiennej zaczyna się od znaku =, po którym jest nazwa zmiennej.

Wiersz polecenia wykonania programu zawiera kod źródłowy w języku Pętlik.

### Postać wyniku
Wynik programu jest efektem wykonania poleceń wypisania wartości zmiennej.

Wartość zmiennej piszemy dziesiętnie, w jednym wierszu, bez nieznaczących zer wiodących.

Wartość 0 jest reprezentowana przez cyfrę 0 a wartości dodatnie zaczynają się od cyfry niezerowej.

### Przykłady
Do treści zadania dołączone są pliki .in z przykładowymi danymi i pliki .out z wynikami wzorcowymi.

- Dla danych przyklad1.in poprawny wynik to przyklad1.out .

- Dla danych przyklad2.in poprawny wynik to przyklad2.out .

- Dla danych przyklad3.in poprawny wynik to przyklad3.out .