# Laborator 9

## Exercitiul 1 - Reduction

Pentru a implementa operatia de reduction, trebuie mai intai ca fiecare proces,
pe baza rankului si a pasului curent, fie sa trimita valoarea lor calculata catre 
un alt proces, sau sa primeasca valoarea partiala de la alt proces si sa isi adauge
si valoarea sa. Pe masura ce pasul creste, numarul proceselor ce primesc valori
intermediare scade. In final, valoarea cumulata va fi detinuta de procesul master.
In cadrul exercitiului, am realizat imultirea dintre valoarea (rank + 1) a fiecarui
proces.

## Exercitiul 2 - Scan

Asemanator exercitiului anterior, vom acumula rezultatul inmultirii valorilor
(rank + 1). De data aceasta produsul final se va regasi in cadrul ultimului proces.

## Exercitiul 3 - Broadcast

La inceput, primul proces va trimite valoarea catre al doilea. Dupa ce acesta o
va primi, pasul va fi incrementat. In urmatoarea etapa, primele 2 procese vor trimite
valoarea, primul catre al 3 - lea proces, al 2 - lea catre al 4 - lea. Dupa ce valorile
sunt primite, vom creste iar pasul.
La fiecare pas, un numar de m procese ce au valoarea o vor trimite fiecare catre procesul
i + m, unde i este indexul procesului ce a trimis. Dupa primirea valorii, vom avea 2 * m
procese ce cunosc valoarea. Algoritmul se opreste cand exact prima jumatate dintre procese
cunosc valoarea. Restul urmand sa o primeasca de la acestea. La final, toate procesele 
au obtinut valoarea ce trebuia trimisa prin broadcast.

## Exercitiul 4 - polynomial

Procesul master va trimite catre restul proceselor valoarea coeficientului lor. Dupa aceasta
operatie, procesul master va face un broadcast cu valoarea lui x, ce va fi folosita in
calculul polinomului. In final, procesul master trimite valoarea calculata de el (c1 * (x^0))
catre procesul 1.
Restul proceselor vor primi coeficientul termenilor lor, precum si valoarea lui x prin broadcast.
Ele isi vor calcula termenul lor. Apoi vor astepta de la procesul anterior valoarea partiala
a polinomului. La aceasta valoare, isi vor adauga termenul lor, iar in cele din urma o vor trimite
catre procesul urmator, sau vor afisa valoarea finala, in cazul ultimului proces.

## Exercitiul 5 - pipeline sort

Procesul master va face un broadcast cu valoarea -1 catre toate procesele initial. Apoi va trimite
catre procesul 1, pe rand, elementele vectorului ce trebuie sortat. Masterul apoi face receive la 
valorile alese de celelalte procese, ele reprezentand vectorul sortat.
Celelalte procese decurg astfel: Procesul i va primi elementele ramase nesortate de la procesul i - 1.
La inceput va inlocui valoarea initiala -1 cu prima primita. Pe masura ce procesul i mai primeste un
element, daca acesta este mai mare decat cel retinut in variabila sa, il va trimite la procesul i + 1.
Daca e mai mic, va trimite la procesul i + 1 variabila sa, si o va inlocui apoi cu cea primita anterior.
In final, ele vor trimite valoarea salvata catre procesul master.

## Exercitiul 6 - rank sort

In cadrul procesului master, am generat vectorul, asigurandu-ne ca nu avem valori duplicat. Am
determinat lungimea intervalului ce va fi prelucrat de un proces si am trimis, prin broadcast 
vectorul v. Dupa ce procesul master isi completeaza intervalul sau din vectorul de pozitii, va
avea loc un gather, pentru a obtine intervalele din vectorul de pozitii calculate si de celelalte
procese. In final, procesul master va aranja elementele conform vectorului de pozitii, astfel obtinand
vectorul sortat.
Restul proceselor vor primi vectorul initial prin broadcast. Isi vor completa intervalul in vectorul
de pozitii iar apoi vor trimite in cadrul gather - ului intervalele lor calculate. 
