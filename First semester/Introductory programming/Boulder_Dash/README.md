# Assigment project 2: Boulder Dash game

## Full description of the task in polish:

### Wprowadzenie
Boulder Dash to komputerowa gra logiczno-zręcznościowa. Tematem zadania jest jej wariant pozbawiony aspektu zręcznościowego.

Rozgrywka Boulder Dasha toczy się na prostokątnej planszy, której pola są zorganizowane w wiersze i kolumny. Gracz steruje postacią Rockforda. Celem jest zebranie wszystkich diamentów, które są na planszy i opuszczenie jej przez wyjście. Oprócz Rockforda, diamentów i wyjścia, na polach planszy są również: ziemia, skały i kamienie. Na każdym polu może być co najwyżej jeden z wymienionych obiektów.

Rockford porusza się po planszy, przechodząc na jedno z czterech pól sąsiadujących w wierszu lub kolumnie z polem aktualnym. Może wejść na pole docelowe, jeżeli spełniony jest jeden z warunków:

- jest ono puste,

- jest na nim ziemia,

- jest na nim diament,

- jest na nim kamień, sąsiaduje ono z polem Rockforda w wierszu a kolejne pole w tym wierszu, licząc od strony Rockforda, jest puste,

- na polu docelowym jest wyjście a Rockford zebrał już z planszy wszystkie diamenty.

Nie można więc wejść na pole ze skałą, a na pole z wyjściem wchodzimy tylko, jeśli wcześniej zebraliśmy wszystkie diamenty.

Pole opuszczone przez Rockforda staje się puste. Rockford, wchodząc na pole z diamentem, zabiera go a wchodząc na pole z ziemią "przekopuje" ją i ziemia znika.

Jeśli na polu docelowym jest wyjście, Rockford znika z planszy. Wejście Rockforda na pole, na którym jest kamień, powoduje przesunięcie kamienia na kolejne, puste pole.

Jeżeli bezpośrednio poniżej pola, na którym jest kamień lub diament, czyli w kolejnym wierszu tej samej kolumny, jest pole puste, to ten kamień lub diament "spada" - jest przenoszony na puste pole poniżej niego.

Stan planszy nazywamy stabilnym, jeśli poniżej żadnego kamienia i poniżej żadnego diamentu nie ma pustego pola. Przed i po każdym ruchu Rockforda wszystkie kamienie i diamenty, które mogą to zrobić, spadają, sprowadzając planszę do stanu stabilnego.

Inaczej, niż w oryginalnej grze Boulder Dash, w naszym wariancie kamień lub diament spadający na inny kamień lub diament nie "ześlizguje się" z niego. Poza tym, Rockford nie ginie, gdy spadnie na niego kamień lub diament.

### Polecenie
Napisz program, który czyta opis początkowego stanu planszy Boulder Dasha, sprowadza planszę do stanu stabilnego i pisze ten stan na wyjście. Następnie czyta i wykonuje polecenia użytkownika, aż do ich końca.

Program obsługuje polecenie przesunięcia Rockforda i polecenie drukowania aktualnego, stabilnego stanu planszy.

Polecenie przesunięcia Rockforda nie zmienia stanu planszy jeśli ruch określony tym poleceniem nie jest wykonalny lub Rockforda nie ma już na planszy.

### Postać danych
W pierwszym wierszu danych jest, rozdzielona spacją, para dodatnich liczb całkowitych w, k. Kolejne w wierszy zawiera opis początkowego, niekoniecznie stabilnego, stanu planszy. W każdym z tych wierszy jest k znaków. Reszta danych to polecenia użytkownika.

Wiersze opisu planszy uporządkowane są w kolejności od góry planszy a zawartość wiersza w kolejności od lewej do prawej.

Stan każdego pola jest opisany jednym znakiem:

- '@' - Rockford,

- ' ' - pole puste,

- '+' - ziemia,

- '#' - skała,

- 'O' - kamień,

- '$' - diament,

- 'X' - wyjście.

Na każdym polu w pierwszym i w ostatnim wierszu planszy oraz w jej pierwszej i ostatniej kolumnie jest skała. Na planszy jest co najwyżej jeden Rockford.

Polecenia przesunięcia Rockforda są reprezentowane przez znaki:

- 'w' - w górę,

- 'a' - w lewo,

- 's' - w dół,

- 'd' - w prawo.

Polecenie drukowania stanu planszy jest reprezentowane przez koniec wiersza '\n'.

### Postać wyniku
Wynik programu jest efektem wykonania poleceń drukowania aktualnego stanu planszy.

Stan planszy piszemy w takim samym formacie, jaki miał stan początkowy na wejściu.

### Przykłady
Do treści zadania dołączone są pliki .in z przykładowymi danymi i pliki .out z wynikami wzorcowymi.

- Dla danych przyklad1.in poprawny wynik to przyklad1.out .

- Dla danych przyklad2.in poprawny wynik to przyklad2.out .

- Dla danych przyklad3.in poprawny wynik to przyklad3.out .