Echipa: Dumitru Elena, Nicolescu Andrei, Paun Iulia Miruna, Tomescu Petru - Alin

Grupa: 331CC

# Laborator 11

## Exercitiul 1 - Algoritmul inel cu functii non-blocante

Este o implementare a algoritmului inel folosind functiile de send si receive
din MPI in varianta lor non-blocanta (MPI_Isend si MPI_Irecv).

Primul proces, care porneste algoritmul inel, va genera un numar aleatoriu, pe
care-l va trimite procesului din dreapta lui. Acest proces va primi numarul dat
de procesul din stanga, il va incrementa cu 2 si-l va trimite mai departe
procesului din dreapta sa.
Acest lucru se va repeta pana cand numarul ("token-ul") va ajunge inapoi la
procesul care a pornit algoritmul.

## Exercitiul 2 - Bariere

Exercitiul presupune plasarea de bariere astfel incat algoritmul sa afiseze
mereu sirul "1\n2\n3\n".

Barierele trebuie plasate intre cele 3 "if"-uri pentru ca fiecare proces sa
printeze corespunzator.

Cu toate acestea, la rulari repetate se va observa ca rezultatul nu este
intotdeauna corect (apar cazuri cand ordinea se inverseaza, in ciuda prezentei
barierelor). Acest fapt se datoreaza modului de implementare a functiei printf.
Nici daca adaugam fflush(stdout) problema nu este rezolvata. Pentru mai multe
detalii, se poate consulta link-ul:

<https://stackoverflow.com/questions/5182045/openmpi-mpi-barrier-problems>

## Exercitiul 3 - Remediere deadlock

Din cauza modului de implementare a functiilor blocante MPI_Send si MPI_Recv, in
cazuri in care se trimit cantitati mari de date pot aparea situatii de tip
deadlock.

Astfel, pentru exercitiul actual, in care avem o astfel de situatie, vom analiza
3 modalitati de rezolvare a problemei, care sunt prezentate si in link-ul:

<http://www.idris.fr/eng/su/shared/mpi_send_recv-eng.html>

### Varianta 1 - folosind MPI_Sendrecv

Prima varianta este sa inlocuim MPI_Send urmat de MPI_Recv cu un singur apel al
functiei MPI_Sendrecv.

### Varianta 2 - folosind MPI_Bsend

A doua varianta este sa inlocuim MPI_Send cu MPI_Bsend - asfel cream un buffer
(folosind funcția MPI_Buffer_attach), prin intermediul căruia sunt schimbate
mesajele.

### Varianta 3 - folosind functii non-blocante si MPI_Waitall

A treia varianta este sa folosim functii non-blocante pentru trimitere si
receptie (ca la primul exercitiu - MPI_Isend si MPI_Irecv). Diferenta este ca,
in loc sa folosim acea combinatie de MPI_Test si MPI_Wait, vom folosi
MPI_Waitall.

Resurse utile:

- <https://stackoverflow.com/questions/15441163/mpi-waitall-is-failing>
- <https://www.rookiehpc.com/mpi/docs/mpi_waitall.php>

## Exercitiul 4 - Algoritmul inel cu o structura de tip coada definita in MPI

Exercitiul este asemanator primului, dar se adauga o structura de tip coada in
care sa fie introduse numerele aleatoare generate de fiecare proces.
De asemenea, fiecare proces isi va genera propriul numar aleatoriu, pe care-l va
adauga in coada. Apoi va trimite coada (cu totul) mai departe procesului
urmator. 

Structura este creata ca un nou tip de date in MPI, folosind functiile si
conventiile specifice acestui tip de operatie (pentru mai multe detalii a se
vedea si comentariile din cod si enuntul exercitiului).
