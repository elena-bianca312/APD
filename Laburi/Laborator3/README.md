Echipa: Dumitru Elena, Nicolescu Andrei, Paun Miruna, Tomescu Petru
Grupa: 331CC

# Laborator 3

## Ex. 1

Am implementat varianta paralelizata a algoritmului "Odd Even Transpositon
Sort". La fiecare iteratie a for-ului exterior intai se va executa partea
"para", apoi cea "impara" a algoritmului. Astfel, toate thread-urile vor rula in
paralel pentru partea para, apoi se vor astepta sa se termine, apoi vor rula in
paralel pentru partea impara, apoi se vor astepta sa se termine si se va trece
la noua iteratie din for-ul exterior (care va rula de maxim N ori).

### Observatii

1) Deoarece fiecare thread va avea partea sa din vector - adica un "start" si un
"end" calculate conform formulelor discutate la laboratoarele anterioare -
trebuie sa ne asiguram ca, indiferent de thread, se va mentine distinctia dintre
faza para si cea impara. Din aceasta cauza, trebuie sa facem acea verificare
suplimentara pentru a vedea paritatea lui "start".

2) Testarea s-a realizat cu script-ul din laborator "test_oets.sh".

## Ex. 2

Am implementat varianta paralelizata a algoritmului "Shear Sort". La fiecare
iteratie a for-ului exterior intai se vor sorta toate liniile din matrice - cele
cu index par crescator, iar cele cu index impar descrescator. Apoi, dupa ce s-au
sortat astfel toate liniile, se vor sorta toate coloanele din matrice crescator.
Astfel, toate thread-urile vor rula in paralel pentru partea de sortare a
liniilor, iar, dupa terminarea acestei operatii - deoarece nu se poate incepe
sortarea coloanelor fara terminarea sortarii tuturor liniilor - se vor sorta in
paralel toate coloanele. In final, asteptam sa se termine sortarea tuturor
coloanelor, dupa care trecem la urmatoarea iteratie a for-ului exterior.

### Observatii

1) Testarea s-a realizat cu script-ul din laborator "test_shear.sh".

## Ex. 3

Am implementat varianta paralelizata a algoritmului "Merge Sort". Spre deosebire
de algoritmul "clasic", care se scrie recursiv, avand o abordare de tip
"Top-Down", algoritmul paralel necesita o abordare de tip "Bottom-Up". Prin
urmare, vom porni sortarea de la nivelul "cel mai de jos", de la intervalele de
marime 2 (i.e. cu 2 elemente) si vom aplica operatia de "merge" pe ele. Apoi vom
continua pe fiecare nivel (al arborelui de executie care se formeaza).
Bineinteles, intrucat fiecare nivel superior depinde de cele inferioare, va
trebui sa asteptam ca toate thread-urile sa termine cu un nivel inainte sa
trecem la urmatorul. Deoarece pe masura ce avansam pe niveluri numarul de
intervale scade si deci, incarcarea per thread scade - ajungand ca, la
nivelurile de sus, chiar sa avem thread-uri ce nu sunt utilizate - avem nevoie
de acel "start_local" si "end_local", care se calculeaza conform formulelor din
laborator (nu le voi detalia).

### Observatii

1) Am pornit de la implementarea neparalelizata a algortimului prezenta in
scheletul laboratorului, care garanteaza functionarea corecta pentru vectori de
o marime egala cu o putere a lui 2.

2) Testarea s-a realizat cu script-ul din laborator "test_merge.sh".

3) Am adus mici schimbari pentru a optimiza functia "merge" data in schelet,
pentru a elimina nevoia folosirii unui vector destinatie (adica schimbarile se
vor regasi direct in vectorul dat ca parametru).

## Ex. 4

Am implementat varianta paralelizata a algoritmului de cautare binara. La fel ca
la exercitiul anterior, este nevoie de o abordare de tip "Bottom-Up". Practic,
fiecare thread va lua un interval din vector. Intai se verifica daca numarul
cautat se afla intr-unul dintre capetele intervalului. Daca da, ne oprim. Daca
nu, verifica daca numarul cautat este in interiorul acelui interval. Daca da,
asteapta si celelalte thread-uri sa termine cu intervalele lor, si apoi toate se
concentreaza pe acel interval, reluandu-se procedeul discutat. In caz contrar,
thread-ul se va opri si va astepta celelalte thread-uri, care cauta fiecare in
intervalul lui.

### Observatii

1) Testarea s-a realizat manual si cu script-ul din laborator
"test_parallel_search.sh".

