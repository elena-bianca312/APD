# Tema 1 - APD
# Dumitru Elena Bianca
# 331 CC

------------------------------------------------------------------
------------------------- Introducere ----------------------------
------------------------------------------------------------------

	In aceasta tema, a trebuit sa paralelizam algoritmul propus pentru a 
pune in practica transformarea genetica pe parcursul mai multor generatii.


------------------------------------------------------------------
-------------- Initializarea thread-urilor in main ---------------
------------------------------------------------------------------

	Observam ca functia principala este run_genetic_algorithm care apeleaza
restul functiilor ajutatoare, asa ca vom adapta aceasta functie pentru a fi cea
cu care initializam thread-urile. Pentru a lucra cu thread-uri, trebuie sa pa-
sam ca parametru functiei o structura cu datele necesare.

	Asadar, am inclus in structura id-ul fiecarui thread (necesar ulterior
pentru a calcula start-ul si end-ul pentru operatii repetate), dar si referinte
catre instante comune, care trebuie sa fie modificate de toate thread-urile in
paralel, precum: current_generation, next_generation, bariera, cursor, count, 
etc. De asemenea, thread-urile trebuie sa aiba acces la datele comune: objects,
object_count, sack_capacity, generations_count, P.


-------------------------------------------------------------------
-------------------- Functiile principale -------------------------
-------------------------------------------------------------------

	Modificam functie de read_input pentru a citi de la tastatura si numa-
rul de thread-uri pentru care vrem sa ruleze algoritmul.

	Ulterior, functia principala propusa spre paralelizare este
run_genetic_algorithm. Aceasta va primi ca parametru un pointer de tip void*,
pe care il vom casta pe structura noastra. In continuare, paralelizam primul
for, pentru ca fiecare thread sa se ocupe de cate o parte a vectorilor de
current_generation, next_generaion, respectiv copy (structura ajutatoare pentru
sortare).

	For-ul mare nu este paralelizat, deoarece acesta urmareste algoritmul in
decursul mai multor generatii, deci nu ar avea sens. Paralelizam functia 
compute_fitness_function, tot asa cu start si stop pentru a calcula in mod 
corect fitness-ul fiecarui individ.

	Urmeaza functia de sortare, un mergeSort paralel, pentru ca toate 
thread-urile sa contribuie la sortare (aceasta parte fiind cea mai costisitoare
din punct de vedere al timpului din tot programul).

	Pentru implementare, am luat merge sort din lab3 cu putine modificari.
Functia de merge ramane la fel (cea care uneste cele 2 array-uri prin compara-
tii alternative). Functia de sortare folosita este cmpfunc, ca in codul initial.

	In schimb, logica se afla in functia mergeSort: deoarece lucram paralel,
trebuie sa ne gandim cum impartim array-ul astfel incat fiecare thread sa aiba 
de lucru. Asadar, approach-ul pentru merge sort va fi de tip bottom-up. Preluam 
formulele din laborator: pentru a face o impartire corecta, trebuie sa impartim 
la 2 * width si sa inmultim din nou. Insa, pentru ca de aceasta data, dimensiu-
nea array-ul nu este mereu putere a lui 2 sau impartirea pe thread-uri nu este 
simetrica, trebuie sa luam ceil() de impartire pentru a ne asigura ca toate va-
lorile sunt prelucrate.

	In continuare, for-ul este normal, fiecare thread lucrand pe segmentul
sau. Trebuie sa tinem cont de inmultirea cu 2 * width si in pas-ul for-ului
pentru a avea un numar corect de pasi.

	Dupa prelucrarea pe fiecare nivel de width, trebuie sa transferam noile
valori (care au fost calculate in componenta copy a structurii de parametri)
din nou in current_generation, care la final va fi sortat si gata de prelucrari
ulterioare. Dupa ce facem transferul, punem o bariera pentru a ne asigura ca
toate thread-urile incep calculul urmatorului nivel in acelasi timp.

	Paralelizam for-urile urmatoare, recalculand start-ul si stop-ul in 
functie de noul count. 

	Pentru partea de crossover first 30% parents with one-point crossover,
trebuie sa tinem cont ca nu putem avea valori impare. Asadar, modificam count.
In for-ul de dupa observam ca pasul este +2, deci trebuie sa modificam si start
sa aiba o valoare para pentru a nu avea suprapuneri. Daca start este impar in-
seamna ca am ajuns la ultimul individ din generatie care trebuie pastrat pentru
generatia urmatoare.

	Urmeaza functia paralelizata calculate_set_chromosomes care calculeaza
numarul de cromozomi setati pentru fiecare individ. Observam ca functia folosi-
ta pentru sortare tine cont de acest aspect cand valorile de fitness sunt egale
si ii calculeaza. Insa, la cat de multe repetari avem intr-o functie de sortare,
acest fapt devine prea costisitor, deci vom calcula numarul de biti sortati
doar o data pe generatie. Astfel, timpul necesar sortarii se reduce considera-
bil.
*cromozom setat = are valoarea 1

	Paralelizam transferul catre o noua generatie si printam best_fitness
dintr-un singur thread pentru a nu avea dubluri (am ales thread-ul 0 pentru ca
acesta exista intotdeauna).

	La final, realizam pentru ultima oara: compute_fitness_function(), sor-
tarea si afisarea. La final, eliberam memoria si ii semnalam thread-ului ca s-a
termina functia prin apelul pthread_exit().

	In main, dam join thread-urilor si eliberam sursele de memorie comune 
folosite.


























