Echipa: Dumitru Elena, Nicolescu Andrei, Paun Miruna, Tomescu Petru-Alin
Grupa: 331CC

# Laborator 7

Am paralelizat folosind ExecutorService si ForkJoinPool 3 probleme de
backtracking (task-urile 1->6).
Am folosit Future si CompletableFuture impreuna cu ExecutorService si 
ForkJoinPool pentru a rezolva problema cautarii unei valori (task 7) si
calcularii inaltimii (task 8) unui arbore binar de cautare.

## Task 1

Folosind ExecutorService, am paralelizat problema gasirii tuturor cailor intr-un
graf de la un nod sursa la un nod destinatie. Initial se adauga in coada de
task-uri cel corespunzator nodului sursa. Apoi, fiecare task va genera la randul
sau alte noi task-uri, pe masura ce sunt gasite noi noduri dintr-o anumita cale
catre nodul destinatie. De asemenea, avem nevoie si de un contor pentru a tine
evidenta numarului de task-uri aflate la momentul actual in coada - vom folosi
un AtomicInteger. Avem nevoie de aceasta informatie pentru a sti cand sa ne
oprim din acceptarea de noi task-uri.

## Task 2

Folosind ExecutorService, am paralelizat problema gasirii tuturor colorarilor
posibile ale nodurilor dintr-un graf, pentru un numar dat de culori. Ideea este
similara celei de la task 1, dar, de data aceasta, nu am mai avut nevoie de un
contor pentru numarul de elemente din coada de task-uri datorita modului de
alcatuire a problemei, in care toate solutiile vor avea aceeasi lungime.

### Observatie

Folosirea unui contor ca la task 1 NU este gresita in acest caz, ci doar
redundanta.

## Task 3

Folosind ExecutorService, am paralelizat problema damelor. Principiul este
acelasi ca la rezolvarea "clasica" cu backtracking dar, in schimb, adaugam
task-uri in coada in loc sa ne folosim de stiva de executie. Ideea este aceeasi
cu cea de la task-urile anterioare.

### Observatie

Folosirea unui contor ca la task 1 NU este gresita in acest caz, ci doar
redundanta.

## Task 4 + 5 + 6

Aceste task-uri sunt identice primelor trei, cerinta fiind sa se foloseasca
ForkJoinPool. Dupa cum se poate vedea, schimbarile aduse fata de varianta cu
ExecutorService sunt minimale, ideea de baza fiind, in esenta, aceeasi.

## Task 7

Am folosit CompletableFuture pentru a cauta o valoare data intr-un arbore binar
de cautare. Ideea este similara task-urilor anterioare, cu diferenta ca vom
retine rezultatul cautarii in obiectul nostru de tip CompletableFuture. De
asemenea, avem nevoie si de acel contor pentru numarul elementelor din coada de
task-uri pentru a sti cand sa ne oprim din acceptat task-uri si cand sa punem
rezultatul corespunzator in CompletableFuture.

## Task 8

Am folosit ForkJoinPool prin intermediul clasei abstracte ForkJoinTask care
implementeaza interfata Future si, mai departe, prin intermediul clasei
RecursiveTask ce implementeaza ForkJoinTask, care are in spate mecanismul bazat
pe Future, pentru a calcula inaltimea unui arbore binar de cautare.
