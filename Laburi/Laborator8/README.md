Echipa: Dumitru Elena, Nicolescu Andrei, Paun Miruna, Tomescu Petru-Alin
Grupa: 331CC

# Laborator 8

Acest laborator reprezinta o introducere in programarea distribuita prin folo-
sirea bibliotecii MPI.


## Exercitiul 1

Pentru a implementa algoritmul inel folosind MPI, am propagat informatia de la
un proces la altul, facand identificarea in functie de rank.
Avem un proces master care declanseaza ciclul facand MPI_Send catre procesul 
cu rang urmator. Celelalte procese procedeaza asemanator, primesc informatia
de la procesul premergator folosind functia MPI_Recv, prelucreaza informatia si
o transmit mai departe. Ultimul proces, cel cu rank-ul N-1, trebuie tratat sepa-
rat deoarece el trimite mai departe procesului ROOT = 0.

Obs! Numarul maxim de procese pe care il putem porni pe masina locala folosind
comanda simpla "mpirun -np nr_processes exec" este egal cu nr de core-uri.
Daca folosim comanda, "mpirun --use-hwthread-cpus -np nr_processed exec", putem
extinde nr_processes la numarul de thread-uri (acolo unde exista hyperthreading)


## Exercitiul 2

Pentru exercitiul 2, am apelat functia MPI_Bcast care face broadcast catre toate
procesele (inclusiv catre sursa), dintr-un proces sursa ales de programator.

Obs! Nu trebuie sa punem nicio conditie suplimentara de a face broadcast 
dintr-un anumit proces. Semnatura functiei asigura unicitatea sursei, iar toate
procesele trebuie sa aiba in componenta lor functia de broadcast pentru a putea
primi corect informatia de la sursa.


## Exercitiul 3

In acest exercitiu, am folosit functiile MPI_Scatter si MPI_Gather pentru a dis-
tribui informatia tuturor proceselor.
Procesul ROOT va distribui bucati (egale) din array-ul care trebuie procesat 
(v_send) catre toate procesele (inclusiv sursa). Acestea, primesc informatia in
vectorul v_recv, o prelucreaza si o trimit inapoi sursei cu ajutorul functiei
MPI_Gather, de data aceasta parametrii vor fi inversati (intre send si receive).

Vom printa raspunsul final acumulat in v_send, vectorul initial propus spre pre-
lucrare.


## Exercitiul 4

In acest exercitiu, toate procesele care nu sunt ROOT vor trimite o informatie
catre procesul root. Acesta va primi informatia intr-o bucla, iar in functia
MPI_Recv vom folosi MPI_ANY_SOURCE pentru a putea receptiona mesaje indiferent
de sursa.
In root, informatia este preluata din campul MPI_SOURCE al lui status.


## Exercitiul 5

In acest exercitiu, exersam lucrul cu tag-urile. Procesul 0 genereaza 10 valori
random pe care le trimite cu tag-uri random catre procesul 1. Acesta le recepti-
oneaza si afiseaza. In mod similar cu exercitiul anterior, obtinem informatia 
despre tag din status.


## Exercitiul 6

Pentru a imparti procesele in grupuri de 4, facem div intre rank si GROUP_SIZE.
Folosim functia MPI_Comm_split cu urmatorii parametri: canalul principal de co-
municatie, noul grup (culoarea) in care este asignat procesul, cheia care consta
in noul rang atribuit in cadrul subgrupului si noul subcanal de comunicatie,
aici custom_group.

Antet functie: 
MPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm * newcomm);

Citim in variabilele new_size si new_rank noile valori ale subgrupului, folosind
functiile MPI_Comm_size si MPI_Comm_rank, care actioneaza pe post de getteri
practic.

Acum trebuie sa aplicam algoritmul inelului in cadrul subgrupului: Daca este ul-
timul proces, ii trimite informatia procesului 0. Altfel, trimitem informatia
procesului urmator din cadrul subgrupului.

Fiecare proces primeste informatia folosind MPI_Recv() si printeaza informatiile
primite. Finalizam procesul, ca la orice cod MPI.







