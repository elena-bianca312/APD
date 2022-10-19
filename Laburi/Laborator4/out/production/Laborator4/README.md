# Laborator 4

## Ex. 1

Am creat clasa MyThread care extinde clasa Java Threads, pentru a putea suprascrie functia run().
In Main, am initializat Thread-urile cu noua clasa si le-am lansat folosind functia start().


## Rezolvarea bug-urilor

### bug 1
Cand generam thread-urile, apelam functia run() in loc de start(). Acest fapt conduce la o derulare secventiala a programului.
Inlocuind cu start(), vom avea si un race condition deoarece toate thread-urile incearca in acelasi timp sa scrie in value, deci vom avea rezultate diferite la fiecare rulare.
	
### bug 2
In clasa MyThread avem un bloc synchronized in interiorul altui bloc synchronized.
Acestea nu se blocheaza deoarece depind de aceeasi instanta, iar blocurile synchronized din Java sunt reentrante.

### bug 3
Desi in functia run, avem zone critice diferite in functie de id, lock-urile lor sunt string-urile a si b care, in Java, au aceeasi referinta.
Prin aceasta optimizare, noi vom avea de fapt 2 zone de cod synchronized care vor depinde de acelasi lock.
