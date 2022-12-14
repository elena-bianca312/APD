Laborator 4

## Ex. 1

Am creat clasa MyThread care extinde clasa Java Threads, pentru a putea
suprascrie functia run(). In Main, am initializat Thread-urile cu noua clasa
si le-am lansat folosind functia start().


## Rezolvarea bug-urilor

### bug 1
Cand generam thread-urile, apelam functia run() in loc de start().
Acest fapt conduce la o derulare secventiala a programului. Inlocuind
cu start(), vom avea si un race condition deoarece toate thread-urile
incearca in acelasi timp sa scrie in value, deci vom avea rezultate
diferite la fiecare rulare.
	
### bug 2
In clasa MyThread avem un bloc synchronized in interiorul altui bloc
synchronized. Acestea nu se blocheaza deoarece depind de aceeasi in-
stanta, iar blocurile synchronized din Java sunt reentrante.

### bug 3
Desi in functia run, avem zone critice diferite in functie de id, lock-
urile lor sunt string-urile a si b care, in Java, au aceeasi referinta.
Acest lucru se intampla pentru ca string-urile nu au fost declarate ca 
instante noi ale clasei String (new String) si au si aceeasi valoare,
deci Java urmareste pastrarea memoriei.
Prin aceasta optimizare, noi vom avea de fapt 2 zone de cod synchronized
care vor depinde de acelasi lock. Asadar, ele nu se vor executa conco-
mitent, iar valoarea va fi calculata corect, dar de fapt secvential.
Daca aveam siruri diferite, ar fi existat un race condition si algo-
ritmul nu ar mai fi avut comportamentul asteptat.
	
### bug 4
Odata lansat threadul in interiorul caruia variabile keepRunning este
si ramane mereu pe true, programul nu se va termina. Astfel, trebuie
sa intervenim sa schimbam valoarea lui keepRunning chiar si din afara
thread-ului. Acest lucru se obtine prin adaugarea keyword-ului
volatile; variabila nu mai intra in cache, va ramane mereu in RAM, fiind
vizibila pe toate thread-urile.
	
### bug 5
Fiecare thread incepe procesul pe un lock (A, respectiv B). Insa, cand
acceseaza lock-ul interior, blocat de lock-ul opus, de fapt se asteapta
unul pe celalalt si vor intra in dead-lock. Daca am bloca lock-ul 
interior cu lock-ul celui exterior, primul thread care este lansat o va
lua inaintea celuilalt si se vor suprapune for-urile din pasii 1 si 2
ai functiei. Astfel, din cauza race condition-ului, valoarea thread-ului
codas va fi suprascrisa de cateva ori si va fi mai mica.
Solutia pentru ca functia sa-si pastreze comportamentul asteptat:
2 zone synchronized separate, neimbricate, guvernate de lock-uri opuse.
Cand se termina primul for la ambele va incepe executia celui de-al
doilea for pentru ambele functii.
	
### bug 6
Fiind lansate mai multe thread-uri, in anumite situatii, unele pot intra
in primul if din Singleton in acelasi timp; astfel vor fi create mai 
multe instante (race condition). Astfel, vom face functia syncronized
(aka adaugam un mutex) pentru a scapa de probleme.
	
	
## Double Vector Elements
	
Am creat clasa MyThread, de data aceasta care implementeaza interfata
Runnable si am definit in constructor datele de care aveam nevoie pentru para-
lelizare (start, end, etc). Am suprascris functia run() pentru a avea comporta-
mentul dorit pe vectorul v (impartit in functie de numarul de thread-uri
bineinteles).


## Shortest Paths Floyd-Warshall
	
Am procedat in mod asemanator cu exercitiul anterior. Am paralelizat
for-ul din mijloc si am adaugat si o bariera pentru a mentine flow-ul corect
al for-urilor si a exersa lucrul cu bariera in Java. Rezultatul este cel corect.
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
