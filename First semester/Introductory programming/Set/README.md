# First project assigment - Simulation of game SET

##Full description of the task in polish:

###Wprowadzenie
SET (po angielsku zbiór, zestaw) jest grą karcianą dla dowolnej liczby graczy. Jest to gra czasu rzeczywistego - nie określa kolejności zagrań.

W SET gra się specjalnymi kartami, które mają po cztery atrybuty przyjmujące jedną z trzech wartości:

- liczbę figur: jedną, dwie lub trzy,

- ich kolor: czerwony, zielony lub fioletowy,

- wypełnienie: puste, pasiaste lub pełne,

- oraz kształt: romb, fala lub owal.

Każda z 3 * 3 * 3 * 3 = 81 kart w talii ma inne wartości atrybutów.

Setem nazywamy trójkę kart, w której każdy atrybut albo na wszystkich kartach ma tę samą wartość albo na każdej ma inną. Setem są np. karty:

- dwa czerwone pełne romby + trzy fioletowe pełne fale + jeden zielony pełny owal

ale nie:

- trzy zielone puste fale + jedna czerwona pełna fala + dwie zielone pasiaste fale

Grę zaczyna się od ułożenia na stole 12 odkrytych kart, wyjętych z potasowanej talii. Zwykle kładzie się je w trzech rzędach po cztery karty, ale nie ma to znaczenia dla rozgrywki. My założymy, że karty uporządkowane są w ciąg w kolejności wyjmowania z talii. Pozycje w ciągu będziemy numerować liczbami naturalnymi.

Gra w SET polega na wyszukiwaniu na stole trójek kart tworzących set. Gracz, który sądzi, że znalazł taką trójkę, rezerwuje dostęp do stołu mówiąc "set". Od tej chwili ma kilka sekund na wskazanie trzech kart. Jeśli wskaże set, zabiera go, w przeciwnym przypadku ponosi jakąś, uzgodnioną wcześniej przez graczy, karę. Jeżeli po zabraniu seta na stole jest mniej niż 12 kart a talia jest jeszcze niepusta, stół uzupełniany jest przez dodanie na koniec trzech kart z talii.

Gdy wszyscy gracze stwierdzą, że nie widzą seta, z talii wykłada się trzy dodatkowe karty. Czasem może więc być ich na stole 15 a nawet więcej. Jeśli gracze nie widzą seta a talia jest już pusta, gra kończy się. Wynikiem każdego gracza jest liczba zdobytych przez niego setów.

###Polecenie
Napisz program symulujący końcówkę gry w SET. Przyjmij, że gracze zawsze wybierają "pierwszy" spośród setów leżących na stole, tzn. taki, przed którym nie ma żadnego innego. Uznajemy, że set złożony z kart na pozycjach x < y < z jest przed setem na pozycjach x' < y' < z', jeśli:

(x < x') lub (x = x' i y < y') lub (x = x' i y = y' i z < z').

###Postać danych
Dane programu to ciąg liczb całkowitych z przedziału od 1111 do 3333, bez powtórzeń. Długość ciągu jest podzielna przez 3. Liczby są zapisane dziesiętnie cyframi ze zbioru {'1', '2', '3'}.

Dane są dowolnie sformatowane za pomocą spacji i podziału na wiersze.

Liczby reprezentują karty pozostające w grze. Na stole leży pierwszych 12 kart, lub wszystkie, gdy jest ich mniej niż 12. Pozostałe karty są w talii. Kolejność kart na stole i w talii jest zgodna z ich kolejnością w danych programu.

Liczba zapisana dziesiętnie jako ABCD reprezentuje kartę o atrybutach:

- jeden - gdy A to 1,

- dwa - gdy A to 2,

- trzy - gdy A to 3,

- czerwony - gdy B to 1,

- zielony - gdy B to 2,

- fioletowy - gdy B to 3,

- puste - gdy C to 1,

- pasiaste - gdy C to 2,

- pełne - gdy C to 3,

- romb - gdy D to 1,

- fala - gdy D to 2,

- owal - gdy D to 3.

###Postać wyniku
Program pisze na wyjście początkowy stan stołu, zapis ciągu zagrań, aż do końca rozgrywki i stan stołu po każdym zagraniu. Zagraniem jest usunięcie setu i ewentualne uzupełnienie stołu do 12 kart lub dodanie 3 kart z talii, jeśli na stole nie było setu.

Zapis stanu stołu jest wierszem zaczynającym się od znaku '='. Po nim wypisujemy karty, w kolejności, jaką mają na stole.

Usunięcie setu zapisujemy w wierszu zaczynającym się od znaku '-'. Po nim podajemy trzy karty tworzące set, w kolejności, w jakiej leżały na stole.

Pobranie dodatkowych kart, gdy na stole nie było setu, jest reprezentowane przez wiersz, w którym jest tylko znak '+'.

Wypisując ciąg kart na stole lub w secie, reprezentujemy je tak samo, jak w danych wejściowych. Każdą kartę poprzedzamy dokładnie jedną spacją.

###Przykłady
Do treści zadania dołączone są pliki .in z przykładowymi danymi i pliki .out z wynikami wzorcowymi.

- Dla danych przyklad1.in poprawny wynik to przyklad1.out .

- Dla danych przyklad2.in poprawny wynik to przyklad2.out .

- Dla danych przyklad3.in poprawny wynik to przyklad3.out .